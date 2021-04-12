class GameLabsConfiguration {
    private bool debugEnabled = false;
    private bool connectionVerification = true;

    private string serverId = "";
    private string apiKey = "";

    [NonSerialized()]
    private const string path = "$profile:gamelabs.cfg";

    void GameLabsConfiguration() {
        // TODO: Ensure configuration structure
    }

    bool CheckDiskPresence() {
        return FileExist(this.path);
    }

    void LoadFromDisk() {
        JsonFileLoader<GameLabsConfiguration>.JsonLoadFile(this.path, this);
    }

    string GetServerId() { return this.serverId; }
    string GetApiKey() { return this.apiKey; }
    bool GetConnectionVerificationStatus() { return this.connectionVerification; }
    bool GetDebugStatus() { return this.debugEnabled; }
};