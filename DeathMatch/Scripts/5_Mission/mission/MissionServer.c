modded class MissionServer
{
	ref DmConnectSyncContext m_DM_ConnectSyncCtx;
	ref DM_ServerSettings m_DM_ServerSettings;
	ref map<string, ref DmPlayerData> m_DmDatabase;
	ref DM_AreaTrail m_DmCurrentTrail;
	float m_DmTrailShift = 0;
	float m_DmTrailTimer = 0;
	float m_DM_currentRadius;
	
    override void OnInit()
	{
		super.OnInit();
		
		m_DmDatabase = new map<string, ref DmPlayerData>;
		m_DM_ServerSettings = new DM_ServerSettings;
		m_DM_ConnectSyncCtx = new DmConnectSyncContext;
		m_DM_ConnectSyncCtx.dm_Weapons = new array<ref DmWeaponPresset>;
		m_DM_ConnectSyncCtx.dm_Equipments = new array<ref DmEquipmentPresset>;
		
		// Settings
		MakeDirectory("$profile:DM");	
		string path = "$profile:DM\\Settings.json";
		if (FileExist(path))
		{
			JsonFileLoader<ref DM_ServerSettings>.JsonLoadFile(path, m_DM_ServerSettings);
		}
		
		if (!m_DM_ServerSettings.m_trails || m_DM_ServerSettings.m_trails.Count() == 0)
		{
			m_DM_ServerSettings.InitDefaultTrails();
		}
		
		JsonFileLoader<ref DM_ServerSettings>.JsonSaveFile(path, m_DM_ServerSettings);
		m_DmCurrentTrail = m_DM_ServerSettings.m_trails.Get(Math.RandomInt(0, m_DM_ServerSettings.m_trails.Count()));
		m_DM_currentRadius = m_DmCurrentTrail.m_minRadius;
		
		// Weapons
		path = "$profile:DM\\Weapons.json";
		if (FileExist(path))
		{
			JsonFileLoader<ref array<ref DmWeaponPresset>>.JsonLoadFile(path, m_DM_ConnectSyncCtx.dm_Weapons);
		}
		else
		{
			DmWeaponPresset.CreateDefaultPressets(m_DM_ConnectSyncCtx.dm_Weapons);
		}
		
		JsonFileLoader<ref array<ref DmWeaponPresset>>.JsonSaveFile(path,  m_DM_ConnectSyncCtx.dm_Weapons);
		
		// Equipment
		path = "$profile:DM\\Equipment.json";
		if (FileExist(path))
		{
			JsonFileLoader<ref array<ref DmEquipmentPresset>>.JsonLoadFile(path, m_DM_ConnectSyncCtx.dm_Equipments);
		}
		else
		{
			DmEquipmentPresset.CreateDefaultPressets(m_DM_ConnectSyncCtx.dm_Equipments);
		}
		
		JsonFileLoader<ref array<ref DmEquipmentPresset>>.JsonSaveFile(path,  m_DM_ConnectSyncCtx.dm_Equipments);
		
		// Players DB
		path = "$profile:DM\\Players.dat";
		if (!FileExist(path) && FileExist(path + ".backup"))
		{
			CopyFile(path + ".backup", path);
		}
		
		if (FileExist(path))
		{
			DM_Log("Start database loading...");
			FileSerializer ctx = new FileSerializer();
			ctx.Open(path, FileMode.READ);
			if (ctx.IsOpen())
			{
				int ver;
				int count;
				ctx.Read(ver);
				ctx.Read(count);			
				for (int i = 0; i < count; i++)
				{
					string sid;
					ref DmPlayerData dmData = new DmPlayerData();
					ctx.Read(sid);
					dmData.Deserialize(ctx, ver);
					m_DmDatabase.Set(sid, dmData);
				}
				
				DM_Log("Finish database loading...OK " + count.ToString());
			}
		}
		
		GetRPCManager().AddRPC("DM", "DM_WeaponBuy", this, SingleplayerExecutionType.Server);
		GetRPCManager().AddRPC("DM", "DM_EquipmentBuy", this, SingleplayerExecutionType.Server);
	};
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		string path = "$profile:DM\\Players.dat";
		if (FileExist(path))
		{
			CopyFile(path, path + ".backup");
			DeleteFile(path);
		}
		
		DM_Log("Start database saving...");
		FileSerializer ctx = new FileSerializer();
		ctx.Open(path, FileMode.WRITE);
		if (ctx.IsOpen())
		{
			int ver = 1;
			int count = m_DmDatabase.Count();
			ctx.Write(ver);
			ctx.Write(count);			
			foreach (string sid, ref DmPlayerData dmData : m_DmDatabase)
			{
				ctx.Write(sid);
				dmData.Serialize(ctx);
			}
			
			DM_Log("Finish database saving...OK " + count.ToString());
		}
	}
	
	override void OnUpdate(float timeslice)
	{
		array<Man> playersList();
		GetGame().GetPlayers(playersList);		
		m_DM_currentRadius = Math.Clamp(playersList.Count() * m_DM_ServerSettings.m_expandStep, m_DmCurrentTrail.m_minRadius, m_DmCurrentTrail.m_maxRadius);
		
		m_DmTrailTimer = m_DmTrailTimer + (timeslice * m_DM_ServerSettings.m_areaMoveSpeed);
		if (m_DmTrailTimer > 10)
		{
			float dist = DM_GetPointsDist();
			if (dist == 0)
			{
				dist = 0.1;
			}
			
			m_DmTrailShift = m_DmTrailShift + (10 / dist);
			m_DmTrailTimer = 0;
		}
		
		vector dmCurPos = DM_GetAreaPos();
		foreach (Man manObj : playersList)
		{
			PlayerBase player = PlayerBase.Cast(manObj);
			if (player)
			{
				if (!player.m_DmIsVarsSynch || player.m_DmZoneRadius != m_DM_currentRadius || player.m_DmCenterX != dmCurPos[0] || player.m_DmCenterZ != dmCurPos[2])
				{
					player.m_DmCenterX = dmCurPos[0];
					player.m_DmCenterZ = dmCurPos[2];
					player.m_DmZoneRadius = m_DM_currentRadius;
					player.m_DmIsVarsSynch = true;
					player.SynchDmDirty();
				}
			}
		}
		
		super.OnUpdate(timeslice);
	};
	
	float DM_GetPointsDist()
	{
		int start = (int)m_DmTrailShift;
		if (start >= m_DmCurrentTrail.m_points.Count())
		{
			m_DmTrailShift = 0;
			start = 0;
		}
		
		int end = start + 1;
		if (end >= m_DmCurrentTrail.m_points.Count())
		{
			end = 0;
		}
		
		vector startPos = m_DmCurrentTrail.m_points.Get(start);
		vector endPos = m_DmCurrentTrail.m_points.Get(end);
		
		startPos[1] = 0;
		endPos[1] = 0;
		return vector.Distance(startPos, endPos);
	}
	
	vector DM_GetAreaPos()
	{
		int start = (int)m_DmTrailShift;
		if (start >= m_DmCurrentTrail.m_points.Count())
		{
			m_DmTrailShift = 0;
			start = 0;
		}
		
		int end = start + 1;
		if (end >= m_DmCurrentTrail.m_points.Count())
		{
			end = 0;
		}
		
		vector startPos = m_DmCurrentTrail.m_points.Get(start);
		vector endPos = m_DmCurrentTrail.m_points.Get(end);
		float delta = Math.Clamp(m_DmTrailShift - start, 0, 1);
		vector result = vector.Lerp(startPos, endPos, delta);
		result[1] = GetGame().SurfaceY(result[0], result[2]);
		return result;
	}
	
	void DM_WeaponBuy(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Server)
		{
			return;
		}
		
		Param1<string> data;
		if (!ctx.Read(data)) return;

		ref DmWeaponPresset wpnData = m_DM_ConnectSyncCtx.FindWeaponPresset(data.param1);
		if (!wpnData)
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(target);
		if (!player || !player.IsAlive())
		{
			return;
		}

		DM_Log("DM_WeaponBuy: " + sender.GetName() + ": " + sender.GetId() + " => " + wpnData.m_Name);
		
		if (player.m_dmPlayerData.IsWeaponLocked(wpnData))
		{
			return;
		}
		
		if (!player.m_dmPlayerData.ContainsWeapon(player.m_dmConnectSyncCtx, wpnData.m_Id))
		{
			player.m_dmPlayerData.AddWeapon(wpnData.m_Id);
			player.m_dmPlayerData.m_Money = player.m_dmPlayerData.m_Money - wpnData.m_Price;
			if (player.m_dmPlayerData.m_Money < 0)
			{
				player.m_dmPlayerData.m_Money = 0;
			}
		}
		
		player.m_dmPlayerData.m_CurrentWeapon = wpnData.m_Id;
		player.SynchDmPlayerDataDirty();
		EquipPlayerWeapon_DM(player, wpnData);
	}
	
	void DM_EquipmentBuy(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Server)
		{
			return;
		}
		
		Param1<string> data;
		if (!ctx.Read(data)) return;

		ref DmEquipmentPresset eqpData = m_DM_ConnectSyncCtx.FindEquipmentPresset(data.param1);
		if (!eqpData)
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(target);
		if (!player || !player.IsAlive())
		{
			return;
		}
		
		DM_Log("DM_WeaponBuy: " + sender.GetName() + ": " + sender.GetId() + " => " + eqpData.m_Name);
		
		if (player.m_dmPlayerData.IsEquipmentLocked(eqpData))
		{
			return;
		}
		
		if (!player.m_dmPlayerData.ContainsEquipment(player.m_dmConnectSyncCtx, eqpData.m_Id))
		{
			player.m_dmPlayerData.AddEquipment(eqpData.m_Id);
			player.m_dmPlayerData.m_Money = player.m_dmPlayerData.m_Money - eqpData.m_Price;
			if (player.m_dmPlayerData.m_Money < 0)
			{
				player.m_dmPlayerData.m_Money = 0;
			}
		}
		
		player.m_dmPlayerData.m_CurrentEquipment = eqpData.m_Id;
		player.SynchDmPlayerDataDirty();
		EquipPlayerClothing_DM(player, eqpData);
		
		ref DmWeaponPresset wpnData = m_DM_ConnectSyncCtx.FindWeaponPresset(player.m_dmPlayerData.m_CurrentWeapon);
		if (wpnData)
		{
			EquipPlayerWeapon_DM(player, wpnData);
		}
	}
	
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		string sid = identity.GetId();
		ref DmPlayerData dmData = null;
		if (!m_DmDatabase.Find(sid, dmData))
		{
			dmData = new DmPlayerData();
			dmData.Init(identity, m_DM_ServerSettings.m_startMoney);
			m_DmDatabase.Insert(sid, dmData);
			DM_Log("Create new player: " + sid);
		}
		else
		{
			DM_Log("Load exist player: " + sid);
		}
		
		if (dmData.m_CurrentWeapon.Length() == 0 || !dmData.ContainsWeapon(m_DM_ConnectSyncCtx, dmData.m_CurrentWeapon))
		{
			dmData.m_CurrentWeapon = m_DM_ConnectSyncCtx.dm_Weapons.Get(0).m_Id;
		}
		
		if (dmData.m_CurrentEquipment.Length() == 0 || !dmData.ContainsEquipment(m_DM_ConnectSyncCtx, dmData.m_CurrentEquipment))
		{
			dmData.m_CurrentEquipment = m_DM_ConnectSyncCtx.dm_Equipments.Get(0).m_Id;
		}
		
		pos = CalculateSafePos_DM(DM_GetAreaPos(), Math.Clamp(m_DM_currentRadius - 20, 10, 10000));
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");
		Class.CastTo(m_player, playerEnt);
		GetGame().SelectPlayer(identity, m_player);
		
		m_player.m_dmServerSettings = m_DM_ServerSettings;
		m_player.m_dmPlayerData = dmData;
		m_player.m_dmConnectSyncCtx = m_DM_ConnectSyncCtx;
		m_player.SynchDmPlayerDataDirty();
		
		ref DmEquipmentPresset eqpData = m_DM_ConnectSyncCtx.FindEquipmentPresset(dmData.m_CurrentEquipment);
		if (eqpData)
		{
			EquipPlayerClothing_DM(m_player, eqpData);
		}
		
		ref DmWeaponPresset wpnData = m_DM_ConnectSyncCtx.FindWeaponPresset(dmData.m_CurrentWeapon);
		if (wpnData)
		{
			EquipPlayerWeapon_DM(m_player, wpnData);
		}
		
		return m_player;
	};
	
	override void EquipCharacter(MenuDefaultCharacterData char_data)
	{

	};
	
	void EquipPlayerClothing_DM(PlayerBase player, ref DmEquipmentPresset ep)
	{
		player.ClearInventory();
		foreach (string itClass : ep.m_Items)
		{
			EntityAI itemObj = player.GetInventory().CreateInInventory(itClass);
			if (itemObj && DM_HasBatterySlot(itClass))
			{
				itemObj.GetInventory().CreateAttachment("Battery9V");
			}
		}
	}
	
	void EquipPlayerWeapon_DM(PlayerBase player, ref DmWeaponPresset wp)
	{
		array<EntityAI> items();
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (auto item : items)
		{
			Weapon_Base wpn = Weapon_Base.Cast(item);
			if (wpn)
			{
				GetGame().ObjectDelete(wpn);
				continue;
			}
			
			Magazine mag = Magazine.Cast(item);
			if (mag && !mag.GetInventory().IsAttachment())
			{
				GetGame().ObjectDelete(mag);
				continue;
			}
		}
		
		Weapon_Base weapon = Weapon_Base.Cast(player.GetHumanInventory().CreateInHands(wp.m_Classname));
		foreach (string att : wp.m_Attachments)
		{
			EntityAI attachmentItem = player.GetInventory().CreateAttachment(att);
			if (attachmentItem && DM_HasBatterySlot(att))
			{
				attachmentItem.GetInventory().CreateAttachment("Battery9V");
			}
		}
		
		if (wp.m_Magazine != "")
		{
			weapon.CF_SpawnMagazine(wp.m_Magazine);
			player.GetInventory().CreateInInventory(wp.m_Magazine);
		}
	}
	
	vector CalculateSafePos_DM(vector center, float radius)
	{
		int retries = 0;
		vector contactPos = vector.Zero;
		vector contactDir = vector.Zero;
		int contactComp = 0;
		while (true)
		{
			float angle = 2.0 * Math.PI * Math.RandomFloat01();
			float dist = Math.Sqrt(Math.RandomFloat01()) * radius;
			float x = center[0] + dist * Math.Cos(angle);
	    	float z = center[2] + dist * Math.Sin(angle);
			float y = GetGame().SurfaceY(x, z);
			if (GetGame().SurfaceIsSea(x, z))
			{
				continue;
			}
			
			if (GetGame().SurfaceIsPond(x, z))
			{
				continue;
			}
			
			vector raycastFrom = Vector(x, y + 0.2, z);
			vector raycastTo = Vector(x, y + 100, z);
			
			if (DayZPhysics.RaycastRV(raycastFrom, raycastTo, contactPos, contactDir, contactComp))
			{
				continue;
			}
			
			if (retries < 100)
			{
				vector checkPos = Vector(x, y + 1.5, z);
				if (CheckNearestPlayer(checkPos, 10, 50))
				{
					retries = retries + 1;
					continue;
				}
			}
			
			return Vector(x, y, z);
		}
		
		return vector.Zero;
	};
	
	bool CheckNearestPlayer(vector pos, float minDist, float maxDist)
	{
		vector contactPos = vector.Zero;
		vector contactDir = vector.Zero;
		int contactComp = 0;
		array<Man> playersList();
		GetGame().GetPlayers(playersList);	
		foreach (Man target: playersList)
		{
			vector targetPos = target.GetWorldPosition();
			float curDist = vector.Distance(pos, targetPos);
			if (curDist > maxDist)
			{
				continue;
			}
			
			if (curDist < minDist)
			{
				return true;
			}
			
			targetPos[1] = targetPos[1] + 1.5;
			if (!DayZPhysics.RaycastRV(pos, targetPos, contactPos, contactDir, contactComp, NULL, NULL, target))
			{
				return true;
			}
		}
		
		return false;
	};
};