modded class ZombieBase  {
    private ref _AI _registeredInstance = new _AI(this, true);

    void ZombieBase () {
        GetGameLabs().IncrAICount();
        GetGameLabs().RegisterAI(this._registeredInstance);
    }

    void ~ZombieBase () {
        GetGameLabs().DecrAICount();
        GetGameLabs().RemoveAI(this._registeredInstance);
    }
};