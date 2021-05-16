modded class TerritoryFlag {
    ref _Event _registeredInstance = new _Event("TerritoryFlag", "pennant", this);

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

    // Public API
    _Event _GetEventInstance() {
        return this._registeredInstance;
    }

    void _SetEventInstance(ref _Event _registeredInstance) {
        this._registeredInstance = _registeredInstance;
    }
};