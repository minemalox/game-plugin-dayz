modded class AnimalBase  {
    private ref _AI _registeredInstance = new _AI(this, false);

    void AnimalBase () {
        if(!GetGame().IsServer()) return;
        GetGameLabs().IncrAnimalCount();
        GetGameLabs().RegisterAI(this._registeredInstance);
    }

    void ~AnimalBase () {
        if(!GetGameLabs().IsServer()) return;
        if(!GetGameLabs()) return;
        GetGameLabs().DecrAnimalCount();
        if(this._registeredInstance) GetGameLabs().RemoveAI(this._registeredInstance);
    }
};