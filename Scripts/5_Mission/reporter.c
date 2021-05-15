class _ServerPlayerEx : _ServerPlayer {
    void _ServerPlayerEx(PlayerBase player) {
        if(player != NULL) {
            this.id = player.GetPlainId(); // Steam64
            this.name = player.GetPlayerName();

            this.position = player.GetPosition();
            this.health = player.GetHealth("GlobalHealth", "Health");
            if(GetGame().GetMission().IsPlayerDisconnecting(player))
                this.loggingOut = 1;

            if(player.IsInVehicle())
                this.insideVehicle = 1;
        }

        if(player.GetItemInHands())
            this.item = player.GetItemInHands().GetType();
    };
};

class _Callback_ServerPoll : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll errorCode(%1)", errorCode));
        if(errorCode == 6 || errorCode == 5) {
            // Server side error, only thrown when either offline or authentication error
            GetGameLabs().GetLogger().Error(string.Format("GameLabs API error, reauthenticating..."));
            GetGameLabs().GetApi().RegisterAsync();
        }
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_ServerPoll response = new _Response_ServerPoll(data);
        GetGameLabs().GetLogger().Debug(string.Format("ServerPoll OnSuccess(%1) : %2", response, data));

        Man man;
        PlayerBase player;
        ServerPollItem order;
        int q;
        for ( int i = 0; i < response.orders.Count(); i++ ) {
            order = response.orders.Get(i);
            // TODO: Add new abstraction layer in 4_World
            if(order.action == "teleport") { //GetGameLabs()._TeleportPlayer(order.target, order.x, order.y);
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Teleporting %1 to %2, %3", order.target, order.x, order.y));
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);

                    vector position;
                    position[0] = order.x;
                    position[1] = GetGame().SurfaceY(order.x, order.y) + 0.2;
                    position[2] = order.y;
                    player.SetPosition(position);
                }
            } else if(order.action == "heal") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Healing %1", order.target));
                // GetGameLabs()._KillPlayer(order.target);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);

                    player.SetHealth(player.GetMaxHealth("", ""));
                    player.SetHealth("", "Blood", player.GetMaxHealth("", "Blood"));
                    player.GetStatHeatComfort().Set(player.GetStatHeatComfort().GetMax());
                    player.GetStatTremor().Set(player.GetStatTremor().GetMin());
                    player.GetStatWet().Set(player.GetStatWet().GetMin());
                    player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
                    player.GetStatWater().Set(player.GetStatWater().GetMax());
                    player.GetStatDiet().Set(player.GetStatDiet().GetMax());
                    player.GetStatSpecialty().Set(player.GetStatSpecialty().GetMax());
                    player.SetBleedingBits(0);
                }
            }
            else if(order.action == "kill") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Killing %1", order.target));
                // GetGameLabs()._HealPlayer(order.target);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);
                    player.SetHealth(0);
                }
            }
            else if(order.action == "spawn") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Spawning %1 for %2 (x%3)", order.parameter, order.target, order.quantity));
                // GetGameLabs()._SpawnItemForPlayer(order.target, order.item);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);
                    if(order.quantity == 1) player.SpawnEntityOnGroundPos(order.parameter, player.GetPosition());
                    else {
                        for (q = 1; q <= order.quantity; q++) player.SpawnEntityOnGroundPos(order.parameter, player.GetPosition());
                    }
                }
            }
            else if(order.action == "spawnat") {
                vector position;
                position[0] = order.x;
                position[1] = GetGame().SurfaceY(order.x, order.y) + 0.1;
                position[2] = order.y;

                GetGameLabs().GetLogger().Warn(string.Format("[Order] Spawning %1 at %2 (x%3)", order.parameter, position, order.quantity));

                if(order.quantity == 1) GetGame().CreateObjectEx(order.parameter, position, ECE_PLACE_ON_SURFACE);
                else {
                    for (q = 1; q <= order.quantity; q++) GetGame().CreateObjectEx(order.parameter, position, ECE_PLACE_ON_SURFACE);
                }
            }
        }
    };
};

class _Callback_ServerDummy : _Callback {};

class GameLabsReporter {
    private bool processReporting = true;

    private int effectiveInterval = 10;
    private int lastSent = 0;
    private int lastReportedPlayerCount = 0;

    private bool isFirstReport = true;
    private bool isFirstEventsReport = true;

    private ref Timer timerPoll;
    private ref Timer timerServer;
    private ref Timer timerPlayers;

    void GameLabsReporter() {
        this.timerPoll = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerPoll.Run(GetGameLabs().GetMetricsInterval(), this, "activePolling", NULL, true);

        if(GetGameLabs().IsReportingEnabled()) {
            this.effectiveInterval = GetGameLabs().GetReportingInterval();
            this.timerServer = new Timer(CALL_CATEGORY_SYSTEM);
            this.timerServer.Run(GetGameLabs().GetReportingInterval(), this, "serverReporting", NULL, true);
        }
    }

    void Disable() {
        this.processReporting = false;
        GetGameLabs().GetLogger().Info("(Reporter) Disabled");
        this.timerPoll.Stop();
        this.timerServer.Stop();
        GetGameLabs().GetLogger().Debug("(Reporter) Timers gracefully closed");
    }

    private void activePolling() {
        if(!this.processReporting || !GetGameLabs()) return;
        ref _Payload_ServerPoll payload = new ref _Payload_ServerPoll(this.isFirstEventsReport);
        GetGameLabs().GetApi().Enable(); // Enable to check if api is back
        GetGameLabs().GetApi().ServerPoll(new _Callback_ServerPoll(), payload);
        if(this.isFirstEventsReport) this.isFirstEventsReport = false;
    }

    private void serverReporting() {
        if(!this.processReporting  || !GetGameLabs()) return;
        if(GetGameLabs()._serverEventsBufferAdded.Count() || GetGameLabs()._serverEventsBufferRemoved.Count() || this.isFirstReport) {
            ref _Payload_ServerEvents payloadEvents = new _Payload_ServerEvents(this.isFirstReport, GetGameLabs()._serverEventsBufferAdded, GetGameLabs()._serverEventsBufferRemoved);

            GetGameLabs().GetApi().ServerEvents(new _Callback_ServerDummy(), payloadEvents);

            GetGameLabs()._serverEventsBufferAdded.Clear();
            GetGameLabs()._serverEventsBufferRemoved.Clear();
        }

        ref array<ref _Vehicle> updated = new array<ref _Vehicle>();
        for(int i = 0; i < GetGameLabs().GetVehicles().Count(); i++) {
            ref _Vehicle vehicle = GetGameLabs().GetVehicles().Get(i);
            if(vehicle.HasUpdated()) updated.Insert(vehicle);
        }

        if(GetGameLabs()._serverEventsBufferAdded.Count() || GetGameLabs()._serverEventsBufferRemoved.Count() || updated.Count() || this.isFirstReport) {
            ref _Payload_ServerVehicles payloadVehicles = new _Payload_ServerVehicles(this.isFirstReport, GetGameLabs()._serverVehiclesBufferAdded, updated, GetGameLabs()._serverVehiclesBufferRemoved);

            GetGameLabs().GetApi().ServerVehicles(new _Callback_ServerDummy(), payloadVehicles);

            GetGameLabs()._serverVehiclesBufferAdded.Clear();
            GetGameLabs()._serverVehiclesBufferRemoved.Clear();
        }

        PlayerBase player;
        ref array<Man> players = new array<Man>;
        GetGame().GetPlayers( players );

        ref array<ref _ServerPlayer> updatedPlayers = new array<ref _ServerPlayer>();
        for ( int x = 0; x < players.Count(); x++ ) {
            player = PlayerBase.Cast(players.Get(x));
            updatedPlayers.Insert(new _ServerPlayerEx(player));
        }
        if(players.Count() || players.Count() != this.lastReportedPlayerCount || this.isFirstReport) {
            this.lastReportedPlayerCount = players.Count();
            ref _Payload_ServerPlayers payloadPlayers = new _Payload_ServerPlayers(this.isFirstReport, this.effectiveInterval, updatedPlayers);
            GetGameLabs().GetApi().ServerPlayers(new _Callback_ServerDummy(), payloadPlayers);
        }

        if(this.isFirstReport) this.isFirstReport = false;
    }
};
