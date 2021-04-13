modded class AnimalBase  {
    private ref _AI _registeredInstance = new _AI(this, false);

    void AnimalBase () {
        GetGameLabs().IncrAnimalCount();
        GetGameLabs().RegisterAI(this._registeredInstance);
    }

    void ~AnimalBase () {
        GetGameLabs().DecrAnimalCount();
        GetGameLabs().RemoveAI(this._registeredInstance);
    }
};