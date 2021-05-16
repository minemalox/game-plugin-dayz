class CfgPatches
{
    class GameLabs_Scripts
    {
        units[]={};
        weapons[]={};
        requiredVersion=0.1;
        requiredAddons[]=
        {
            "Define",
            "DZ_Data",
            "DZ_Characters",
            "DZ_Characters_Tops",
            "DZ_Characters_Headgear",
            "DZ_Characters_Heads",

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
		name = "GameLabs";
		credits = "GameLabs";
		creditsJson = "GameLabs/Scripts/Data/Credits.json";
		author = "GameLabs by CFTools";
		authorID = "0"; 
		version = "1.0";
		extra = 0;
		type = "mod";
		
		dependencies[] = { "Core", "Game", "World", "Mission" };
		
		class defs
		{
            class coreScriptModule
            {
                value = "";
                files[] = {"GameLabs/Scripts/1_Core"};
            };
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
    class Inventory_Base;
    class Container_Base;
    class Clothing_Base;
    class Clothing: Clothing_Base {};
    class Switchable_Base;

	class TShirt_ColorBase;
	class TShirt_CFTools: TShirt_ColorBase
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

    class Hoodie_ColorBase;
    class Hoodie_CFTools: Hoodie_ColorBase
    {
        scope=2;
        visibilityModifier=0.94999999;
        displayName="Blue Hoodie (CFTools edition)";
        descriptionShort="Blue Hoodie - Enhanced with a tactically placed CFTools logo";
        hiddenSelectionsTextures[]=
        {
            "GameLabs\Scripts\Assets\hoodie_grd_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\hoodie_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\hoodie_f_cft_blue_co.paa"
        };
    };

    class MilitaryBeret_ColorBase;
    class MilitaryBeret_Red;
    class MilitaryBeret_CFTools: MilitaryBeret_Red
    {
        scope=2;
        model="\DZ\characters\headgear\MilitaryBeret_g.p3d";
        displayName="Military Beret (CFTools edition)";
        descriptionShort="Blue Beret - Enhanced with a tactically placed CFTools logo";
        class ClothingTypes
        {
            male="\DZ\characters\headgear\MilitaryBeret_m.p3d";
            female="\DZ\characters\headgear\MilitaryBeret_f.p3d";
        };
        hiddenSelectionsTextures[]=
        {
            "GameLabs\Scripts\Assets\MilitaryBeret_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\MilitaryBeret_cft_blue_co.paa",
            "GameLabs\Scripts\Assets\MilitaryBeret_cft_blue_co.paa"
        };
        hiddenSelectionsMaterials[]=
        {
            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat",
            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat",
            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat"
        };
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints=100;
                    healthLevels[]=
                    {

                        {
                            1,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat"
                                    }
                        },

                        {
                            0.69999999,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret.rvmat"
                                    }
                        },

                        {
                            0.5,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret_damage.rvmat"
                                    }
                        },

                        {
                            0.30000001,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret_damage.rvmat"
                                    }
                        },

                        {
                            0,

                                    {
                                            "GameLabs\Scripts\Assets\MilitaryBeret_destruct.rvmat"
                                    }
                        }
                    };
                };
            };
        };
        class AnimEvents
        {
            class SoundWeapon
            {
                class pickUpItem
                {
                    soundSet="Shirt_pickup_SoundSet";
                    id=797;
                };
                class drop
                {
                    soundset="Shirt_drop_SoundSet";
                    id=898;
                };
            };
        };
    };
};