class GameLabsCore {
    private const string modControlledVersionIdentifier = "1.0";

    private ref GameLabsAPI api;
    private ref GameLabsLogger logger;
    private ref GameLabsConfiguration configuration;

    bool errorFlag = false;
    string modLicensingOffender;

    private int _computedServerFps = 0;
    private int _computedAI = 0;
    private int _computedAnimals = 0;
    private int _computedVehicles = 0;

    private int _metricsInterval = 5;
    private int _reportingInterval = 10;
    private bool _reportingEnabled = false;
    private bool _reportStatistics = false;
    private ref array <ref string> _monitoredActions = new array<ref string>;

    private ref array<ref _AI> _serverAI = new array<ref _AI>;
    private ref array<ref _Event> _serverEvents = new array<ref _Event>;
    private ref array<ref _Vehicle> _serverVehicles = new array<ref _Vehicle>;

    ref array<ref _Event> _serverEventsBufferAdded = new array<ref _Event>;
    ref array<ref _Event> _serverEventsBufferRemoved = new array<ref _Event>;

    ref array<ref _Vehicle> _serverVehiclesBufferAdded = new array<ref _Vehicle>;
    ref array<ref _Vehicle> _serverVehiclesBufferRemoved = new array<ref _Vehicle>;

    void GameLabsCore() {
        // TODO: Complete bootstrapping
        this.configuration = GameLabsConfiguration();
        if(!this.configuration.CheckDiskPresence()) {
            // Shut down when no config is present on server
            if(GetGame().IsServer()) {
                this.errorFlag = true;
            } else {
                // Instance logger with logging disabled
                this.logger = GameLabsLogger("GameLabsCore", false);
            }
        } else {
            this.configuration.LoadFromDisk();
            this.logger = GameLabsLogger("GameLabsCore", this.GetDebugStatus());

            this.api = GameLabsAPI(this.configuration.GetServerId(), this.configuration.GetApiKey());
        }
    }

    string GetVersionIdentifier() { return this.modControlledVersionIdentifier; }

    // Module getters
    GameLabsLogger GetLogger() { return this.logger; }
    GameLabsAPI GetApi() { return this.api; }

    // Config getter passthrough
    bool GetDebugStatus() { return this.configuration.GetDebugStatus(); }
    bool GetConnectionVerificationStatus() { return this.configuration.GetConnectionVerificationStatus(); }

    /* Public API */
    void RequestServerFPS() {
        if(GetGame().IsServer()) return;
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.RQ_SERVERFPS, NULL, true);
    }

    int GetServerFPS() { return this._computedServerFps; }
    void SetServerFPS(int fpsValue) { this._computedServerFps = fpsValue; }

    int GetAICount() { return this._computedAI; }
    void IncrAICount() { this._computedAI++; }
    void DecrAICount() { this._computedAI--; }
    int GetAIActiveCount() {
        int count = 0;
        _AI obj;
        for(int i = 0; i < this._serverAI.Count(); i++) {
            obj = this._serverAI.Get(i);
            if(obj.IsInfected() && obj.IsActive()) count++;
        }
        return count;
    }

    int GetAnimalCount() { return this._computedAnimals; }
    void IncrAnimalCount() { this._computedAnimals++; }
    void DecrAnimalCount() { this._computedAnimals--; }
    int GetAnimalActiveCount() {
        int count = 0;
        _AI obj;
        for(int i = 0; i < this._serverAI.Count(); i++) {
            obj = this._serverAI.Get(i);
            if(!obj.IsInfected() && obj.IsActive()) count++;
        }
        return count;
    }

    int GetVehicleCount() { return this._computedVehicles; }
    void IncrVehicleCount() { this._computedVehicles++; }
    void DecrVehicleCount() { this._computedVehicles--; }

    ref array<ref _AI> GetAI() {
        return this._serverAI;
    }
    void RegisterAI(ref _AI _reference) {
        this._serverAI.Insert(_reference);
    }
    void RemoveAI(ref _AI _reference) {
        for(int i = 0; i < this._serverAI.Count(); i++) {
            if(this._serverAI.Get(i) == _reference) {
                this._serverAI.Remove(i);
            }
        }
    }

    ref array<ref _Vehicle> GetVehicles() {
        return this._serverVehicles;
    }
    void RegisterVehicle(ref _Vehicle _reference) {
        this._serverVehicles.Insert(_reference);
        this._serverVehiclesBufferAdded.Insert(_reference);
    }
    void RemoveVehicle(ref _Vehicle _reference) {
        for(int i = 0; i < this._serverVehicles.Count(); i++) {
            if(this._serverVehicles.Get(i) == _reference) {
                this._serverVehiclesBufferRemoved.Insert(_reference);
                this._serverVehicles.Remove(i);
            }
        }
    }

    ref array<ref _Event> GetEvents() {
        return this._serverEvents;
    }
    void RegisterEvent(ref _Event _reference) {
        for(int i = 0; i < this._serverEvents.Count(); i++) {
            if(this._serverEvents.Get(i).Equals(_reference)) {
                return;
            }
        }
        this._serverEvents.Insert(_reference);
        this._serverEventsBufferAdded.Insert(_reference);
    }
    void RemoveEvent(ref _Event _reference) {
        for(int i = 0; i < this._serverEvents.Count(); i++) {
            if(this._serverEvents.Get(i) == _reference) {
                this._serverEventsBufferRemoved.Insert(this._serverEvents.Get(i));
                this._serverEvents.Remove(i);
            }
        }
    }

    int GetMetricsInterval() { return this._metricsInterval; }
    int GetReportingInterval() { return this._reportingInterval; }
    bool IsReportingEnabled() { return this._reportingEnabled; }
    bool IsStatReportingEnabled() { return this._reportStatistics; }
    bool IsMonitoredAction(string action) { if(this._monitoredActions.Find(action) == -1) {return false;} else {return true;}  }

    // Private calls, do not access
    void _PropagateFeatures(ref _Response_Register registerResponse) {
        if(registerResponse.modLicensingStatus == 1) {
            this.errorFlag = true;
            this.modLicensingOffender = registerResponse.modLicensingOffender;
            this.GetLogger().Error(string.Format("Server is not authorized to use %1, contact mod author for details", this.modLicensingOffender));
            return;
        }

        this._monitoredActions = registerResponse.monitoredActions;

        this._metricsInterval = registerResponse.features.metricsInterval;
        this._reportingInterval = registerResponse.features.reportingInterval;

        if(registerResponse.features.allowReporting == 2) {
            this.GetLogger().Debug("allowReporting = 2 => Reporting enabled");
            this._reportingEnabled = true;
        } else {
            this.GetLogger().Debug("allowReporting != 2 => Reporting NOT enabled");
            this._reportingEnabled = false;
        }
        if(registerResponse.features.statisticsReporting == 2) {
            this.GetLogger().Debug("statisticsReporting = 2 => Statistics enabled");
            this._reportStatistics = true;
        } else {
            this.GetLogger().Debug("statisticsReporting != 2 => Statistics NOT enabled");
            this._reportStatistics = false;
        }
    }

    Man GetPlayerBySteam64(string steam64) {
        array<Man> players = new array<Man>;
        GetGame().GetWorld().GetPlayerList( players );

        for ( int x = 0; x < players.Count(); x++ ) {
            if(players.Get(x).GetIdentity().GetPlainId() == steam64) return players.Get(x);
        }
        return NULL;
    }

    void _TeleportPlayer(string steam64, float x, float y) {
        Man player = this.GetPlayerBySteam64(steam64);
        if(player == NULL) return;
        this.GetLogger().Debug(string.Format("[Order] Teleporting %1 to %2, %3", steam64, x, y));
        vector position;
        position[0] = x;
        position[1] = GetGame().SurfaceY(x, y) + 0.2;
        position[2] = y;
        player.SetPosition(position);
    }

    /*
     * TODO: Refactor for 4_World
    void _HealPlayer(string steam64) {
        Man man = this.GetPlayerBySteam64(steam64);
        if(man == NULL) return;
        PlayerBase player = PlayerBase.Cast(man);
        this.GetLogger().Debug(string.Format("[Order] Healing %1", steam64));
        player.SetHealth( player.GetMaxHealth( "", "" ) );
        player.SetHealth( "","Blood", player.GetMaxHealth( "", "Blood" ) );
        player.GetStatHeatComfort().Set(player.GetStatHeatComfort().GetMax());
        player.GetStatTremor().Set(player.GetStatTremor().GetMin());
        player.GetStatWet().Set(player.GetStatWet().GetMin());
        player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
        player.GetStatWater().Set(player.GetStatWater().GetMax());
        player.GetStatDiet().Set(player.GetStatDiet().GetMax());
        player.GetStatSpecialty().Set(player.GetStatSpecialty().GetMax());
        player.SetBleedingBits(0);
        player.SetHealth(0.0);
    }

    void _KillPlayer(string steam64) {
        Man man = this.GetPlayerBySteam64(steam64);
        if(man == NULL) return;
        PlayerBase player = PlayerBase.Cast(man);
        this.GetLogger().Debug(string.Format("[Order] Killing %1", steam64));
        player.SetHealth(0);
    }

    void _SpawnItemForPlayer(string steam64, string item) {
        Man man = this.GetPlayerBySteam64(steam64);
        if(man == NULL) return;
        PlayerBase player = PlayerBase.Cast(man);
        this.GetLogger().Debug(string.Format("[Order] Spawning %1 for %2", item, steam64));
        player.SpawnEntityOnGroundPos(player, entity.GetPosition());
    }
     */
};

// Public interface access
private static ref GameLabsCore g_GameLabsCore;
static ref GameLabsCore GetGameLabs() {
    if(!g_GameLabsCore) {
        g_GameLabsCore = GameLabsCore();
    }
    return g_GameLabsCore;
};