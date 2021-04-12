modded class DayZGame {
    private int gl_tps = 0;
    private int gl_ticks = 0;
    private int gl_tpsTime = 0;

    override void OnUpdate(bool doSim, float timeslice) {
        super.OnUpdate(doSim, timeslice);

        this.gl_ticks++;
        if(this.gl_tpsTime + 1 < GetGame().GetTickTime()) {
            this.gl_tpsTime = GetGame().GetTickTime();
            this.gl_tps = this.gl_ticks / 2;
            this.gl_ticks = 0;
            GetGameLabs().SetServerFPS(this.gl_tps); // GameLabsCore can not directly access this value
        }
    }
};