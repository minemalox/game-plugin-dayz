class _ServerPlayerEx : _ServerPlayer {
    string name;
    void _ServerPlayerEx(PlayerBase player) {
        this.id = player.GetIdentity().GetPlainId(); // Steam64
        this.name = player.GetIdentity().GetName();

        this.position = player.GetPosition();

        this.health = player.GetHealth( "GlobalHealth","Health" );
        if(player.GetItemInHands())
            this.item = player.GetItemInHands().GetType();

        if(GetGame().GetMission().IsPlayerDisconnecting(player))
            this.loggingOut = 1;
    };
};

class _Callback_ServerPoll : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll errorCode(%1)", errorCode));
        if(errorCode == 6 || errorCode == 5) {
            // Server side error, only thrown when either offline or authentication error
            GetGameLabs().GetLogger().Error(string.Format("GameLabs API error, reauthenticating..."));
            int registerStatus = GetGameLabs().GetApi().Register();
            if(registerStatus == 2) {
                GetGameLabs().GetLogger().Error(string.Format("GameLabs API Re-Enabled"));
                GetGameLabs().GetApi().Enable();
            } else {
                GetGameLabs().GetLogger().Error(string.Format("GameLabs API Disabled"));
                GetGameLabs().GetApi().Disable();
            }
        }
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_ServerPoll response = new _Response_ServerPoll(data);
        GetGameLabs().GetLogger().Debug(string.Format("ServerPoll OnSuccess(%1)", response));
    };
};

class _Callback_ServerDummy : _Callback {};

class GameLabsReporter {
    private int lastSent = 0;

    private bool isFirstReport = true;
    private bool isFirstEventsReport = true;

    private ref Timer timerPoll;
    private ref Timer timerServer;
    private ref Timer timerPlayers;

    void GameLabsReporter() {
        this.timerPoll = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerPoll.Run(GetGameLabs().GetMetricsInterval(), this, "activePolling", NULL, true);

        if(GetGameLabs().IsReportingEnabled()) {
            this.timerServer = new Timer(CALL_CATEGORY_SYSTEM);
            this.timerServer.Run(GetGameLabs().GetReportingInterval(), this, "serverReporting", NULL, true);
            this.serverReporting();
            this.isFirstReport = false;
        }
    }

    private void activePolling() {
        ref _Payload_ServerPoll payload = new ref _Payload_ServerPoll(this.isFirstEventsReport);
        GetGameLabs().GetApi().Enable(); // Enable to check if api is back
        GetGameLabs().GetApi().ServerPoll(new _Callback_ServerPoll(), payload);
        if(this.isFirstEventsReport) this.isFirstEventsReport = false;
    }

    private void serverReporting() {
        if(GetGameLabs()._serverEventsBufferAdded.Count() || GetGameLabs()._serverEventsBufferRemoved.Count()) {
            ref _Payload_ServerEvents payloadEvents = new _Payload_ServerEvents(isFirstReport, GetGameLabs()._serverEventsBufferAdded, GetGameLabs()._serverEventsBufferRemoved);

            GetGameLabs().GetApi().ServerEvents(new _Callback_ServerDummy(), payloadEvents);

            GetGameLabs()._serverEventsBufferAdded.Clear();
            GetGameLabs()._serverEventsBufferRemoved.Clear();
        }

        ref array<ref _Vehicle> updated = new array<ref _Vehicle>();
        for(int i = 0; i < GetGameLabs().GetVehicles().Count(); i++) {
            ref _Vehicle vehicle = GetGameLabs().GetVehicles().Get(i);
            if(vehicle.HasUpdated()) updated.Insert(vehicle);
        }

        if(GetGameLabs()._serverEventsBufferAdded.Count() || GetGameLabs()._serverEventsBufferRemoved.Count() || updated.Count()) {
            ref _Payload_ServerVehicles payloadVehicles = new _Payload_ServerVehicles(isFirstReport, GetGameLabs()._serverVehiclesBufferAdded, updated, GetGameLabs()._serverVehiclesBufferRemoved);

            GetGameLabs().GetApi().ServerVehicles(new _Callback_ServerDummy(), payloadVehicles);

            GetGameLabs()._serverVehiclesBufferAdded.Clear();
            GetGameLabs()._serverVehiclesBufferRemoved.Clear();
        }

        ref array<Man> players = new array<Man>;
        GetGame().GetPlayers( players );

        ref array<ref _ServerPlayer> updatedPlayers = new array<ref _ServerPlayer>();
        for ( int x = 0; x < players.Count(); x++ ) {
            updatedPlayers.Insert(new _ServerPlayerEx(players.Get(x)));
        }
        if(players.Count()) {
            ref _Payload_ServerPlayers payloadPlayers = new _Payload_ServerPlayers(isFirstReport, updatedPlayers);
            GetGameLabs().GetApi().ServerPlayers(new _Callback_ServerDummy(), payloadPlayers);
        }
    }
};