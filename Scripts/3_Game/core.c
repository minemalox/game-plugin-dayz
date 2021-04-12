class GameLabsCore {
    private const string modControlledVersionIdentifier = "0.0";

    private ref GameLabsAPI api;
    private ref GameLabsLogger logger;
    private ref GameLabsConfiguration configuration;

    bool errorFlag = false;
    private int _computedServerFps = 0;

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
    int GetServerFPS() {
        if(GetGame().IsServer()) {
            return this._computedServerFps;
        } else {
            // TODO: Implement RPC based retrieval
            return -1;
        }
    }

    // Special setters
    void SetServerFPS(int fpsValue) { this._computedServerFps = fpsValue; }
};

// Public interface access
private static ref GameLabsCore g_GameLabsCore;
static ref GameLabsCore GetGameLabs() {
    if(!g_GameLabsCore) {
        g_GameLabsCore = GameLabsCore();
    }
    return g_GameLabsCore;
};