class GameLabsConfiguration {
    bool connectionVerification = true;
    string serverId;
    string apiKey;

    void GameLabsConfiguration() {
        // TODO: Create
    }



        this.LoadConfiguration()
            JsonFileLoader<CannabisPlusConfigManager>.JsonLoadFile(m_CPConfigPath, settings);
    }
};