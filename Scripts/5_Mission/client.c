class GameLabsClient {
    /*
     * TODO: PoC. This is inefficient beyond reasonable doubt, implement proper sync
     */

    ref Timer timerSync;

    void GameLabsReporter() {
        GetGameLabs().GetLogger().Info("+GameLabsReporter");
    }

    void StartSynchronization() {
        this.timerSync = new Timer(CALL_CATEGORY_SYSTEM);
        this.timerSync.Run(5, this, "clientSync", NULL, true);
    }

    private void clientSync() {
        GetGameLabs().RequestServerFPS();
    }
};