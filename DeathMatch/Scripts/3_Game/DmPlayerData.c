class DmPlayerData
{
	string m_Name;
	int m_Kills;
	int m_Death;
	int m_Money;
	int m_Level;
	float m_Exp;
	string m_Weapons;
	string m_CurrentWeapon;
	string m_Equipment;
	string m_CurrentEquipment;
	string m_Friends;
	string m_Skills;
	int m_TimeStamp;
	int m_LeaderBoardIndex = -1;
	
	void Init(PlayerIdentity identity, int money)
	{
		m_Name = identity.GetName();
		m_Kills = 0;
		m_Death = 0;
		m_Money = money;
		m_Level = 1;
		m_Exp = 0;
		m_Weapons = "";
		m_CurrentWeapon = "";
		m_Equipment = "";
		m_CurrentEquipment = "";
		m_Friends = "";
		m_Skills = "";
		m_TimeStamp = 0;
	}
	
	void AddWeapon(string id)
	{
		if (m_Weapons.Length() == 0)
		{
			m_Weapons = id;
		}
		else
		{
			m_Weapons = m_Weapons + ";" + id;
		}
	}
	
	bool ContainsWeapon(ref DmConnectSyncContext dmConnectSyncCtx, string id)
	{
		TStringArray resultList = new TStringArray;
		m_Weapons.Split(";", resultList);
		foreach (string w : resultList)
		{
			if (w == id)
			{
				ref DmWeaponPresset ww = dmConnectSyncCtx.FindWeaponPresset(id);
				if (ww && ww.m_Level <= m_Level)
				{
					return true;
				}
			}
		}
		
		foreach (ref DmWeaponPresset wp : dmConnectSyncCtx.dm_Weapons)
		{
			if (wp.m_Id == id && wp.m_Price == 0 && wp.m_Level <= m_Level)
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool IsWeaponLocked(ref DmWeaponPresset presset)
	{
		return presset.m_Level > m_Level || presset.m_Price > m_Money;
	}
	
	bool IsWeaponSelected(ref DmWeaponPresset presset)
	{
		return presset.m_Id == m_CurrentWeapon;
	}
	
	
	void AddEquipment(string id)
	{
		if (m_Equipment.Length() == 0)
		{
			m_Equipment = id;
		}
		else
		{
			m_Equipment = m_Equipment + ";" + id;
		}
	}
	
	bool ContainsEquipment(ref DmConnectSyncContext dmConnectSyncCtx, string id)
	{
		TStringArray resultList = new TStringArray;
		m_Equipment.Split(";", resultList);
		foreach (string w : resultList)
		{
			if (w == id)
			{
				ref DmEquipmentPresset ww = dmConnectSyncCtx.FindEquipmentPresset(id);
				if (ww && ww.m_Level <= m_Level)
				{
					return true;
				}
			}
		}
		
		foreach (ref DmEquipmentPresset wp : dmConnectSyncCtx.dm_Equipments)
		{
			if (wp.m_Id == id && wp.m_Price == 0 && wp.m_Level <= m_Level)
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool IsEquipmentLocked(ref DmEquipmentPresset presset)
	{
		return presset.m_Level > m_Level || presset.m_Price > m_Money;
	}
	
	bool IsEquipmentSelected(ref DmEquipmentPresset presset)
	{
		return presset.m_Id == m_CurrentEquipment;
	}
	
	
	void Serialize(Serializer ctx)
	{
		ctx.Write(m_Name);
		ctx.Write(m_Kills);
		ctx.Write(m_Death);
		ctx.Write(m_Money);
		ctx.Write(m_Level);
		ctx.Write(m_Exp);
		ctx.Write(m_Weapons);
		ctx.Write(m_CurrentWeapon);
		ctx.Write(m_Equipment);
		ctx.Write(m_CurrentEquipment);
		ctx.Write(m_Friends);
		ctx.Write(m_Skills);
		ctx.Write(m_TimeStamp);
	}
	
	void Deserialize(Serializer ctx, int version)
	{
		ctx.Read(m_Name);
		ctx.Read(m_Kills);
		ctx.Read(m_Death);
		ctx.Read(m_Money);
		ctx.Read(m_Level);
		ctx.Read(m_Exp);
		ctx.Read(m_Weapons);
		ctx.Read(m_CurrentWeapon);
		ctx.Read(m_Equipment);
		ctx.Read(m_CurrentEquipment);
		ctx.Read(m_Friends);
		ctx.Read(m_Skills);
		
		if (version >= 2)
		{
			ctx.Read(m_TimeStamp);
		}
		else
		{
			m_TimeStamp = 0;
		}
	}
};