modded class MissionServer {
    void MissionServer() {
        Print("[GameLabs] Loaded 123");
    }

    override void EquipCharacter(MenuDefaultCharacterData char_data) {
        PlayerIdentity identity = m_player.GetIdentity();

        if(identity.GetId() == "2XAdLQ7Sr0EYZXmMRn7pC8emM5z0JhHAMCvk4mQFuOE=") {
            Print("[GameLabs] Granting CFTools shirt on spawn");
            ItemBase item = m_player.GetItemInHands();
            if(item) {
                m_player.DropItem(item);
            }
            item = m_player.GetHumanInventory().CreateInHands("CFToolsShirt");
            m_player.DropItem(item);
        }

        super.EquipCharacter(char_data);
    }
};

modded class MissionGameplay {
    void MissionGameplay()
    {
        Print("[GameLabs] - Loaded Client Mission");
    }
};