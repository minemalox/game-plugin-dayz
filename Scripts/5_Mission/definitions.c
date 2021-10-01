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

        GetGameLabs().GetLogger().Debug(string.Format("Player<%1> received CFTools Id from API cftools_id=%2, gamesession_id=%3", player, response.cftools_id, response.gamesession_id));
    };
};
// ************************

// /v1/player/disconnect
class _Callback_PlayerDisconnect : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("PlayerDisconnect errorCode(%1)", errorCode));
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("PlayerDisconnect timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        // TODO: Something here?
    };
};
// ************************

// Reporter internals

class _ServerPlayerEx : _ServerPlayer {
void _ServerPlayerEx(PlayerBase player) {
    if(player != NULL) {
        this.id = player.GetPlainId(); // Steam64
        this.name = player.GetPlayerName();

        this.position = player.GetPosition();
        this.health = player.GetHealth("GlobalHealth", "Health");
        if(GetGame().GetMission().IsPlayerDisconnecting(player))
            this.loggingOut = 1;

        if(player.IsInVehicle())
            this.insideVehicle = 1;
    }

    if(player.GetItemInHands())
        this.item = player.GetItemInHands().GetType();
};
};

class _Callback_ServerDummy : _Callback {};

// Poll Protocols

// v1
class _Callback_ServerPoll : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll errorCode(%1)", errorCode));
        if(errorCode == 6 || errorCode == 5) {
            // Server side error, only thrown when either offline or authentication error
            GetGameLabs().GetLogger().Error(string.Format("GameLabs API error, reauthenticating..."));
            GetGameLabs().GetApi().RegisterAsync();
        }
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_ServerPoll response = new _Response_ServerPoll(data);

        int q;
        Man man;
        vector position;
        PlayerBase player;
        ServerPollItem order;

        for ( int i = 0; i < response.orders.Count(); i++ ) {
            order = response.orders.Get(i);
            // TODO: Add new abstraction layer in 4_World
            if(order.action == "teleport") { //GetGameLabs()._TeleportPlayer(order.target, order.x, order.y);
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Teleporting %1 to %2, %3", order.target, order.x, order.y));
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);

                    position[0] = order.x;
                    position[1] = GetGame().SurfaceY(order.x, order.y) + 0.2;
                    position[2] = order.y;
                    player.SetPositionEx(position);
                }
            } else if(order.action == "heal") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Healing %1", order.target));
                // GetGameLabs()._KillPlayer(order.target);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);

                    player.SetHealth(player.GetMaxHealth("", ""));
                    player.SetHealth("", "Blood", player.GetMaxHealth("", "Blood"));
                    player.GetStatHeatComfort().Set(player.GetStatHeatComfort().GetMax());
                    player.GetStatTremor().Set(player.GetStatTremor().GetMin());
                    player.GetStatWet().Set(player.GetStatWet().GetMin());
                    player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
                    player.GetStatWater().Set(player.GetStatWater().GetMax());
                    player.GetStatDiet().Set(player.GetStatDiet().GetMax());
                    player.GetStatSpecialty().Set(player.GetStatSpecialty().GetMax());
                    player.SetBleedingBits(0);
                }
            }
            else if(order.action == "kill") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Killing %1", order.target));
                // GetGameLabs()._HealPlayer(order.target);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);
                    player.SetHealth(0);
                }
            }
            else if(order.action == "spawn") {
                GetGameLabs().GetLogger().Warn(string.Format("[Order] Spawning %1 for %2 (x%3)", order.parameter, order.target, order.quantity));
                // GetGameLabs()._SpawnItemForPlayer(order.target, order.item);
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);
                    if(order.quantity <= 1) player.SpawnEntityOnGroundPos(order.parameter, player.GetPosition());
                    else {
                        for (q = 1; q <= order.quantity; q++) player.SpawnEntityOnGroundPos(order.parameter, player.GetPosition());
                    }
                }
            }
            else if(order.action == "spawnat") {
                position[0] = order.x;
                position[1] = GetGame().SurfaceY(order.x, order.y) + 0.1;
                position[2] = order.y;

                GetGameLabs().GetLogger().Warn(string.Format("[Order] Spawning %1 at %2 (x%3)", order.parameter, position, order.quantity));

                if(order.quantity <= 1) GetGame().CreateObjectEx(order.parameter, position, ECE_PLACE_ON_SURFACE);
                else {
                    for (q = 1; q <= order.quantity; q++) GetGame().CreateObjectEx(order.parameter, position, ECE_PLACE_ON_SURFACE);
                }
            }
        }
    };
};

// v2
class _SP2Position {
    float x;
    float y;
    float z;
};
class _SP2OrderParams {
    int flags;
    int type;
    int quantity = 1;

    string class;
    string entityId;

    array<float> overcast;
    array<float> fog;
    array<float> rain;
    float wind;

    _SP2Position position;
};
class ServerPoll2Item {
        int action;
        string target;
        _SP2OrderParams params;
};
class _Response_ServerPoll2 : _Response {
    ref array<ServerPoll2Item> orders;
    void _Response_ServerPoll2(string content) { JsonFileLoader<_Response_ServerPoll2>.JsonLoadData(content, this); }
};

class _Callback_ServerPoll2 : _Callback {
    override void OnError(int errorCode) {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll2 errorCode(%1)", errorCode));
        if(errorCode == 6 || errorCode == 5) {
            // Server side error, only thrown when either offline or authentication error
            GetGameLabs().GetLogger().Error(string.Format("GameLabs API error, reauthenticating..."));
            GetGameLabs().GetApi().RegisterAsync();
        }
    };

    override void OnTimeout() {
        GetGameLabs().GetLogger().Error(string.Format("ServerPoll2 timed out"));
    };

    override void OnSuccess(string data, int dataSize) {
        _Response_ServerPoll2 response = new _Response_ServerPoll2(data);

        Man man;
        PlayerBase player;

        vector position;
        ServerPoll2Item order;

        bool orderStatus;
        bool requiresPlayer = false;

        for ( int i = 0; i < response.orders.Count(); i++ ) {
            order = response.orders.Get(i);
            GetGameLabs().GetLogger().Debug(string.Format("Processing order %1/%2 action=%3", i+1, response.orders.Count(), order.action));

            position[0] = order.params.position.x;
            if(order.params.position.z == NULL) {
                position[1] = GetGame().SurfaceY(order.x, order.y) + 0.2;
            } else {
                position[1] = order.params.position.z;
            }
            position[2] = order.params.position.y;

            if(order.target) {
                requiresPlayer = true;
                man = GetPlayerBySteam64(order.target);
                if(man != NULL) {
                    player = PlayerBase.Cast(man);
                } else {
                    player = NULL;
                }
            } else {
                requiresPlayer = false;
                player = NULL;
            }

            if(requiresPlayer && !player) {
                GetGameLabs().GetLogger().Info(string.Format("Order %1/%2 [action=%3] skipped, no player found with identity=%4", i+1, response.orders.Count(), order.action, order.target));
            } else {
                orderStatus = APIOrderHandler(order.action, player, position, order.params);
                GetGameLabs().GetLogger().Debug(string.Format("Order %1/%2 [action=%3] status=%4", i+1, response.orders.Count(), order.action, orderStatus));
                if(!orderStatus) {
                    GetGameLabs().GetLogger().Warning(string.Format("Order %1/%2 [action=%3] failed", i+1, response.orders.Count(), order.action));
                }
            }
        }
    };
};