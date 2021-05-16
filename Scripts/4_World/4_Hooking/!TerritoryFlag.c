modded class TerritoryFlag {
    ref _Event _registeredInstance = new _Event("TerritoryFlag", "flag", this);

    void TerritoryFlag () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().RegisterEvent(this._registeredInstance);
    }

    void ~TerritoryFlag () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};