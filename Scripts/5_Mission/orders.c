enum GameLabsOrders {
    INVALID = 0,
    PLAYER_TELEPORT,
    PLAYER_HEAL,
    PLAYER_KILL,
    PLAYER_STRIP,
    PLAYER_NUKE,
    PLAYER_SPAWN,
    SERVER_WEATHER,
    SERVER_TIME,
};

enum OrderParamFlags {
    DEBUG_SPAWN,
};

bool _ProcessTeleportPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Teleport] %1 position=%2", player, position));
    player.SetPositionEx(position);
    return true;
};

bool _ProcessHealPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Heal] %1", player));
    player.HealEx();
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
    GetGameLabs().GetLogger().Warn(string.Format("[Strip] %1", player));
    player.Explode(DT_EXPLOSION, "LandFuelFeed_Ammo");
    return true;
};

bool _ProcessSpawnPlayer(PlayerBase player, vector position, _SP2OrderParams params) {
    GetGameLabs().GetLogger().Warn(string.Format("[Spawn] Spawning %1 (x%2) for %3 [flags=%4]", params.class, params.quantity, player, params.flags));

    EntityAI entity;
    if(order.quantity <= 1) {
        entity = player.SpawnEntityOnGroundPos(params.class, player.GetPosition());
        if(params.flags & OrderParamFlags.DEBUG_SPAWN) {
            entity.OnDebugSpawn();
        }
    } else {
        for(int i = 1; i <= order.quantity; i++) {
            entity = player.SpawnEntityOnGroundPos(params.class, player.GetPosition());
            if(params.flags & OrderParamFlags.DEBUG_SPAWN) {
                entity.OnDebugSpawn();
            }
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

// Override to add custom actions
bool APIOrderHandler(int action, PlayerBase player, vector position, _SP2OrderParams params) {
    bool status;
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

        default: {
            status = false;
            GetGameLabs().GetLogger().Warning(string.Format("action=%1 has no processor", action));
            break;
        }
    }
    return status;
};