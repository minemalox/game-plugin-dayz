

modded class PlayerBase extends ManBase {
    override void EEKilled(Object killer) {
        super.EEKilled(killer);

        ref _Payload_PlayerDeath payload;
        ref _LogPlayerEx logObjectMurderer;
        ref _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        EntityAI weapon;

        PlayerBase murderer;
        if(killer.IsWeapon() || killer.IsMeleeWeapon()) {
            weapon = EntityAI.Cast(killer);
            murderer = PlayerBase.Cast(weapon.GetHierarchyParent());
        }

        if(murderer) {
            logObjectMurderer = new _LogPlayerEx(murderer);
            payload = new _Payload_PlayerDeath(logObjectPlayer, logObjectMurderer, killer.GetType());
        } else if(this == killer) { // Environmental death
            payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Environment");
        } else { // Infected
            payload = new _Payload_PlayerDeath(logObjectPlayer, NULL, "__Infected");
        }

        GetGameLabs().GetApi().PlayerDeath(new _Callback(), payload);
    };

    override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef) {
        super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

        ref _Payload_PlayerDamage payload;
        ref _LogPlayerEx logObjectMurderer;
        ref _LogPlayerEx logObjectPlayer = new _LogPlayerEx(this);

        PlayerBase murderer;
        if(source) {
            murderer = PlayerBase.Cast(source.GetHierarchyParent());
        }

        if(murderer) {
            logObjectMurderer = new _LogPlayerEx(murderer);
            payload = new _Payload_PlayerDamage(logObjectPlayer, logObjectMurderer, source, damageResult.GetDamage(dmgZone, "Health"), dmgZone);
        } else {
            payload = new _Payload_PlayerDamage(logObjectPlayer, NULL, source, damageResult.GetDamage(dmgZone, "Health"), dmgZone);
        }

        GetGameLabs().GetApi().PlayerDamage(new _Callback(), payload);
    };
};