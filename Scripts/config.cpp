class CfgPatches
{
    class GameLabs_Scripts
    {
        units[]={};
        weapons[]={};
        requiredVersion=0.1;
        requiredAddons[]=
        {
            "DZ_Data",
            "DZ_Characters",
            "DZ_Characters_Tops"
        };
    };
};

class CfgMods
{
	class GameLabs
	{
		dir = "GameLabs";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "CFTools GameLabs";
		credits = "CFTools Software UG (haftungsbeschraenkt)";
		creditsJson = "GameLabs/Scripts/Data/Credits.json";
		author = "CFTools Software UG (haftungsbeschraenkt)";
		authorID = "0"; 
		version = "0.1"; 
		extra = 0;
		type = "mod";
		
		dependencies[] = { "Game", "World", "Mission" };
		
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"GameLabs/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"GameLabs/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"GameLabs/Scripts/5_Mission"};
			};
		};
	};
};

class CfgVehicles
{
	class Clothing;
	class TShirt_ColorBase;
	class CFToolsShirt: TShirt_ColorBase
	{
		scope=2;
		displayName="Black Shirt (CFTools edition)";
		descriptionShort="Black T-Shirt - Enhanced with a tactically placed CFTools logo in bright pink (thx Baroness)";
		hiddenSelectionsTextures[]=
		{
			"GameLabs\Scripts\Assets\shirtcftools.paa",
			"GameLabs\Scripts\Assets\shirtcftools.paa",
			"GameLabs\Scripts\Assets\shirtcftools.paa"
		};
	};
};