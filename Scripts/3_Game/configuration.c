class GameLabsConfiguration {
    private bool debugEnabled = false;
    private bool connectionVerification = true;

    private string serverId = "";
    private string apiKey = "";

    private string baseUrl = "https://api.cftools.cloud/gamelabs/dz";
    private string storeUrl = "https://api.cftools.cloud/gamelabs/dz";

    private bool speedCheckEnabled = true;
    private float speedCheckThresholdFoot = 8.0;
    private float speedCheckThresholdVehicle = 100.0;

    private float playerTickInterval = 2.5;

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
        this._ValidateAndExtend();
    }

    float GetPlayerTickInterval() { return this.playerTickInterval; }

    bool GetSpeedCheckStatus() { return this.speedCheckEnabled; }
    float GetSpeedCheckThresholdFoot() { return this.speedCheckThresholdFoot; }
    float GetSpeedCheckThresholdVehicle() { return this.speedCheckThresholdVehicle; }

    string GetServerId() { return this.serverId; }
    string GetApiKey() { return this.apiKey; }
    string GetBaseURL() { return this.baseUrl; }
    string GetStoreURL() { return this.storeUrl; }
    bool GetConnectionVerificationStatus() { return this.connectionVerification; }
    bool GetDebugStatus() { return this.debugEnabled; }
    void OverrideDebugStatus(bool debugEnabled) { this.debugEnabled = debugEnabled; }

    void _ValidateAndExtend() {
        if(this.speedCheckEnabled == NULL) this.speedCheckEnabled = true;
        if(this.speedCheckThresholdFoot == 1) this.speedCheckThresholdFoot = 8.0;
        if(this.speedCheckThresholdVehicle == 1) this.speedCheckThresholdVehicle = 100.0;
    }
};