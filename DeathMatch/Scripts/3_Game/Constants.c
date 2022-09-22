const string DeathMatch_ModPreffix = "[DM] ";
const bool DeathMatch_TestingMode = false;

void DM_Log(string message)
{
	Print(DeathMatch_ModPreffix + message);
}

bool DM_HasBatterySlot(string classname)
{
	array<string> slots = new array<string>;
	string preffix = FindItemPreffix(classname);
	GetGame().ConfigGetTextArray( preffix + " " + classname + " attachments", slots);
	return slots && slots.Find("BatteryD") != -1;
}

string FindItemPreffix(string itemClassname)
{
	if (GetGame().ConfigIsExisting("CfgVehicles " + itemClassname)) return "CfgVehicles";
	if (GetGame().ConfigIsExisting("CfgMagazines " + itemClassname)) return "CfgMagazines";
	if (GetGame().ConfigIsExisting("CfgAmmo " + itemClassname)) return "CfgAmmo";
	if (GetGame().ConfigIsExisting("CfgWeapons " + itemClassname)) return "CfgWeapons";
	if (GetGame().ConfigIsExisting("CfgNonAIVehicles " + itemClassname)) return "CfgNonAIVehicles";
	return "";
}