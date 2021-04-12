/*
 * Since BI can not properly integrate full header support, but headers are encrypted, auth key resides in Content-Type
 */

class GameLabsAPI {
    private static const string baseUrl = "https://api.cftools.local/gamelabs/dayz";

    private ref RestApi restApi;
    private ref RestContext restContext;
    private ref JsonSerializer jsonSerializer = new JsonSerializer();

    private string serverId;
    private string apiKey;
    private string authKey;

    private bool active = false;

    void GameLabsAPI(string serverId, string apiKey) {
        this.serverId = serverId;
        this.apiKey = apiKey;

        this.restApi = CreateRestApi();
        this.restContext = this.restApi.GetRestContext(this.baseUrl);

        this.restApi.EnableDebug(false);
    }

    void Enable() { this.active = true; }
    void Disable() { this.active = false; }
    bool IsEnabled() { return this.active; }

    int Register() {
        this.restContext.SetHeader("application/json");

        ref _Payload_Register payload = new ref _Payload_Register(this.serverId, this.apiKey);
        _Response_Register response = new _Response_Register(this.restContext.POST_now("/v1/auth/register", payload.ToJson()));
        if(response.status == 2) {
            this.authKey = response.authKey;
            this.restContext.SetHeader(this.authKey);
            GetGameLabs().GetLogger().Debug(string.Format("authKey=%1", this.authKey));
        }
        return response.status;
    }

    int Verify() {
        if(!this.IsEnabled()) return -1;

        ref _Payload payload = new ref _Payload();
        _Response response = new _Response(this.restContext.POST_now("/v1/auth/verify", payload.ToJson()));
        return response.status;
    }

    void ServerPoll(ref Managed cb, ref _Payload_ServerPoll payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/server/poll", payload.ToJson());
    }
};