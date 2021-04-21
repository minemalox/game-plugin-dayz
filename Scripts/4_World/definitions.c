class _LogPlayerEx : _LogPlayer {
    void _LogPlayerEx(PlayerBase player) {
        this.id = player.GetIdentity().GetPlainId();

        this.position = player.GetPosition();

        this.blood =  player.GetHealth("GlobalHealth", "Blood");
        this.health = player.GetHealth( "GlobalHealth","Health" );
        if(player.GetStatWater()) this.water = player.GetStatWater().Get();
        if(player.GetStatEnergy()) this.energy = player.GetStatEnergy().Get();
        if(player.GetBleedingManagerServer()) this.bleedingSources = player.GetBleedingManagerServer().GetBleedingSourcesCount();
    }
};