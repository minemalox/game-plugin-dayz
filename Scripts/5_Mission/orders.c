enum GameLabsOrders {
    INVALID             = 0,
    PLAYER_TELEPORT     = 1,
    PLAYER_HEAL         = 2,
    PLAYER_KILL         = 3,
    PLAYER_STRIP        = 4,
    PLAYER_NUKE         = 5,
    PLAYER_SPAWN        = 6,
    SERVER_WEATHER      = 100,
    SERVER_TIME         = 101,
    WORLD_SPAWN         = 200,
};

enum OrderParamFlags {
    INVALID         = 0,
    DEBUG_SPAWN     = 1,
};

bool _ProcessTeleportPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Teleport] %1 position=%2", player, position));
    player.SetPositionEx(position);
    return true;
};

bool _ProcessHealPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Heal] %1", player));
    player.GLHealEx();
    return true;
};

bool _ProcessKillPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Heal] %1", player));
    player.SetHealth(0);
    return true;
};

bool _ProcessStripPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Strip] %1", player));
    player.RemoveAllItems();
    return true;
};

bool _ProcessNukePlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Nuke] %1", player));
    player.Explode(DT_EXPLOSION, "LandFuelFeed_Ammo");
    return true;
};

bool _ProcessSpawnPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) for %3 [flags=%4]", params.gameClass, params.quantity, player, params.flags));

    EntityAI entity;
    for(int i = 1; i <= params.quantity; i++) {
        entity = player.SpawnEntityOnGroundPos(params.gameClass, player.GetPosition());
        if(params.flags & OrderParamFlags.DEBUG_SPAWN) {
            entity.OnDebugSpawn();
        }
    }

    return true;
};

bool _ProcessWeatherServer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Weather] Updating weather overcast=%1, fog=%2, rain=%3, wind=%4", params.overcast, params.fog, params.rain, params.wind));
    Weather weather = GetGame().GetWeather();
    if(!weather) return false;

    if(params.overcast && weather.GetOvercast()) weather.GetOvercast().Set(params.overcast[0], params.overcast[1], params.overcast[2]);
    if(params.fog && weather.GetFog()) weather.GetFog().Set(params.fog[0], params.fog[1], params.fog[2]);
    if(params.rain && weather.GetRain()) weather.GetRain().Set(params.rain[0], params.rain[1], params.rain[2]);
    if(params.wind) weather.SetWindSpeed(params.wind);

    return true;
};

bool _ProcessTimeServer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Time] Updating game time hour=%1, minute=%2", params.hour, params.minute));

    int year, month, day, hour, minute;
    GetGame().GetWorld().GetDate(year, month, day, hour, minute);
    GetGame().GetWorld().SetDate(year, month, day, params.hour, params.minute);

    return true;
};

bool _ProcessSpawnWorld(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) at %3 [flags=%4]", params.gameClass, params.quantity, position, params.flags));

    EntityAI entity;
    for(int i = 1; i <= params.quantity; i++) {
        entity = GetGame().CreateObject(params.gameClass, position);
        if(params.flags & OrderParamFlags.DEBUG_SPAWN) {
            entity.OnDebugSpawn();
        }
    }

    return true;
};

// Override to add custom actions
// TODO: Create class construct for cleaner implementation; Call with enscript api
bool APIOrderOverride(int action, PlayerBase player, vector position, _SP2OrderParams params) {
    return true;
};

bool APIOrderHandler(int action, PlayerBase player, vector position, _SP2OrderParams params) {
    bool status;
    // Action id range 0-1000 is protected
    if(action > 1000) status = APIOrderOverride(action, player, position, params);
    else
        switch(action) {
            // player targeted
            case GameLabsOrders.PLAYER_TELEPORT: { status = _ProcessTeleportPlayer(player, position, params); break; }
            case GameLabsOrders.PLAYER_HEAL: { status = _ProcessHealPlayer(player, position, params); break; }
            case GameLabsOrders.PLAYER_KILL: { status = _ProcessKillPlayer(player, position, params); break; }
            case GameLabsOrders.PLAYER_STRIP: { status = _ProcessStripPlayer(player, position, params); break; }
            case GameLabsOrders.PLAYER_NUKE: { status = _ProcessNukePlayer(player, position, params); break; }
            case GameLabsOrders.PLAYER_SPAWN: { status = _ProcessSpawnPlayer(player, position, params); break; }

            // world targeted
            case GameLabsOrders.SERVER_WEATHER: { status = _ProcessWeatherServer(player, position, params); break; }
            case GameLabsOrders.SERVER_TIME: { status = _ProcessTimeServer(player, position, params); break; }
            case GameLabsOrders.WORLD_SPAWN: {status = _ProcessSpawnWorld(player, position, params); break; }

            default: {
                status = false;
                GetGameLabs().GetLogger().Warn(string.Format("action=%1 has no processor", action));
                break;
            }
        }
    return status;
};