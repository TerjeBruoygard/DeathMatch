class CfgPatches
{
	class DeathMatch
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"JM_CF_Scripts"
		};
	};
};

class CfgMods
{
	class DeathMatch
	{
		dir = "DeathMatch";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "Death Match";
		credits = "Terje Bruoygard, Duck";
		version = "2.0.0";
		author = "Jacob_Mango";
		extra = 0;
		type = "mod";
		inputs = "DeathMatch/Inputs/inputs.xml";
		
		class defs
		{
			class gameScriptModule {
				value = "";
				files[] = {"DeathMatch/Scripts/3_Game"};
			};

			class worldScriptModule {
				value = "";
				files[] = {"DeathMatch/Scripts/4_World"};
			};

			class missionScriptModule {
				value = "";
				files[] = {"DeathMatch/Scripts/5_Mission"};
			};
            
            class imageSets
            {
                files[]=
                {
                    "DeathMatch/Textures/dm_gui.imageset"
                };
            };
		};
	};
};
