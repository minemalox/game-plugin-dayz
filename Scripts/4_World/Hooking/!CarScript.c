modded class CarScript {
    private ref _Vehicle _registeredInstance = new _Vehicle(this);
    void CarScript() {
        GetGameLabs().RegisterVehicle(this._registeredInstance);
        GetGameLabs().IncrVehicleCount();
    }

    void ~CarScript() {
        if(this._registeredInstance) GetGameLabs().RemoveVehicle(this._registeredInstance);
        GetGameLabs().DecrVehicleCount();
    }
};