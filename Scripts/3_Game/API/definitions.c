class _Payload {
        string _v = GetGameLabs().GetVersionIdentifier();
        string ToJson() { return JsonFileLoader<_Payload>.JsonMakeData(this); }
};

class _Response {
        int status;
        string error;

        void _Response(string content) { JsonFileLoader<_Response>.JsonLoadData(content, this); }

        /*
         * void _Response(string content) {
            this._content = content;

            JsonSerializer serializer = new JsonSerializer(); // This allows for generic unpacking without overriding everything
            serializer.ReadFromString(this, this._content, this._jsonError);
            GetGameLabs().GetLogger().Debug(string.Format("Response: %1", this._content));
        }
         */
};

class _Callback : RestCallback {

};

// Register: /v1/auth/register
class _Payload_Register : _Payload {
    string serverId;
    string apiKey;

    void _Payload_Register(string serverId, string apiKey) { this.serverId = serverId; this.apiKey = apiKey; }
    string ToJson() { return JsonFileLoader<_Payload_Register>.JsonMakeData(this); }
};

class _Response_Register : _Response {
    string authKey;
    void _Response_Register(string content) { JsonFileLoader<_Response_Register>.JsonLoadData(content, this); }
};
// ************************

// Register: /v1/server/poll
class _Payload_ServerPoll : _Payload {
    float serverFps;

    void _Payload_ServerPoll(float serverFps) { this.serverFps = serverFps; }
    string ToJson() { return JsonFileLoader<_Payload_ServerPoll>.JsonMakeData(this); }
};
class ServerPollItem {
    string action;
    string target;
    string parameter;
};
class _Response_ServerPoll : _Response {
    array<ref ServerPollItem> actions;
    void _Response_ServerPoll(string content) { JsonFileLoader<_Response_ServerPoll>.JsonLoadData(content, this); }
};
// ************************