class GameLabsRPC {
    void GameLabsRPC() {
        GetDayZGame().Event_OnRPC.Insert( OnRPC );
    }

    void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        if(GetGame().IsServer()) HandleServerRPC(sender, target, rpc_type, ctx);
        else HandleClientRPC(sender, target, rpc_type, ctx);
    }

    /*
     * TODO: PoC. This is inefficient beyond reasonable doubt, implement proper sync
     */

    private void HandleClientRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        switch(rpc_type) {
            case GameLabsRPCS.RE_SERVERFPS: {
                ref Param1<float> response = new Param1<float>(-1.0);
                ctx.Read(response);

                float serverFps = response.param1;
                GetGameLabs().SetServerFPS(serverFps);
                break;
            }
        }
    }

    private void HandleServerRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
        switch(rpc_type) {
            case GameLabsRPCS.RQ_SERVERFPS: {
                ref Param1<float> payload = new Param1<float>(GetGameLabs().GetServerFPS());
                GetGame().RPCSingleParam(null, GameLabsRPCS.RE_SERVERFPS, payload, true, sender);
                break;
            }
        }
    }
};