modded class AnimalBase  {
    private ref _AI _registeredInstance;

    void AnimalBase () {
        this._registeredInstance = new _AI(this, false);
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().IncrAnimalCount();
        GetGameLabs().RegisterAI(this._registeredInstance);
    }

    void ~AnimalBase () {
        if(!GetGameLabs()) return;
        if(!GetGameLabs().IsServer()) return;
        GetGameLabs().DecrAnimalCount();
        if(this._registeredInstance) GetGameLabs().RemoveAI(this._registeredInstance);
    }
};