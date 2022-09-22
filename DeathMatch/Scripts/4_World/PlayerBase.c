modded class PlayerBase
{
	// Client vars
	float m_DmZoneRadiusLast = -1;
	
	// Synch vars
	bool m_DmPlayerReady = false;
	bool m_DmIsVarsSynch = false;
	float m_DmCenterX;
	float m_DmCenterZ;
	float m_DmZoneRadius;
	int m_DmHealthValue = 100;
	ref DmPlayerData m_dmPlayerData = null;
	ref DmConnectSyncContext m_dmConnectSyncCtx = null;
	
	// Server vars
	bool m_DmSynchDirty = false;
	bool m_DmPlayerDataSynchDirty = false;
	float m_DmSynchTimer = 1000;
	float m_DmPlayerDataSynchTimer = 1000;
	float m_RefillMagsTimer = 0;
	EntityAI m_lastDamageSource = NULL;
	ref DM_ServerSettings m_dmServerSettings = null;
	
	void DM_PSynchStat(ParamsReadContext ctx)
	{
		ref Param4<float, float, float, int> dmContext = new Param4<float, float, float, int>(0, 0, 0, 0);
		ctx.Read( dmContext );
		m_DmIsVarsSynch = true;
		m_DmCenterX = dmContext.param1;
		m_DmCenterZ = dmContext.param2;
		m_DmZoneRadius = dmContext.param3;
		m_DmHealthValue = dmContext.param4;
	}
	
	void DM_PSynchData(ParamsReadContext ctx)
	{
		ref Param1<ref DmPlayerData> dmPlayerDataContext = new Param1<ref DmPlayerData>(null);
		ctx.Read( dmPlayerDataContext );
		m_dmPlayerData = dmPlayerDataContext.param1;
	}
	
	void DM_PSynchInfo(ParamsReadContext ctx)
	{
		ref Param1<ref DmConnectSyncContext> dmConnSyncCtx = new Param1<ref DmConnectSyncContext>(null);
		ctx.Read( dmConnSyncCtx );
		m_dmConnectSyncCtx = dmConnSyncCtx.param1;
	}
	
	void SynchDmDirty()
	{
		m_DmSynchDirty = true;
	}
	
	void SynchDmPlayerDataDirty()
	{
		m_DmPlayerDataSynchDirty = true;
	}
	
	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);
		
		if (m_DmPlayerReady && GetGame().IsServer() && IsAlive())
		{
			DM_RefillStats(deltaTime);
			DM_RefillMags(deltaTime);
			DM_Synch(deltaTime);
		}
		
		if (!m_DmPlayerReady && GetGame().IsClient() && IsAlive())
		{
			GetRPCManager().SendRPC("DM", "DM_PReady", new Param1<int>(0), true);
			m_DmPlayerReady = true;
		}
	}
	
    override bool CanDropEntity (notnull EntityAI item)
	{
		return false;
	}
	
	override bool PhysicalPredictiveDropItem(EntityAI entity, bool heavy_item_only = true)
	{
		return false;
	}
	
	void DM_OnDead(PlayerBase killer)
	{
		if (GetGame().IsServer() && m_dmPlayerData && killer.m_dmPlayerData && m_dmServerSettings)
		{
			m_dmPlayerData.m_Death = m_dmPlayerData.m_Death + 1;
			
			float killerLevel = (float)killer.m_dmPlayerData.m_Level;
			m_dmPlayerData.m_Exp = m_dmPlayerData.m_Exp - (m_dmServerSettings.m_expDecPerDeath / killerLevel);
			while (m_dmPlayerData.m_Exp < 0)
			{
				if (m_dmPlayerData.m_Level == 1)
				{
					m_dmPlayerData.m_Exp = 0;
					break;
				}
				
				m_dmPlayerData.m_Level = m_dmPlayerData.m_Level - 1;
				m_dmPlayerData.m_Exp = m_dmPlayerData.m_Exp + m_dmPlayerData.m_Level;
			}
			
			SynchDmPlayerDataDirty();
		}
	}
	
	void DM_OnKill(PlayerBase murder)
	{
		if (GetGame().IsServer() && m_dmPlayerData && murder.m_dmPlayerData && m_dmServerSettings)
		{
			m_dmPlayerData.m_Kills = m_dmPlayerData.m_Kills + 1;
			m_dmPlayerData.m_Money = m_dmPlayerData.m_Money + m_dmServerSettings.m_moneyPerKill;
			
			float murderLevel = (float)murder.m_dmPlayerData.m_Level;
			m_dmPlayerData.m_Exp = m_dmPlayerData.m_Exp + (murderLevel * m_dmServerSettings.m_expIncPerKill);
			while (m_dmPlayerData.m_Exp > m_dmPlayerData.m_Level)
			{
				m_dmPlayerData.m_Exp = m_dmPlayerData.m_Exp - m_dmPlayerData.m_Level;
				m_dmPlayerData.m_Level = m_dmPlayerData.m_Level + 1;
			}
			
			SynchDmPlayerDataDirty();
		}
	}
	
	override void EEKilled( Object killer )
	{
		PlayerBase killerPlayer = PlayerBase.Cast(killer);
		if (killerPlayer != null && killerPlayer != this)
		{
			this.DM_OnDead(killerPlayer);
			killerPlayer.DM_OnKill(this);
		}
		else if (m_lastDamageSource != null)
		{
			killerPlayer = PlayerBase.Cast(m_lastDamageSource);
			if (killerPlayer != null && killerPlayer != this)
			{
				this.DM_OnDead(killerPlayer);
				killerPlayer.DM_OnKill(this);
			}
		}
		
		
		super.EEKilled( killer );
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 1000, false, this);
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (IsAlive() && this != source)
		{
			m_lastDamageSource = source;
		}
		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	void DM_RefillStats(float deltaTime)
	{
		if (GetStatWater().Get() < GetStatWater().GetMax() - 100)
		{
			GetStatWater().Set(GetStatWater().GetMax());
		}
		
		if (GetStatEnergy().Get() < GetStatEnergy().GetMax() - 100)
		{
			GetStatEnergy().Set(GetStatEnergy().GetMax());
		}
		
		float blood = GetHealth("GlobalHealth", "Blood");
		float maxBlood = GetMaxHealth("GlobalHealth", "Blood");
		if (blood < maxBlood - 100)
		{
			SetHealth("GlobalHealth", "Blood", maxBlood);
		}
		
		float shock = GetHealth("GlobalHealth", "Shock");
		float maxShock = GetMaxHealth("GlobalHealth", "Shock");
		if (shock < maxShock - 10)
		{
			SetHealth("GlobalHealth", "Shock", maxShock);
		}
		
		if (GetBleedingSourceCount() > 0 && m_BleedingManagerServer)
		{
			m_BleedingManagerServer.RemoveAllSources();
		}
		
		if( m_AgentPool && m_AgentPool.GetAgents() > 0 )
		{
			m_AgentPool.RemoveAllAgents();
		}
		
		if (GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
		{
			SetBrokenLegs(eBrokenLegs.NO_BROKEN_LEGS);
		}
		
		float dmDistInZone = DM_GetDistanceInZone();
		if (dmDistInZone > 0)
		{
			DecreaseHealth("GlobalHealth", "Health", deltaTime * dmDistInZone * m_dmServerSettings.m_healthOutOfZoneDmg);
		}
		else
		{
			AddHealth("GlobalHealth", "Health", deltaTime * m_dmServerSettings.m_healthRegen);
		}
		
		int currentHealth = (int)Math.Round(GetHealth("GlobalHealth", "Health"));
		if (currentHealth != m_DmHealthValue)
		{
			m_DmHealthValue = currentHealth;
			SynchDmDirty();
		}
	}
	
	void DM_RefillMags(float deltaTime)
	{
		m_RefillMagsTimer += deltaTime;
		if (m_RefillMagsTimer > 5)
		{
			m_RefillMagsTimer = 0;
			
			array<EntityAI> items();
			GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
			foreach (auto item : items)
			{
				Weapon wpn = Weapon.Cast(item);
				if (wpn && wpn.GetHealth01("", "") < 0.8)
				{
					wpn.SetHealth01("", "", 1);
					continue;
				}
				
				Magazine mag = Magazine.Cast(item);
				if (mag && !mag.GetInventory().IsAttachment())
				{
					int curAmmo = mag.GetAmmoCount();
					int maxAmmo = mag.GetAmmoMax();
					if (mag.GetAmmoCount() < mag.GetAmmoMax())
					{
						string magType = mag.GetType();
						GetGame().ObjectDelete(mag);
						GetInventory().CreateInInventory(magType);
					}
				}
			}
		}
	}
	
	float DM_GetDistanceInZone()
	{
		if (!m_DmIsVarsSynch)
		{
			return -1;
		}
		
		vector targetPos = GetWorldPosition();
		targetPos[1] = 0;
		float distance = vector.Distance(Vector(m_DmCenterX, 0, m_DmCenterZ), targetPos);
		return distance - m_DmZoneRadius;
	}
	
	void DM_Synch(float deltaTime)
	{
		m_DmSynchTimer = m_DmSynchTimer + deltaTime;
		m_DmPlayerDataSynchTimer = m_DmPlayerDataSynchTimer + deltaTime;
		
		if (m_DmSynchDirty && m_DmSynchTimer > 1.0)
		{
			m_DmSynchTimer = 0;
			m_DmSynchDirty = false;
			ref Param4<float, float, float, int> dmContext = new Param4<float, float, float, int>(m_DmCenterX, m_DmCenterZ, m_DmZoneRadius, m_DmHealthValue);
			GetRPCManager().SendRPC("DM", "DM_PSynchStat", dmContext, true, GetIdentity());
		}
		
		if (m_DmPlayerDataSynchDirty && m_DmPlayerDataSynchTimer > 1.0)
		{
			m_DmPlayerDataSynchTimer = 0;
			m_DmPlayerDataSynchDirty = false;
			ref Param1<ref DmPlayerData> dmPlayerDataContext = new Param1<ref DmPlayerData>(m_dmPlayerData);
			GetRPCManager().SendRPC("DM", "DM_PSynchData", dmPlayerDataContext, true, GetIdentity());
		}
	}
};