class CfgPatches
{
	class GunGame
	{
		units[] = {};
		weapons[] = {};
		//requiredVersion=0.1;
		requiredAddons[] =
		{
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class GunGame
	{
		type = "mod";

		name = "Gun Game";
		tooltip = "Gun Game for dayz";
		overview = "Custom gun game mode for dayz";
		action = "https://github.com/TheDimin";
		author = "TheDimin";
		version = "0.01";

		dependencies[] = { "Game", "World", "Mission" };

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "GunGame/Scripts/3_Game" };
			};

			class worldScriptModule
			{
				value = "";
				files[] = { "GunGame/Scripts/4_World" };
			};

			class missionScriptModule
			{
				value = "";
				files[] = { "GunGame/Scripts/5_Mission" };
			};
		};
	};
};


//class CfgMissions
//{
//	class MPMissions
//	{
//		class GunGame
//		{
//
//		}
//	};
//}