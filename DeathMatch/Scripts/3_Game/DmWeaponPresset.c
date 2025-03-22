class DmWeaponPresset
{
	string m_Id;
	string m_Name;
	string m_Classname;
	ref array<string> m_Attachments = new array<string>;
	string m_Magazine;
	int m_Level;
	int m_Price;
	
	ref DmWeaponPresset Init(string id, string name, string classname, string magazine, int level, int price)
	{
		m_Id = id;
		m_Name = name;
		m_Classname = classname;
		m_Magazine = magazine;
		m_Level = level;
		m_Price = price;
		return this;
	}
	
	ref DmWeaponPresset Atth(string classname)
	{
		m_Attachments.Insert(classname);
		return this;
	}
	
	static void CreateDefaultPressets(array<ref DmWeaponPresset> result)
	{
		result.Insert((new DmWeaponPresset()).Init("makarov", "IJ70 'Makarov'", "MakarovIJ70", "Mag_IJ70_8Rnd", 1, 0));
		result.Insert((new DmWeaponPresset()).Init("izh18", "Izh18 'Singleshot'", "Izh18", "Ammo_762x39Tracer", 1, 0));
		result.Insert((new DmWeaponPresset()).Init("izh43", "Izh43 'Shotgun'", "Izh43Shotgun", "Ammo_12gaPellets", 1, 0));
		
		result.Insert((new DmWeaponPresset()).Init("P1", "P1", "P1", "Mag_P1_8Rnd", 2, 0));
		result.Insert((new DmWeaponPresset()).Init("colt1911_base", "Colt1911 'Classic'", "Colt1911", "Mag_1911_7Rnd", 3, 0));
		result.Insert((new DmWeaponPresset()).Init("colt1911_eng", "Colt1911 'Engraved'", "Engraved1911", "Mag_1911_7Rnd", 3, 100));
		result.Insert((new DmWeaponPresset()).Init("glock", "Glock19", "Glock19", "Mag_Glock_15Rnd", 4, 0).Atth("FNP45_MRDSOptic"));
		result.Insert((new DmWeaponPresset()).Init("lhorn_base", "Longhorn 'Handcannon'", "Longhorn", "Ammo_308WinTracer", 5, 0).Atth("FNP45_MRDSOptic"));
		result.Insert((new DmWeaponPresset()).Init("lhorn_opt", "Longhorn 'Optics'", "Longhorn", "Ammo_308WinTracer", 5, 250).Atth("PistolOptic"));

		result.Insert((new DmWeaponPresset()).Init("mossin_base", "Mosin9130 'Classic'", "Mosin9130", "Ammo_762x54", 6, 0).Atth("Mosin_Compensator"));
		result.Insert((new DmWeaponPresset()).Init("mossin_mark", "Mosin9130 'Optic'", "Mosin9130", "Ammo_762x54Tracer", 6, 300).Atth("Mosin_Compensator").Atth("PUScopeOptic"));
		result.Insert((new DmWeaponPresset()).Init("winfild", "Winchester", "Repeater", "Ammo_357", 7, 0));
		result.Insert((new DmWeaponPresset()).Init("mp133_base", "Mp133 'Shotgun'", "Mp133Shotgun", "Ammo_12gaPellets", 8, 0).Atth("FNP45_MRDSOptic"));
		result.Insert((new DmWeaponPresset()).Init("sks_base", "SKS 'Rifle'", "SKS", "Ammo_762x39", 9, 0));

		result.Insert((new DmWeaponPresset()).Init("pp19_spec", "PP19 'Bizon'", "PP19", "Mag_PP19_64Rnd", 10, 300).Atth("PP19_Bttstck").Atth("PistolSuppressor").Atth("KobraOptic"));				
		result.Insert((new DmWeaponPresset()).Init("mp5_base", "MP5 'Assault'", "MP5K", "Mag_MP5_30Rnd", 11, 500).Atth("MP5k_StockBttstck").Atth("MP5_RailHndgrd").Atth("PistolSuppressor").Atth("ReflexOptic"));
		result.Insert((new DmWeaponPresset()).Init("mp5_mark", "MP5 'Marksman'", "MP5K", "Mag_MP5_30Rnd", 11, 800).Atth("MP5k_StockBttstck").Atth("MP5_RailHndgrd").Atth("PistolSuppressor").Atth("ACOGOptic"));
		
		result.Insert((new DmWeaponPresset()).Init("m16_base", "M16A2", "M16A2", "Mag_STANAG_30Rnd", 12, 0));	
		result.Insert((new DmWeaponPresset()).Init("famas_base", "FAMAS", "FAMAS", "Mag_FAMAS_25Rnd", 13, 0));

		result.Insert((new DmWeaponPresset()).Init("ak74_base",  "AK-74 'Classic'",   "AK74",  "Mag_AK74_30Rnd",   14, 0).Atth("AK_WoodBttstck").Atth("AK_WoodHndgrd"));		
		result.Insert((new DmWeaponPresset()).Init("ak101_base", "AK-101 'Classic'",  "AK101", "Mag_AK101_30Rnd",  15, 0).Atth("AK_PlasticHndgrd").Atth("AK_PlasticBttstck"));
		result.Insert((new DmWeaponPresset()).Init("ak101_tact", "AK-101 'Tactical'", "AK101", "Mag_AK101_30Rnd",  15, 1000).Atth("AK_RailHndgrd").Atth("AKS74U_Bttstck").Atth("AK_Suppressor").Atth("KobraOptic"));
		result.Insert((new DmWeaponPresset()).Init("akm_base",   "AKM 'Classic'",     "AKM",   "Mag_AKM_30Rnd",    16, 0).Atth("AK74_Hndgrd").Atth("AK74_WoodBttstck"));
		result.Insert((new DmWeaponPresset()).Init("akm_mark",   "AKM 'Marksman'",    "AKM",   "Mag_AKM_30Rnd",    17, 800).Atth("AK74_Hndgrd").Atth("AK74_WoodBttstck").Atth("PSO1Optic"));
		result.Insert((new DmWeaponPresset()).Init("akm_tact",   "AKM 'Tactical'",    "AKM",   "Mag_AKM_Palm30Rnd",18, 1200).Atth("AK_RailHndgrd").Atth("AK_WoodBttstck_Black").Atth("AK_Suppressor"));
		result.Insert((new DmWeaponPresset()).Init("akm_mgun",   "AKM 'Machinegun'",  "AKM",   "Mag_AKM_Drum75Rnd",19, 2500).Atth("AK_RailHndgrd").Atth("AK_FoldingBttstck").Atth("KobraOptic"));
		result.Insert((new DmWeaponPresset()).Init("aksu_base", "AK74U 'Silence'", "AKS74U_Black", "Mag_AK74_30Rnd", 20, 0).Atth("AKS74U_Bttstck").Atth("AK_Suppressor"));
	
		
		result.Insert((new DmWeaponPresset()).Init("aug_base", "AUG 'Classic'", "AugShort", "Mag_Aug_30Rnd", 21, 0));
		result.Insert((new DmWeaponPresset()).Init("aug_spec", "AUG 'Tactical'", "Aug", "Mag_Aug_30Rnd", 22, 1200).Atth("M68Optic").Atth("M4_Suppressor"));
		result.Insert((new DmWeaponPresset()).Init("aug_mgun", "AUG 'Machinegun'", "Aug", "Mag_STANAG_60Rnd", 23, 2500).Atth("ACOGOptic_6x"));
		
		result.Insert((new DmWeaponPresset()).Init("fal_base", "FAL 'Classic'", "FAL", "Mag_FAL_20Rnd", 24, 500).Atth("Fal_FoldingBttstck").Atth("M4_T3NRDSOptic"));
		result.Insert((new DmWeaponPresset()).Init("fal_mark", "FAL 'Marksman'", "FAL", "Mag_FAL_20Rnd", 25, 3000).Atth("Fal_OeBttstck").Atth("ACOGOptic_6x"));
		
		result.Insert((new DmWeaponPresset()).Init("blaze_base", "Blaze 'Classic'", "B95", "Ammo_308Win", 26, 0));
		result.Insert((new DmWeaponPresset()).Init("blaze_snip", "Blaze 'Sniper'", "B95", "Ammo_308WinTracer", 26, 300).Atth("HuntingOptic"));
		result.Insert((new DmWeaponPresset()).Init("scout_tact", "Scout 'Tactical'", "Scout", "Mag_Scout_5Rnd", 27, 1000).Atth("ACOGOptic_6x").Atth("M4_Suppressor"));
		result.Insert((new DmWeaponPresset()).Init("cr527_base", "CR527 'Rifle'", "CZ527", "Mag_CZ527_5rnd", 28, 0));
		result.Insert((new DmWeaponPresset()).Init("cr527_snip", "CR527 'Sniper'", "CZ527_Black", "Mag_CZ527_5rnd", 28, 300).Atth("HuntingOptic"));
		result.Insert((new DmWeaponPresset()).Init("cr550_snip", "CR550 'Savanna'", "CZ550", "Mag_CZ550_10Rnd", 29, 600).Atth("HuntingOptic"));
		
		result.Insert((new DmWeaponPresset()).Init("m4_base", "M4A1 'Classic'", "M4A1", "Mag_STANAG_30Rnd", 30, 0).Atth("M4_PlasticHndgrd").Atth("M4_CarryHandleOptic").Atth("M4_OEBttstck"));	
		result.Insert((new DmWeaponPresset()).Init("m4_tact", "M4A1 'Tactical'", "M4A1", "Mag_STANAG_30Rnd", 30, 1500).Atth("M4_RISHndgrd").Atth("M4_T3NRDSOptic").Atth("M4_MPBttstck").Atth("M4_Suppressor"));	
		result.Insert((new DmWeaponPresset()).Init("m4_mark", "M4A1 'Marksman'", "M4A1", "Mag_STANAG_30Rnd", 30, 2500).Atth("M4_MPHndgrd").Atth("ACOGOptic").Atth("M4_MPBttstck").Atth("M4_Suppressor"));
		result.Insert((new DmWeaponPresset()).Init("m4_mgun", "M4A1 'Spam'", "M4A1", "Mag_STANAG_60Rnd", 30, 3000).Atth("M4_PlasticHndgrd").Atth("ReflexOptic").Atth("M4_CQBBttstck"));	
		result.Insert((new DmWeaponPresset()).Init("m70_snip", "M70 'Tundra'", "Winchester70", "Ammo_308WinTracer", 30, 1000).Atth("HuntingOptic"));
		
		result.Insert((new DmWeaponPresset()).Init("deagle_hitman", "Deagle 'Hitman'", "Deagle", "Mag_Deagle_9rnd", 31, 0).Atth("PistolSuppressor"));
		result.Insert((new DmWeaponPresset()).Init("deagle_cannon", "Deagle 'Handcannon'", "Deagle", "Mag_Deagle_9rnd", 32, 0).Atth("PistolOptic"));
		
		result.Insert((new DmWeaponPresset()).Init("saiga_base", "Saiga 'Classic'", "Saiga", "Mag_Saiga_5Rnd", 35, 0).Atth("Saiga_Bttstck"));
		result.Insert((new DmWeaponPresset()).Init("saiga_drum", "Saiga 'Drum'", "Saiga", "Mag_Saiga_Drum20Rnd", 35, 3000).Atth("Saiga_Bttstck").Atth("KobraOptic"));
		
		result.Insert((new DmWeaponPresset()).Init("vss_base", "VSS 'Vintorez'", "VSS", "Mag_VSS_10Rnd", 38, 800).Atth("PSO11Optic"));
		result.Insert((new DmWeaponPresset()).Init("asval_auto", "AS-VAL 'Sturmer'", "ASVAL", "Mag_VAL_20Rnd", 40, 1500).Atth("M4_T3NRDSOptic"));
		result.Insert((new DmWeaponPresset()).Init("asval_mark", "AS-VAL 'Marksman'", "ASVAL", "Mag_VSS_10Rnd", 40, 3000).Atth("ACOGOptic"));
		
		result.Insert((new DmWeaponPresset()).Init("svd_base", "SVD 'Classic'", "SVD", "Mag_SVD_10Rnd", 45, 0).Atth("PSO1Optic"));
		result.Insert((new DmWeaponPresset()).Init("svd_tact", "SVD 'Tactical'", "SVD", "Mag_SVD_10Rnd", 45, 5000).Atth("PSO1Optic").Atth("AK_Suppressor").Atth("GhillieAtt_Mossy"));
		
		result.Insert((new DmWeaponPresset()).Init("deagle_gold", "Deagle 'Singleshot'", "Deagle_Gold", "Ammo_357", 50, 0));
		result.Insert((new DmWeaponPresset()).Init("m79_base", "M79 'Launcher'", "M79", "Ammo_40mm_Explosive", 50, 10000));
	}
};