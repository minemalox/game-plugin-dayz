

modded class PlayerBase extends ManBase {
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
        if(!this.IsAlive()) return;

        ref _Payload_PlayerDamage payload;
        ref _LogPlayerEx logObjectMurderer;
        ref _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        PlayerBase murderer;
        if(source) {
            murderer = PlayerBase.Cast(source.GetHierarchyParent());
        }
        if(!source || !murderer) return;

        logObjectMurderer = new _LogPlayerEx(murderer);
        payload = new _Payload_PlayerDamage(logObjectPlayer, logObjectMurderer, source, damageResult.GetDamage(dmgZone, "Health"), dmgZone);
        GetGameLabs().GetApi().PlayerDamage(new _Callback(), payload);
    };
};