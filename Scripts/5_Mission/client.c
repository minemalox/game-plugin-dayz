class GameLabsClient {
    /*
     * TODO: PoC
     */

    private bool processReporting = true;
    ref Timer timerSync;

    void StartSynchronization() {
        this.timerSync = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerSync.Run(5, this, "clientSync", NULL, true);
    }

    void Disable() {
        this.processReporting = false;
        GetGameLabs().GetLogger().Info("(Client) Disabled");
        this.timerSync.Stop();
        GetGameLabs().GetLogger().Debug("(Client) Timers gracefully closed");
    }

    void SyncExpansionChat(ChatMessageEventParams params) {
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.SY_EXPANSIONCHAT, params, true);
    }

    private void clientSync() {
        if(!this.processReporting) return;
        GetGameLabs().RequestServerFPS();
    }
};