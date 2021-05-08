modded class ZombieBase  {
    private ref _AI _registeredInstance = new _AI(this, true);

    void ZombieBase () {
        if(!GetGame().IsServer()) return;
        GetGameLabs().IncrAICount();
        GetGameLabs().RegisterAI(this._registeredInstance);
    }

    void ~ZombieBase () {
        if(!GetGameLabs().IsServer()) return;
        if(!GetGameLabs()) return;
        GetGameLabs().DecrAICount();
        if(this._registeredInstance) GetGameLabs().RemoveAI(this._registeredInstance);
    }
};