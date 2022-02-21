class Saiga_CFTools extends Saiga_Base {
    override float GetChanceToJam() { return 0.0; }

    override bool CanPutInCargo(EntityAI parent) {
        PlayerBase player;
        if(parent.GetHierarchyRootPlayer() && PlayerBase.CastTo(player, parent.GetHierarchyRootPlayer())) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent) {
        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutAsAttachment(EntityAI parent) {
        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override void EEFired(int muzzleType, int mode, string ammoType) {
        super.EEFired(muzzleType, mode, ammoType);

        Magazine magazine;
        if(GetGame().IsServer()) {
            magazine = this.GetMagazine(this.GetCurrentMuzzle());
            if(GetGame().IsMultiplayer() && magazine) GetGame().RemoteObjectDelete(magazine);
            if(magazine) magazine.ServerSetAmmoMax();
            if(GetGame().IsMultiplayer() && magazine) GetGame().RemoteObjectCreate(magazine);
        } else {
            magazine = this.GetMagazine(this.GetCurrentMuzzle());
            if(magazine) magazine.LocalSetAmmoMax();
        }
    }

    override void OnDebugSpawn() {
        GameInventory inventory = GetInventory();
        inventory.CreateInInventory( "Saiga_Bttstck" );
        inventory.CreateInInventory( "KobraOptic" );
        inventory.CreateInInventory( "Battery9V" );

        SpawnAttachedMagazine("Mag_Saiga_Drum20Rnd");
    }
};