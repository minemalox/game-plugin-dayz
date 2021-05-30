// /v1/player/connect
class _Callback_PlayerConnect : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("PlayerConnect errorCode(%1)", errorCode));
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("PlayerConnect timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_PlayerConnect response = new _Response_PlayerConnect(data);
        GetGameLabs().SetPlayerGamesessionId(response.steam64, response.gamesession_id);
        GetGameLabs().SetPlayerUpstreamIdentity(response.steam64, response.cftools_id);

        PlayerBase player = GetPlayerBySteam64(response.steam64);
        player.SetUpstreamIdentity(response.cftools_id);
        player.SetGamesessionId(response.gamesession_id);
        player.OnUpstreamIdentityReceived();

        GetGameLabs().GetLogger().Debug(string.Format("Player<%1> received CFTools Id from API cftools_id=%2", player, response.cftools_id));
    };
};
// ************************
