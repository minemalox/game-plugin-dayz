#define GAMELABS

class CfgPatches
{
    class GameLabsDefine
    {
        requiredVersion = 0.1;
        requiredAddons[] = {};
    };
};

class CfgMods
{
    class GameLabsDefine
    {
        dir="GameLabsDefine";
        name="GameLabs";
        type="mod";
        dependencies[]={"GameLib", "Game", "World", "Mission"};
        class defs
        {
            class gameLibScriptModule
            {
                value = "";
                files[] = {
                    "GameLabs/GameLabsDefine/Global"
                };
            };
            class gameScriptModule
            {
                value = "";
                files[]={
                    "GameLabs/GameLabsDefine/Global"
                };
            };

            class worldScriptModule
            {
                value="";
                files[]={
                    "GameLabs/GameLabsDefine/Global"
                };
            };

            class missionScriptModule
            {
                value="";
                files[]={
                    "GameLabs/GameLabsDefine/Global"
                };
            };
        };
    };
};
