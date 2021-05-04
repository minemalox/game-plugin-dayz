class GameLabsClient {
    /*
     * TODO: PoC. This is inefficient beyond reasonable doubt, implement proper sync
     */

    ref Timer timerSync;

    void StartSynchronization() {
        this.timerSync = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerSync.Run(5, this, "clientSync", NULL, true);
    }

    void SyncExpansionChat(ChatMessageEventParams params) {
        GetGame().RPCSingleParam(NULL, GameLabsRPCS.SY_EXPANSIONCHAT, params, true);
        //GetGame().RPC(NULL, GameLabsRPCS.SY_EXPANSIONCHAT, params, true, NULL);
    }

    private void clientSync() {
        GetGameLabs().RequestServerFPS();
    }
};