modded class PlayerBase
{
	// Client vars
	bool m_DmDataUpdated = false;
	float m_DmZoneRadiusLast = -1;
	float m_DmCenterXLast = 0;
	float m_DmCenterZLast = 0;
	
	// Synch vars
	bool m_DmIsVarsSynch = false;
	float m_DmCenterX;
	float m_DmCenterZ;
	float m_DmZoneRadius;
	int m_DmHealthValue = 100;
	ref DmPlayerData m_dmPlayerData = null;
	ref DmConnectSyncContext m_dmConnectSyncCtx = null;
	
	// Server vars
	bool m_DmPlayerReady = false;
	bool m_DmSynchDirty = false;
	bool m_DmPlayerDataSynchDirty = false;
	float m_DmSynchTimer = 1000;
	float m_DmPlayerDataSynchTimer = 1000;
	float m_RefillMagsTimer = 0;
	float m_DmHealthRegenTimeout = 0;
	float m_DmClearItemsTimer = 0;
	EntityAI m_lastDamageSource = NULL;
	ref DM_ServerSettings m_dmServerSettings = null;
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		if (rpc_type == 14880011)
		{
			ref Param4<float, float, float, int> dmContext = new Param4<float, float, float, int>(0, 0, 0, 0);
			ctx.Read( dmContext );
			m_DmIsVarsSynch = true;
			m_DmCenterX = dmContext.param1;
			m_DmCenterZ = dmContext.param2;
			m_DmZoneRadius = dmContext.param3;
			m_DmHealthValue = dmContext.param4;
		}
		else if (rpc_type == 14880022)
		{
			ref Param1<ref DmPlayerData> dmPlayerDataContext = new Param1<ref DmPlayerData>(null);
			ctx.Read( dmPlayerDataContext );
			m_dmPlayerData = dmPlayerDataContext.param1;
			m_DmDataUpdated = true;
		}
		else if (rpc_type == 14880033)
		{
			ref Param1<ref DmConnectSyncContext> dmConnSyncCtx = new Param1<ref DmConnectSyncContext>(null);
			ctx.Read( dmConnSyncCtx );
			m_dmConnectSyncCtx = dmConnSyncCtx.param1;
		}
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
		
		if (GetGame().IsServer() && IsAlive())
		{
			DM_RefillStats(deltaTime);
			DM_RefillMags(deltaTime);
			DM_Synch(deltaTime);
			DM_ClearItems(deltaTime);
			
			if (m_dmPlayerData)
			{
				m_dmPlayerData.m_TimeStamp = 0;
			}
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
	
	override bool PredictiveDropEntity (notnull EntityAI item)
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
			
			int distance = (int)Math.Round(vector.Distance(GetWorldPosition(), murder.GetWorldPosition()));
			string srcName = GetIdentity().GetName() + " (" + distance.ToString() + "m)";
			string dstName = murder.GetIdentity().GetName();
			GetRPCManager().SendRPC("DM", "DM_KillFeed", new Param3<string, string, string>(srcName, dstName, m_dmPlayerData.m_CurrentWeapon), true, null); 
			SetHealth01("GlobalHealth", "Health", 1.0);
			SynchDmPlayerDataDirty();
			SetSynchDirty();
		}
	}
	
	override void EEKilled( Object killer )
	{	
		EntityAI killerEntity = EntityAI.Cast(killer);
		if (killerEntity != null)
		{
			PlayerBase killerPlayer = PlayerBase.Cast(killerEntity);
			if (!killerPlayer)
			{
				killerPlayer = PlayerBase.Cast(killerEntity.GetHierarchyRootPlayer());
			}
			
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
		}
		
		
		super.EEKilled( killer );
		
		int fastRespawnTimeoutTime = (int)(m_dmServerSettings.m_fastRespawnTimeout * 1000);
		if (fastRespawnTimeoutTime > 0)
		{
			ref Param2<PlayerBase, PlayerIdentity> fastRespawnParams = new Param2<PlayerBase, PlayerIdentity>(this, GetIdentity());
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(GetGame().GetMission(), "DM_PlayerFastRespawnHandler", fastRespawnTimeoutTime, false, fastRespawnParams);
		}
		else
		{
			ref Param1<PlayerBase> deletePlayerParams = new Param1<PlayerBase>(this);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(GetGame(), "ObjectDelete", 1000, false, deletePlayerParams);
		}
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (source)
		{
			PlayerBase sourcePlayer = PlayerBase.Cast(source);
			if (!sourcePlayer)
			{
				sourcePlayer = PlayerBase.Cast(source.GetHierarchyRootPlayer());
			}
			
			if (sourcePlayer && IsAlive() && this != sourcePlayer)
			{
				m_lastDamageSource = source;
			}
		}
		
		m_DmHealthRegenTimeout = m_dmServerSettings.m_healthRegenTimeoutOnHit;
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	override bool IsInventoryVisible()
	{
		return false;
	}
	
	void DM_RefillStats(float deltaTime)
	{
		m_DmHealthRegenTimeout = Math.Clamp(m_DmHealthRegenTimeout - deltaTime, 0, 100000);
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
			SetSynchDirty();
		}
		
		float shock = GetHealth("GlobalHealth", "Shock");
		float maxShock = GetMaxHealth("GlobalHealth", "Shock");
		if (shock < maxShock - 10)
		{
			SetHealth("GlobalHealth", "Shock", maxShock);
			SetSynchDirty();
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
		
		// float dmDistInZone = DM_GetDistanceInZone();
		// if (dmDistInZone > 0)
		// {
		// 	DecreaseHealth("GlobalHealth", "Health", deltaTime * dmDistInZone * m_dmServerSettings.m_healthOutOfZoneDmg);
		// }
		// else if (m_DmHealthRegenTimeout < 0.1)
		// {
		// 	AddHealth("GlobalHealth", "Health", deltaTime * m_dmServerSettings.m_healthRegen);
		// }
		
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
				ItemBase itemBase = ItemBase.Cast(item);
				if (itemBase && itemBase.GetHealth01("", "") < 0.75)
				{
					itemBase.SetHealth01("", "", 1.0);
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
		
		if (!m_DmPlayerReady)
		{
			m_DmPlayerReady = true;
			ref Param1<ref DmConnectSyncContext> rpcConnCtx = new Param1<ref DmConnectSyncContext>(m_dmConnectSyncCtx);
			
			// dont do async call to try and fix desync issue
			//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RPCSendDelayedConnectSync, 1200, false, rpcConnCtx, GetIdentity());
			
			RPCSingleParam(14880033, ctx, true, identity);
		}
		
		if (m_DmSynchDirty && m_DmSynchTimer > 1.0)
		{
			m_DmSynchTimer = 0;
			m_DmSynchDirty = false;
			ref Param4<float, float, float, int> dmContext = new Param4<float, float, float, int>(m_DmCenterX, m_DmCenterZ, m_DmZoneRadius, m_DmHealthValue);
			RPCSingleParam(14880011, dmContext, true, GetIdentity());
		}
		
		if (m_DmPlayerDataSynchDirty && m_DmPlayerDataSynchTimer > 1.0)
		{
			m_DmPlayerDataSynchTimer = 0;
			m_DmPlayerDataSynchDirty = false;
			ref Param1<ref DmPlayerData> dmPlayerDataContext = new Param1<ref DmPlayerData>(m_dmPlayerData);
			RPCSingleParam(14880022, dmPlayerDataContext, true, GetIdentity());
		}
	}
	
	void DM_ClearItems(float deltaTime)
	{
		if (m_dmServerSettings.m_clearDroppedItemsOnGround == 0)
		{
			return;
		}
		
		m_DmClearItemsTimer = m_DmClearItemsTimer + deltaTime;
		if (m_DmClearItemsTimer > 1.0)
		{
			m_DmClearItemsTimer = 0;
			ItemBase nearestItem = NULL;
			array<Object> nearestObjects();
			GetGame().GetObjectsAtPosition3D(GetWorldPosition(), 10, nearestObjects, NULL);
			foreach (Object nearestObj : nearestObjects)
			{
				if (ItemBase.CastTo(nearestItem, nearestObj))
				{
					GetGame().ObjectDelete(nearestItem);
				}
			}
		}
	}

	void RPCSendDelayedConnectSync(Param1<ref DmConnectSyncContext> ctx, PlayerIdentity identity)
	{
		RPCSingleParam(14880033, ctx, true, identity);
	}

};