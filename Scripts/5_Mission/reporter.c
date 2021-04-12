class _Callback_ServerPoll : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll errorCode(%1)", errorCode));
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_ServerPoll response = new _Response_ServerPoll(data);
        GetGameLabs().GetLogger().Debug(string.Format("ServerPoll OnSuccess(%1)", response));
    };
};

class GameLabsReporter {
    int lastSent = 0;

    ref Timer timerPoll;
    ref Timer timerServer;
    ref Timer timerPlayers;

    void GameLabsReporter() {
        this.timerPoll = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerPoll.Run(2, this, "activePolling", NULL, true);
    }

    private void activePolling() {
        ref _Payload_ServerPoll payload = new ref _Payload_ServerPoll(GetGameLabs().GetServerFPS());
        GetGameLabs().GetApi().ServerPoll(new _Callback_ServerPoll(), payload);
    }
};