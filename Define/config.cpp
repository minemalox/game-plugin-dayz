class CfgPatches
{
    class Define
    {
        requiredVersion = 0.1;
        requiredAddons[] = {};
    };
};

class CfgMods
{
    class Define
    {
        dir="Define";
        name="GameLabs";
        type="mod";
        dependencies[]={"GameLib", "Game", "World", "Mission"};
        class defs
        {
            class gameLibScriptModule
            {
                value = "";
                files[] = {
                    "Define/Global"
                };
            };
            class gameScriptModule
            {
                value = "";
                files[]={
                    "Define/Global"
                };
            };

            class worldScriptModule
            {
                value="";
                files[]={
                    "Define/Global"
                };
            };

            class missionScriptModule
            {
                value="";
                files[]={
                    "Define/Global"
                };
            };
        };
    };
};