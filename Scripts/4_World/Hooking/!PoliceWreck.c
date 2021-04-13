class Land_Wreck_hb01_aban1_police extends House {
    ref _Event _registeredInstance = new _Event(this.GetType(), this);

    override void EEInit() {
        super.EEInit();
        vector position = GetPosition();
        if(position[0] <= 0 && position[1] <= 0 && position[2] <= 0) return;
        GetGameLabs().RegisterEvent(this._registeredInstance);
    }
    override void EEDelete(EntityAI parent) {
        super.EEDelete(parent);
        GetGameLabs().RemoveEvent(this._registeredInstance);
    }
};
