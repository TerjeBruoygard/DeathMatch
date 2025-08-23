modded class MissionGameplay
{
	ref DmPlayerMenu m_dmPlayerMenu = null;
	ref array<Particle> m_DM_area_particles = new array<Particle>;
	ref array<Widget> m_DmKillFeedItems = new array<Widget>;
	Widget m_DmHud;
	float m_DmWarningBlinkTimer = 0;
	float m_DmKillfeedClean = 10;
	
	override void OnInit()
	{
		super.OnInit();
		
		if ( !m_DmHud && m_HudRootWidget )
		{
			m_DmHud = GetGame().GetWorkspace().CreateWidgets("DeathMatch/Layouts/DmHud.layout", m_HudRootWidget);
			m_DmHud.Show(false);
		}
		
		GetRPCManager().AddRPC("DM", "DM_KillFeed", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("DM", "DM_LeaderBoard", this, SingleplayerExecutionType.Client);
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
	}
	
	override void OnMissionFinish()
	{
		while (m_DmKillFeedItems.Count() > 0)
		{
			DM_PopKillFeed();
		}
		
		DM_DestroyBorderParticles();
		super.OnMissionFinish();
		
	}
	
	override void OnKeyRelease(int key)
	{
		super.OnKeyRelease(key);
		
		if ( key == KeyCode.KC_ESCAPE )
		{
			if (m_dmPlayerMenu && m_dmPlayerMenu.m_active)
			{
				m_dmPlayerMenu.m_active = false;
			}
		}
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (m_DmHud && player && player.IsAlive())
		{
			m_DmHud.Show(true);
			
			Widget dmOutsideWarning = m_DmHud.FindAnyWidget("WarningOutsidePanel_DM");
			if (dmOutsideWarning)
			{
				if (player.DM_GetDistanceInZone() > 0)
				{
					m_DmWarningBlinkTimer = m_DmWarningBlinkTimer + (timeslice * 3);
					dmOutsideWarning.Show( ((int)m_DmWarningBlinkTimer) % 2 == 0 );
				}
				else
				{
					dmOutsideWarning.Show(false);
					m_DmWarningBlinkTimer = 0;
				}
			}
			
			ProgressBarWidget dmHealthProgress = ProgressBarWidget.Cast(m_DmHud.FindAnyWidget("HealthProgress_DM"));
			TextWidget dmHealthText = TextWidget.Cast(m_DmHud.FindAnyWidget("HealthText_DM"));
			if (dmHealthProgress && dmHealthText)
			{
				dmHealthProgress.SetCurrent(player.m_DmHealthValue);
				dmHealthText.SetText("HP: " + player.m_DmHealthValue.ToString() + "/100");
			}
			
			ProgressBarWidget dmExpProgress = ProgressBarWidget.Cast(m_DmHud.FindAnyWidget("ExpProgress_DM"));
			TextWidget dmExpText = TextWidget.Cast(m_DmHud.FindAnyWidget("ExpText_DM"));
			if (dmExpProgress && dmExpText && player.m_dmPlayerData)
			{
				dmExpProgress.SetCurrent(Math.Clamp((player.m_dmPlayerData.m_Exp / player.m_dmPlayerData.m_Level) * 100.0, 0, 100));
				dmExpText.SetText("LVL: " + player.m_dmPlayerData.m_Level.ToString());
			}
			
			TextWidget dmMoneyText = TextWidget.Cast(m_DmHud.FindAnyWidget("MoneyText_DM"));
			if (dmMoneyText && player.m_dmPlayerData)
			{
				dmMoneyText.SetText(player.m_dmPlayerData.m_Money.ToString());
			}
			
			TextWidget dmDeathText = TextWidget.Cast(m_DmHud.FindAnyWidget("DeathText_DM"));
			if (dmDeathText && player.m_dmPlayerData)
			{
				dmDeathText.SetText(player.m_dmPlayerData.m_Death.ToString());
			}
			
			TextWidget dmKillsText = TextWidget.Cast(m_DmHud.FindAnyWidget("KillsText_DM"));
			if (dmKillsText && player.m_dmPlayerData)
			{
				dmKillsText.SetText(player.m_dmPlayerData.m_Kills.ToString());
			}
			
			if (player.m_DmIsVarsSynch && (player.m_DmZoneRadiusLast != player.m_DmZoneRadius || player.m_DmCenterX != player.m_DmCenterXLast || player.m_DmCenterZ != player.m_DmCenterZLast))
			{
				DM_DestroyBorderParticles();
				DM_SpawnBorderParticles(player, Vector(player.m_DmCenterX, 0, player.m_DmCenterZ), player.m_DmZoneRadius);
				player.m_DmZoneRadiusLast = player.m_DmZoneRadius;
				player.m_DmCenterXLast = player.m_DmCenterX;
				player.m_DmCenterZLast = player.m_DmCenterZ;
			}
			
			UAInput dmMenuKey = GetUApi().GetInputByName("UAToggleDeathMatchMenu");
			TextWidget dmMenuKeyText = TextWidget.Cast(m_DmHud.FindAnyWidget("OpenMenuTooltipKey_DM"));
			if (dmMenuKeyText)
			{
				string keyName = "";
				dmMenuKey.SelectAlternative(0);
				for( int c = 0; c < dmMenuKey.BindKeyCount() && keyName.Length() == 0; c++ )
				{
				  	keyName = GetUApi().GetButtonName(dmMenuKey.GetBindKey(0));
				}
				
				dmMenuKeyText.SetText(keyName);
			}
			
			if ( dmMenuKey.LocalClick() && GetGame().GetUIManager().GetMenu() == NULL )
			{
				if (m_dmPlayerMenu && m_dmPlayerMenu.m_active && player.m_dmConnectSyncCtx) 
				{
					m_dmPlayerMenu.m_dirty = true;
				}
				else
				{		
			        if (!m_dmPlayerMenu)
					{
						m_dmPlayerMenu = new DmPlayerMenu();
					}
		
					GetGame().GetUIManager().ShowScriptedMenu( m_dmPlayerMenu, NULL );
				}
			}
			
			if (player.m_DmDataUpdated && m_dmPlayerMenu && m_dmPlayerMenu.m_active && player.m_dmConnectSyncCtx) 
			{
				player.m_DmDataUpdated = false;
				m_dmPlayerMenu.m_dirty = true;
			}
			
			m_DmKillfeedClean = m_DmKillfeedClean - timeslice;
			if (m_DmKillfeedClean <= 0)
			{
				m_DmKillfeedClean = 10;
				DM_PopKillFeed();
			}
		}
	};
	
	void DM_PopKillFeed()
	{
		if (m_DmKillFeedItems.Count() == 0)
		{
			return;
		}
		
		float x, y, w, h;
		EntityAI wpnObject;
		Widget item = m_DmKillFeedItems.Get(0);
		item.GetSize(w, h);
		
		if (item)
		{
			item.GetUserData(wpnObject);
			if (wpnObject)
			{
				GetGame().ObjectDelete(wpnObject);
			}
			
			m_HudRootWidget.RemoveChild(item);
		}
		
		m_DmKillFeedItems.Remove(0);
		foreach (Widget widget : m_DmKillFeedItems)
		{
			widget.GetPos(x, y);
			widget.SetPos(x, y - (h + 4));
		}
	}
	
	void DM_LeaderBoard(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Client)
		{
			return;
		}
		
		Param1<ref DmLeaderBoardData> data;
		if (!ctx.Read(data))
		{
			return;
		}
		
		if (m_dmPlayerMenu && m_dmPlayerMenu.m_active)
		{
			m_dmPlayerMenu.m_leaderBoardData = data.param1;
			m_dmPlayerMenu.m_dirty = true;
		}
	}
	
	void DM_KillFeed(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Client)
		{
			return;
		}
		
		Param3<string, string, string> data;
		if (!ctx.Read(data))
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
		{
			return;
		}
		
		if (!player.m_dmConnectSyncCtx)
		{
			return;
		}
		
		ref DmWeaponPresset wpnData = player.m_dmConnectSyncCtx.FindWeaponPresset(data.param3);
		if (!wpnData)
		{
			return;
		}
		
		if (m_DmKillFeedItems.Count() > 4)
		{
			DM_PopKillFeed();
		}
		
		float x, y, w, h;
		int count = m_DmKillFeedItems.Count();
		Widget item = GetGame().GetWorkspace().CreateWidgets( "DeathMatch/Layouts/DmKillMessage.layout", m_HudRootWidget );
		item.GetPos(x, y);
		item.GetSize(w, h);
		item.SetPos(x, y + ((h + 4) * count));
		
		TextWidget.Cast(item.FindAnyWidget("DM_Src")).SetText(data.param1);
		TextWidget.Cast(item.FindAnyWidget("DM_Dst")).SetText(data.param2);
		
		Weapon_Base itemObject = Weapon_Base.Cast( GetGame().CreateObject(wpnData.m_Classname, "0 0 0", true, false, false) );
		if (itemObject)
		{			
			item.SetUserData(itemObject);	
			if (wpnData.m_Magazine != "")
			{
				itemObject.CF_SpawnMagazine(wpnData.m_Magazine);
			}

			if (wpnData.m_Attachments && wpnData.m_Attachments.Count() > 0)
			{
				foreach (string attClassname : wpnData.m_Attachments)
				{
					itemObject.GetInventory().CreateAttachment(attClassname);
				}
			}
							
			ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(item.FindAnyWidget("DM_Wpn"));
			previewWidget.SetItem(itemObject);
			previewWidget.SetView(itemObject.GetViewIndex());
			previewWidget.SetModelPosition(Vector(0,0,0.25));
		}
		
		m_DmKillFeedItems.Insert(item);
		m_DmKillfeedClean = 10;
	}
	
	void DM_SpawnBorderParticles(PlayerBase player, vector center, float radius)
	{
		if (!player)
		{
			return;
		}
		
		if (player.m_DmVisibleGasBorders != 1)
		{
			return;
		}
		
		int particleId = ParticleList.CONTAMINATED_AREA_GAS_BIGASS;
		vector pos = "0 0 0";
		radius = radius + 20;
		
		float a;
		float circle_length = Math.PI2 * radius;
		int points_count = (int)(circle_length / 40);
		float points_offset = Math.PI2 / points_count;
		
		a = 0.0;
		while (a < Math.PI2)
		{
			pos[0] = center[0] + (radius * Math.Cos(a));
			pos[2] = center[2] + (radius * Math.Sin(a));
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
			
			m_DM_area_particles.Insert(Particle.PlayInWorld(particleId, pos));
			a = a + points_offset;
		}
	}
	
	void DM_DestroyBorderParticles()
	{
		foreach (Particle p : m_DM_area_particles)
		{
			if (p != null)
			{
				p.Stop();
			}
		}
		m_DM_area_particles.Clear();
	}
};