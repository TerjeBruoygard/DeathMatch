class DmConnectSyncContext
{
	ref array<ref DmWeaponPresset> dm_Weapons;
	ref array<ref DmEquipmentPresset> dm_Equipments;
	
	ref DmWeaponPresset FindWeaponPresset(string id)
	{
		foreach (ref DmWeaponPresset w : dm_Weapons)
		{
			if (w.m_Id == id)
			{
				return w;
			}
		}
		
		return null;
	}
	
	ref DmEquipmentPresset FindEquipmentPresset(string id)
	{
		foreach (ref DmEquipmentPresset e : dm_Equipments)
		{
			if (e.m_Id == id)
			{
				return e;
			}
		}
		
		return null;
	}
};