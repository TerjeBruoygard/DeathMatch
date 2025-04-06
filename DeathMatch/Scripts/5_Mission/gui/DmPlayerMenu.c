class DmPlayerMenu extends UIScriptedMenu
{
    bool m_active = false;
	bool m_dirty = false;
	static string m_selectedPage = "Weapons";
	static string m_selectedLastPage = "Weapons";
	
	float m_weaponsPageScroll = 0;
	float m_equipmentPageScroll = 0;
	
	ref array<Widget> m_weaponWidgets = new array<Widget>;
	ref array<Weapon_Base> m_weaponEntities = new array<Weapon_Base>;
	
	ref array<Widget> m_equipmentWidgets = new array<Widget>;
	ref array<EntityAI> m_equipmentEntities = new array<EntityAI>;
	
	// ref array<Widget> m_leaderBoardWidgets = new array<Widget>;
	// ref DmLeaderBoardData m_leaderBoardData = null;
	
	override Widget Init()
    {
		layoutRoot = GetGame().GetWorkspace().CreateWidgets( "DeathMatch/Layouts/DmPlayerMenu.layout" );
		m_active = true;
		m_dirty = true;
        return layoutRoot;
    };
	
	void SelectPage()
	{		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
		{
			return;
		}
		
		if (m_selectedLastPage != m_selectedPage)
		{
			layoutRoot.FindAnyWidget( "PageTab" + m_selectedLastPage ).SetColor( ARGB(180, 255, 255, 255) );
			m_selectedLastPage = m_selectedPage;
		}
		
		layoutRoot.FindAnyWidget( "PageTab" + m_selectedPage ).SetColor( ARGB(255, 255, 255, 255) );
		
		if (m_selectedPage == "Weapons")
		{
			DisplayWeaponsPage();
		}
		else if (m_selectedPage == "Equipments")
		{
			DisplayEquipmentsPage();
		}
		// else if (m_selectedPage == "LeaderBoard")
		// {
		// 	if (m_leaderBoardData == null)
		// 	{
		// 		HideAllPages();
		// 		GetRPCManager().SendRPC("DM", "DM_LeaderBoardSrv", new Param1<int>(0), true, null, player); 
		// 	}
		// 	else
		// 	{
		// 		DisplayLeaderBoardPage();
		// 	}
		// }
	}
	
	void HideAllPages()
	{
		layoutRoot.FindAnyWidget( "PageBodyWeapons" ).Show(false);
		layoutRoot.FindAnyWidget( "PageBodyEquipments" ).Show(false);
		// layoutRoot.FindAnyWidget( "PageBodyLeaderBoard" ).Show(false);
	}
	
	void ClearWeaponsCache()
	{
		foreach (Weapon_Base w : m_weaponEntities)
		{
			GetGame().ObjectDelete(w);
		}
		
		m_weaponEntities.Clear();
	}
	
	void ClearEquipmentsCache()
	{
		foreach (EntityAI e : m_equipmentEntities)
		{
			GetGame().ObjectDelete(e);
		}
		
		m_equipmentEntities.Clear();
	}
	
	// void DisplayLeaderBoardPage()
	// {
	// 	HideAllPages();
	// 	layoutRoot.FindAnyWidget( "PageBodyLeaderBoard" ).Show(true);
		
	// 	float w, h, c;
	// 	ScrollWidget scrollWidget = ScrollWidget.Cast(layoutRoot.FindAnyWidget( "PageBodyLeaderBoardScroll" ));
	// 	foreach (Widget del : m_leaderBoardWidgets)
	// 	{
	// 		scrollWidget.RemoveChild(del);
	// 	}
		
	// 	m_leaderBoardWidgets.Clear();
	// 	float contentWidth = scrollWidget.GetContentWidth();	
	// 	c = 0;
	// 	for (int i = 0; i < m_leaderBoardData.m_Names.Count(); i++)
	// 	{
	// 		int index = m_leaderBoardData.m_Index.Get(i);
	// 		if (index == -1)
	// 		{
	// 			continue;
	// 		}
			
	// 		Widget item = GetGame().GetWorkspace().CreateWidgets( "DeathMatch/Layouts/DmPlayerMenu_ItemLeaderboard.layout" );
	// 		scrollWidget.AddChild(item);
	// 		item.GetSize(w, h);
	// 		item.SetPos(0, (h + 2) * c);
	// 		item.SetSize(contentWidth - 5, h);
	// 		c = c + 1;
			
	// 		string name = m_leaderBoardData.m_Names.Get(i);
	// 		int level = m_leaderBoardData.m_Levels.Get(i);
	// 		int kills = m_leaderBoardData.m_Kills.Get(i);
	// 		int death = m_leaderBoardData.m_Death.Get(i);
	// 		float kd = 0;
			
	// 		if (kills > 0 && death > 0)
	// 		{
	// 			kd = (float)kills / (float)death;
	// 		}
			
	// 		string kdStr = kd.ToString();
	// 		int dotIndex = kdStr.IndexOf(".");
	// 		if (dotIndex != -1 && kdStr.Length() - dotIndex > 2)
	// 		{
	// 			kdStr = kdStr.Substring(0, dotIndex + 3);
	// 		}
			
	// 		TextWidget.Cast(item.FindAnyWidget("DM_LB_Name")).SetText(index.ToString() + ": " + name);
	// 		TextWidget.Cast(item.FindAnyWidget("DM_LB_Level")).SetText("Level: " + level.ToString());
	// 		TextWidget.Cast(item.FindAnyWidget("DM_LB_Kills")).SetText("Kills: " + kills.ToString());
	// 		TextWidget.Cast(item.FindAnyWidget("DM_LB_Death")).SetText("Death: " + death.ToString());
	// 		TextWidget.Cast(item.FindAnyWidget("DM_LB_Kd")).SetText("KD: " + kdStr);
	// 	}
	// }
	
	void DisplayEquipmentsPage()
	{
		HideAllPages();
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.m_dmConnectSyncCtx || !player.m_dmPlayerData)
		{
			return;
		}
		
		layoutRoot.FindAnyWidget( "PageBodyEquipments" ).Show(true);
		
		TextWidget.Cast(layoutRoot.FindAnyWidget("PageBodyEquipmentsLevel")).SetText("Your level: " + player.m_dmPlayerData.m_Level.ToString());
		TextWidget.Cast(layoutRoot.FindAnyWidget("PageBodyEquipmentsMoneyText")).SetText(player.m_dmPlayerData.m_Money.ToString());
		
		ScrollWidget scrollWidget = ScrollWidget.Cast(layoutRoot.FindAnyWidget( "PageBodyEquipmentsScroll" ));
		m_equipmentPageScroll = scrollWidget.GetVScrollPos01();
		foreach (Widget del : m_equipmentWidgets)
		{
			scrollWidget.RemoveChild(del);
		}
		
		int i = 0;
		float w, h;
		float contentWidth = scrollWidget.GetContentWidth();		
		m_equipmentWidgets.Clear();
		ClearEquipmentsCache();
		
		foreach (ref DmEquipmentPresset wpnData : player.m_dmConnectSyncCtx.dm_Equipments)
		{
			Widget item = GetGame().GetWorkspace().CreateWidgets( "DeathMatch/Layouts/DmPlayerMenu_ItemEquipment.layout" );
			scrollWidget.AddChild(item);
			item.GetSize(w, h);
			item.SetPos(0, (h + 2) * i);
			item.SetSize(contentWidth - 5, h);
			
			// dont check if player has equip, just assume they do
			if (player.m_dmPlayerData.IsEquipmentSelected(wpnData))
			{
				item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 48, 40));
				item.FindAnyWidget("DM_WI_Price").Show(false);
				item.FindAnyWidget("DM_WI_Btn").Show(false);
			}
			else
			{
				item.FindAnyWidget("DM_WI_Btn").SetUserData(wpnData);
				item.FindAnyWidget("DM_WI_Btn").SetUserID(2001);
				item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 40, 40));
				item.FindAnyWidget("DM_WI_Price").Show(false);
				item.FindAnyWidget("DM_WI_BtnBackground").SetColor(ARGB(81, 0, 155, 38));
				TextWidget.Cast(item.FindAnyWidget("DM_WI_BtnText")).SetText("USE");
			}

			// if (player.m_dmPlayerData.ContainsEquipment(player.m_dmConnectSyncCtx, wpnData.m_Id))
			// {
			// 	if (player.m_dmPlayerData.IsEquipmentSelected(wpnData))
			// 	{
			// 		item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 48, 40));
			// 		item.FindAnyWidget("DM_WI_Price").Show(false);
			// 		item.FindAnyWidget("DM_WI_Btn").Show(false);
			// 	}
			// 	else
			// 	{
			// 		item.FindAnyWidget("DM_WI_Btn").SetUserData(wpnData);
			// 		item.FindAnyWidget("DM_WI_Btn").SetUserID(2001);
			// 		item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 40, 40));
			// 		item.FindAnyWidget("DM_WI_Price").Show(false);
			// 		item.FindAnyWidget("DM_WI_BtnBackground").SetColor(ARGB(81, 0, 155, 38));
			// 		TextWidget.Cast(item.FindAnyWidget("DM_WI_BtnText")).SetText("USE");
			// 	}
			// }
			// else if (player.m_dmPlayerData.IsEquipmentLocked(wpnData))
			// {
			// 	item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 48, 40, 40));
			// 	item.FindAnyWidget("DM_WI_Btn").Show(false);
			// }
			// else
			// {
			// 	item.FindAnyWidget("DM_WI_Btn").SetUserData(wpnData);
			// 	item.FindAnyWidget("DM_WI_Btn").SetUserID(2001);
			// 	item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 40, 40));
			// 	item.FindAnyWidget("DM_WI_BtnBackground").SetColor(ARGB(81, 175, 144, 49));
			// 	TextWidget.Cast(item.FindAnyWidget("DM_WI_BtnText")).SetText("BUY");
			// }
			
			TextWidget.Cast(item.FindAnyWidget("DM_WI_Name")).SetText(wpnData.m_Name);
			
			// dont show level or price
			item.FindAnyWidget("DM_WI_Level").Show(false);
			item.FindAnyWidget("DM_WI_Price").Show(false);
			// TextWidget.Cast(item.FindAnyWidget("DM_WI_Level")).SetText("Unlock level: " + wpnData.m_Level.ToString());
			// if (wpnData.m_Price > 0)
			// {
			// 	TextWidget.Cast(item.FindAnyWidget("DM_WI_Price")).SetText("Price: " + wpnData.m_Price.ToString() + "$");
			// }
			// else
			// {
			// 	item.FindAnyWidget("DM_WI_Price").Show(false);
			// }
			
			for (int q = 0; q < wpnData.m_Items.Count() && q < 5; q++)
			{
				EntityAI itemObject = EntityAI.Cast( GetGame().CreateObject(wpnData.m_Items.Get(q), "0 0 0", true, false, false) );
				if (itemObject)
				{
					ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(item.FindAnyWidget("DM_WI_Preview" + q.ToString()));
					previewWidget.SetItem(itemObject);
					previewWidget.SetView(itemObject.GetViewIndex());
					previewWidget.SetModelPosition(Vector(0,0,0.25));
					m_equipmentEntities.Insert(itemObject);
				}
			}
			
			i = i + 1;
		}
		
		scrollWidget.VScrollToPos01(m_equipmentPageScroll);
	}
	
	void DisplayWeaponsPage()
	{
		HideAllPages();
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.m_dmConnectSyncCtx || !player.m_dmPlayerData)
		{
			return;
		}
		
		layoutRoot.FindAnyWidget( "PageBodyWeapons" ).Show(true);
		
		// hide level and money for possible future use
		// TextWidget.Cast(layoutRoot.FindAnyWidget("PageBodyWeaponsLevel")).SetText("Your level: " + player.m_dmPlayerData.m_Level.ToString());
		// TextWidget.Cast(layoutRoot.FindAnyWidget("PageBodyWeaponsMoneyText")).SetText(player.m_dmPlayerData.m_Money.ToString());
		
		ScrollWidget scrollWidget = ScrollWidget.Cast(layoutRoot.FindAnyWidget( "PageBodyWeaponsScroll" ));
		m_weaponsPageScroll = scrollWidget.GetVScrollPos01();
		foreach (Widget del : m_weaponWidgets)
		{
			scrollWidget.RemoveChild(del);
		}
		
		int i = 0;
		float w, h;
		float contentWidth = scrollWidget.GetContentWidth();		
		m_weaponWidgets.Clear();
		ClearWeaponsCache();
		
		foreach (ref DmWeaponPresset wpnData : player.m_dmConnectSyncCtx.dm_Weapons)
		{
			Widget item = GetGame().GetWorkspace().CreateWidgets( "DeathMatch/Layouts/DmPlayerMenu_ItemWeapon.layout" );
			scrollWidget.AddChild(item);
			item.GetSize(w, h);
			item.SetPos(0, (h + 2) * i);
			item.SetSize(contentWidth - 5, h);
			
			// dont check if player owns weapon, just assume they do
			if (player.m_dmPlayerData.IsWeaponSelected(wpnData))
			{
				item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 48, 40));
				item.FindAnyWidget("DM_WI_Price").Show(false);
				item.FindAnyWidget("DM_WI_Btn").Show(false);
			}
			else
			{
				item.FindAnyWidget("DM_WI_Btn").SetUserData(wpnData);
				item.FindAnyWidget("DM_WI_Btn").SetUserID(1001);
				item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 40, 40));
				item.FindAnyWidget("DM_WI_Price").Show(false);
				item.FindAnyWidget("DM_WI_BtnBackground").SetColor(ARGB(81, 0, 155, 38));
				TextWidget.Cast(item.FindAnyWidget("DM_WI_BtnText")).SetText("USE");
			}

			// if (player.m_dmPlayerData.ContainsWeapon(player.m_dmConnectSyncCtx, wpnData.m_Id))
			// {
			// 	if (player.m_dmPlayerData.IsWeaponSelected(wpnData))
			// 	{
			// 		item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 48, 40));
			// 		item.FindAnyWidget("DM_WI_Price").Show(false);
			// 		item.FindAnyWidget("DM_WI_Btn").Show(false);
			// 	}
			// 	else
			// 	{
			// 		item.FindAnyWidget("DM_WI_Btn").SetUserData(wpnData);
			// 		item.FindAnyWidget("DM_WI_Btn").SetUserID(1001);
			// 		item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 40, 40));
			// 		item.FindAnyWidget("DM_WI_Price").Show(false);
			// 		item.FindAnyWidget("DM_WI_BtnBackground").SetColor(ARGB(81, 0, 155, 38));
			// 		TextWidget.Cast(item.FindAnyWidget("DM_WI_BtnText")).SetText("USE");
			// 	}
			// }
			// else if (player.m_dmPlayerData.IsWeaponLocked(wpnData))
			// {
			// 	item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 48, 40, 40));
			// 	item.FindAnyWidget("DM_WI_Btn").Show(false);
			// }
			// else
			// {
			// 	item.FindAnyWidget("DM_WI_Btn").SetUserData(wpnData);
			// 	item.FindAnyWidget("DM_WI_Btn").SetUserID(1001);
			// 	item.FindAnyWidget("DM_WI_Background").SetColor(ARGB(255, 40, 40, 40));
			// 	item.FindAnyWidget("DM_WI_BtnBackground").SetColor(ARGB(81, 175, 144, 49));
			// 	TextWidget.Cast(item.FindAnyWidget("DM_WI_BtnText")).SetText("BUY");
			// }
			
			TextWidget.Cast(item.FindAnyWidget("DM_WI_Name")).SetText(wpnData.m_Name);
			
			// dont display any level text
			// TextWidget.Cast(item.FindAnyWidget("DM_WI_Level")).SetText("Unlock level: " + wpnData.m_Level.ToString());


			// always hide price and level
			item.FindAnyWidget("DM_WI_Level").Show(false);
			item.FindAnyWidget("DM_WI_Price").Show(false);
			
			// dont show price
			// if (wpnData.m_Price > 0)
			// {
			// 	TextWidget.Cast(item.FindAnyWidget("DM_WI_Price")).SetText("Price: " + wpnData.m_Price.ToString() + "$");
			// }
			// else
			// {
			// 	item.FindAnyWidget("DM_WI_Price").Show(false);
			// }
			
			Weapon_Base itemObject = Weapon_Base.Cast( GetGame().CreateObject(wpnData.m_Classname, "0 0 0", true, false, false) );
			if (itemObject)
			{				
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
								
				ItemPreviewWidget previewWidget = ItemPreviewWidget.Cast(item.FindAnyWidget("DM_WI_Preview"));
				previewWidget.SetItem(itemObject);
				previewWidget.SetView(itemObject.GetViewIndex());
				previewWidget.SetModelPosition(Vector(0,0,0.25));
				m_weaponEntities.Insert(itemObject);
			}
			
			i = i + 1;
		}
		
		scrollWidget.VScrollToPos01(m_weaponsPageScroll);
	}
	
	override void Update(float timeslice)
	{
		super.Update(timeslice);
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.IsAlive())
		{
			m_active = false;
		}
		
		if (m_dirty)
		{
			SelectPage();
			m_dirty = false;
		}

		if (!m_active)
		{
			GetGame().GetUIManager().Back();
		}
	};
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);	
		
		if (button == MouseState.LEFT)
		{
			if (w.GetParent().GetName() == "PageTabWeapons")
			{
				m_selectedPage = "Weapons";
				m_dirty = true;
			}
			else if (w.GetParent().GetName() == "PageTabEquipments")
			{
				m_selectedPage = "Equipments";
				m_dirty = true;
			}
			// else if (w.GetParent().GetName() == "PageTabLeaderBoard")
			// {
			// 	m_selectedPage = "LeaderBoard";
			// 	m_dirty = true;
			// }
			
			if (m_selectedPage == "Weapons")
			{
				if (w.GetUserID() == 1001)
				{
					ref DmWeaponPresset dmWp = null;
					w.GetUserData(dmWp);
					if (dmWp)
					{
						GetRPCManager().SendRPC("DM", "DM_WeaponBuy", new Param1<string>(dmWp.m_Id), true, null, GetGame().GetPlayer()); 
					}
				}
			}
			else if (m_selectedPage == "Equipments")
			{
				if (w.GetUserID() == 2001)
				{
					ref DmEquipmentPresset dmEp = null;
					w.GetUserData(dmEp);
					if (dmEp)
					{
						GetRPCManager().SendRPC("DM", "DM_EquipmentBuy", new Param1<string>(dmEp.m_Id), true, null, GetGame().GetPlayer()); 
					}
				}
			}
		}
		
		return false;
	};
	
	override void OnShow()
	{
		super.OnShow();

		GetGame().GetInput().ChangeGameFocus(1);

		SetFocus( layoutRoot );
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			player.GetInputController().SetDisabled(true);
			player.GetActionManager().EnableActions(false);
		}
	}

	override void OnHide()
	{		
		super.OnHide();

		GetGame().GetInput().ResetGameFocus();
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			player.GetInputController().SetDisabled(false);
			player.GetActionManager().EnableActions(true);
		}
		
		Close();
		ClearWeaponsCache();
		ClearEquipmentsCache();
		// m_leaderBoardData = null;
	}
};