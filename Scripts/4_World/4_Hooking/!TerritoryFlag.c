modded class TerritoryFlag {
    ref _Event _registeredInstance = new _Event(this.GetType(), "flag-alt", this);

    void TerritoryFlag () {
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this._registeredInstance);
    }

    void ~TerritoryFlag () {
        if(!GetGame().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};