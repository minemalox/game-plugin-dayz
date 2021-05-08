modded class CarScript {
    private ref _Vehicle _registeredInstance = new _Vehicle(this);
    void CarScript() {
        if(!GetGame().IsServer()) return;
        GetGameLabs().RegisterVehicle(this._registeredInstance);
        GetGameLabs().IncrVehicleCount();
    }

    void ~CarScript() {
        if(!GetGameLabs().IsServer()) return;
        if(!GetGameLabs()) return;
        if(this._registeredInstance) GetGameLabs().RemoveVehicle(this._registeredInstance);
        GetGameLabs().DecrVehicleCount();
    }
};