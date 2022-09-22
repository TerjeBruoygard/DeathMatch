modded class MissionGameplay
{
	ref DmPlayerMenu m_dmPlayerMenu = null;
	ref array<Particle> m_DM_area_particles = new array<Particle>;
	Widget m_DmHud;
	float m_WarningBlinkTimer = 0;
	
	override void OnInit()
	{
		super.OnInit();
		
		if ( !m_DmHud && m_HudRootWidget )
		{
			m_DmHud = GetGame().GetWorkspace().CreateWidgets("DeathMatch/Layouts/DmHud.layout", m_HudRootWidget);
			m_DmHud.Show(false);
		}
		
		GetRPCManager().AddRPC("DM", "DM_PSynchStat", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("DM", "DM_PSynchData", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("DM", "DM_PSynchInfo", this, SingleplayerExecutionType.Client);
	}
	
	void DM_PSynchStat(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Client)
		{
			return;
		}
		
		if (sender != null)
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if (player)
		{
			if (player.IsAlive())
			{
				player.DM_PSynchStat(ctx);
				DM_Log("DM_PSynchStat - OK");
			}
		}
		else
		{
			DM_Log("DM_PSynchStat player not found.");
		}
	}
	
	void DM_PSynchData(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Client)
		{
			return;
		}
		
		if (sender != null)
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if (player)
		{
			if (player.IsAlive())
			{
				player.DM_PSynchData(ctx);
				DM_Log("DM_PSynchData - OK");
				
				if (m_dmPlayerMenu && m_dmPlayerMenu.m_active && player.m_dmConnectSyncCtx) 
				{
					m_dmPlayerMenu.m_dirty = true;
				}
			}
		}
		else
		{
			DM_Log("DM_PSynchData player not found.");
		}
	}
	
	void DM_PSynchInfo(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		if (type != CallType.Client)
		{
			return;
		}
		
		if (sender != null)
		{
			return;
		}
		
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if (player)
		{
			if (player.IsAlive())
			{
				player.DM_PSynchInfo(ctx);
				DM_Log("DM_PSynchInfo - OK");
			}
		}
		else
		{
			DM_Log("DM_PSynchInfo player not found.");
		}
	}
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
	}
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		DM_DestroyBorderParticles();
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
					m_WarningBlinkTimer = m_WarningBlinkTimer + (timeslice * 3);
					dmOutsideWarning.Show( ((int)m_WarningBlinkTimer) % 2 == 0 );
				}
				else
				{
					dmOutsideWarning.Show(false);
					m_WarningBlinkTimer = 0;
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
			
			if (player.m_DmIsVarsSynch && player.m_DmZoneRadiusLast != player.m_DmZoneRadius)
			{
				DM_DestroyBorderParticles();
				DM_SpawnBorderParticles(Vector(player.m_DmCenterX, 0, player.m_DmCenterZ), player.m_DmZoneRadius);
				player.m_DmZoneRadiusLast = player.m_DmZoneRadius;
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
		}
	};
	
	void DM_SpawnBorderParticles(vector center, float radius)
	{
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