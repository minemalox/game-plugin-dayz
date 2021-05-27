modded class CarScript {
    private ref _Vehicle _registeredInstance;
    void CarScript() {
        this._registeredInstance = new _Vehicle(this);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().RegisterVehicle(this._registeredInstance);
        GetGameLabs().IncrVehicleCount();
    }

    void ~CarScript() {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        if(this._registeredInstance) GetGameLabs().RemoveVehicle(this._registeredInstance);
        GetGameLabs().DecrVehicleCount();
    }
};