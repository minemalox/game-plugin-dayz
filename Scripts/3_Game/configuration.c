class GameLabsConfiguration {
    private bool debugEnabled = false;
    private bool connectionVerification = true;

    private string serverId = "";
    private string apiKey = "";

    private string baseUrl = "https://api.cftools.cloud/gamelabs/dz";
    private string storeUrl = "https://api.cftools.cloud/gamelabs/dz";

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
    string GetBaseURL() { return this.baseUrl; }
    string GetStoreURL() { return this.storeUrl; }
    bool GetConnectionVerificationStatus() { return this.connectionVerification; }
    bool GetDebugStatus() { return this.debugEnabled; }
    void OverrideDebugStatus(bool debugEnabled) { this.debugEnabled = debugEnabled; }
};