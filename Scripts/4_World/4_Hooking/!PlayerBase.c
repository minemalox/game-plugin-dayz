modded class PlayerBase extends ManBase {
    private string gl_steam64 = "";
    private string gl_name = "Survivor";

    string GetPlainId() {
        return this.gl_steam64;
    }

    string GetPlayerName() {
        return this.gl_name;
    }

    void GameLabs_OnConnect(string steam64, string name) {
        GetGameLabs().GetLogger().Debug(string.Format("GameLabs_OnConnect(this=%1, steam64=%2, name=%3)", this, steam64, name));
        this.gl_steam64 = string.Format("%1", steam64);
        this.gl_name = string.Format("%1", name);
    }

    void GameLabs_MakeReady(string steam64, string name) {
        GetGameLabs().GetLogger().Debug(string.Format("GameLabs_MakeReady(this=%1, steam64=%2, name=%3)", this, steam64, name));
        this.gl_steam64 = string.Format("%1", steam64);
        this.gl_name = string.Format("%1", name);
    }

    override void EEKilled(Object killer) {
        super.EEKilled(killer);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs().IsStatReportingEnabled()) return;

        ref _Payload_PlayerDeath payload;
        ref _LogPlayerEx logObjectMurderer;
        ref _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        EntityAI weapon;

        PlayerBase murderer;
        if(killer.IsWeapon() || killer.IsMeleeWeapon()) {
            weapon = EntityAI.Cast(killer);
            murderer = PlayerBase.Cast(weapon.GetHierarchyParent());
        }
        GetGameLabs().GetLogger().Debug(string.Format("EEKilled(this=%1, killer=%2, weapon=%3, murderer=%4)", this, killer, weapon, murderer));

        if(murderer) {
            logObjectMurderer = new _LogPlayerEx(murderer);
            payload = new _Payload_PlayerDeath(logObjectPlayer, logObjectMurderer, killer.GetType(), killer.GetDisplayName());
        } else if(this == killer) { // Suicide, potentially Environmental death
            if(weapon) {
                payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, killer.GetType(), killer.GetDisplayName());
            } else if(this.CommitedSuicide() || this.CommitedSuicide()) {
                weapon = this.GetItemInHands();
                if(weapon) payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Suicide", weapon.GetType());
                else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Suicide", "");
            }
            else payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment", "");

        } else { // Infected
            payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Infected", "");
        }

        GetGameLabs().GetApi().PlayerDeath(new _Callback(), payload);
    };

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs().IsStatReportingEnabled()) return;

        DayZPlayerImplement playerImplement = DayZPlayerImplement.Cast(this);
        GetGameLabs().GetLogger().Debug(string.Format("EEHitBy.EVAL (deathSync=%1, suicide=%2)", this.m_DeathSyncSent, this.CommitedSuicide()));
        if(this.m_DeathSyncSent || this.CommitedSuicide()) return; // Prevent logging of hits for dead bodies

        ref _Payload_PlayerDamage payload;
        ref _LogPlayerEx logObjectMurderer;
        ref _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        PlayerBase murderer;
        if(source) {
            murderer = PlayerBase.Cast(source.GetHierarchyParent());
        }
        if(!source || !murderer) return;

        GetGameLabs().GetLogger().Debug(string.Format("EEHitBy(this=%1, murderer=%2, source=%3, component=%4, dmgZone=%5, ammo=%6, modelPos=%7, speedCoef=%8)", this, murderer, source, component, dmgZone, ammo, modelPos, speedCoef));
        logObjectMurderer = new _LogPlayerEx(murderer);
        payload = new _Payload_PlayerDamage(logObjectPlayer, logObjectMurderer, source, damageResult.GetDamage(dmgZone, "Health"), dmgZone);
        GetGameLabs().GetApi().PlayerDamage(new _Callback(), payload);
    };
};