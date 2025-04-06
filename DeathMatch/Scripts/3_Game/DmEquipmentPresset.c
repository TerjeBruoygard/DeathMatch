class DmEquipmentPresset
{
	string m_Id;
	string m_Name;
	ref array<string> m_Items = new array<string>;
	int m_Level;
	int m_Price;
	
	ref DmEquipmentPresset Init(string id, string name, int level, int price)
	{
		m_Id = id;
		m_Name = name;
		m_Level = level;
		m_Price = price;
		return this;
	}
	
	ref DmEquipmentPresset Atth(string classname)
	{
		m_Items.Insert(classname);
		return this;
	}
	
	static void CreateDefaultPressets(array<ref DmEquipmentPresset> result)
	{
		result.Insert((new DmEquipmentPresset()).Init("prison", "Prisoner", 0, 0).Atth("PrisonerCap").Atth("PrisonUniformJacket").Atth("PrisonUniformPants").Atth("AthleticShoes_Black"));
		result.Insert((new DmEquipmentPresset()).Init("farmer", "Farmer", 2, 50).Atth("LeatherHat_Brown").Atth("LeatherJacket_Brown").Atth("CanvasPants_Beige").Atth("Wellies_Black"));
		result.Insert((new DmEquipmentPresset()).Init("worker", "Worker", 3, 100).Atth("FlatCap_BrownCheck").Atth("BomberJacket_SkyBlue").Atth("Jeans_Brown").Atth("WorkingGloves_Yellow").Atth("WorkingBoots_Grey"));
		result.Insert((new DmEquipmentPresset()).Init("tramp", "Tramp", 4, 150).Atth("FlatCap_Blue").Atth("TrackSuitJacket_Black").Atth("TrackSuitPants_Black").Atth("Sneakers_Black"));
		result.Insert((new DmEquipmentPresset()).Init("medic", "Medic", 5, 150).Atth("MedicalScrubsHat_Blue").Atth("MedicalScrubsShirt_Blue").Atth("MedicalScrubsPants_Blue").Atth("DressShoes_Beige").Atth("SurgicalGloves_Green"));
		result.Insert((new DmEquipmentPresset()).Init("cowboy", "Cowboy", 10, 200).Atth("CowboyHat_Brown").Atth("DenimJacket").Atth("Jeans_BlueDark").Atth("CombatBoots_Beige"));
		result.Insert((new DmEquipmentPresset()).Init("sherif", "Sheriff", 15, 300).Atth("CowboyHat_black").Atth("M65Jacket_Black").Atth("CargoPants_Grey").Atth("CombatBoots_Black").Atth("PoliceVest").Atth("WorkingGloves_Black"));
		
		result.Insert((new DmEquipmentPresset()).Init("sold_a", "Soldier I", 20, 300).Atth("PilotkaCap").Atth("WoolCoat_Green").Atth("CargoPants_Green").Atth("CombatBoots_Black"));
		result.Insert((new DmEquipmentPresset()).Init("sold_b", "Soldier II", 25, 500).Atth("Ssh68Helmet").Atth("TTsKOJacket_Camo").Atth("TTSKOPants").Atth("TTSKOBoots").Atth("AssaultBag_Ttsko").Atth("WorkingGloves_Black"));
		result.Insert((new DmEquipmentPresset()).Init("sold_c", "Soldier III", 30, 1000).Atth("GorkaHelmet").Atth("GorkaHelmetVisor").Atth("GorkaEJacket_Flat").Atth("GorkaPants_Flat").Atth("TTSKOBoots").Atth("SmershVest").Atth("WorkingGloves_Black"));
		result.Insert((new DmEquipmentPresset()).Init("sold_d", "Soldier IV", 35, 3000).Atth("PlateCarrierVest").Atth("PlateCarrierPouches").Atth("Mich2001Helmet").Atth("BalaclavaMask_Blackskull").Atth("BDUJacket").Atth("BDUPants").Atth("CombatBoots_Black").Atth("WorkingGloves_Black"));
		
		result.Insert((new DmEquipmentPresset()).Init("ghill_a", "Ghillie Mossy", 40, 5000).Atth("GhillieHood_Mossy").Atth("GhillieSuit_Mossy").Atth("GorkaPants_Autumn").Atth("GorkaEJacket_Autumn").Atth("Sneakers_Black"));
		result.Insert((new DmEquipmentPresset()).Init("ghill_b", "Ghillie Woodland", 40, 5000).Atth("GhillieHood_Woodland").Atth("GhillieSuit_Woodland").Atth("GorkaPants_PautRev").Atth("GorkaEJacket_PautRev").Atth("Sneakers_Black"));
		result.Insert((new DmEquipmentPresset()).Init("ghill_c", "Ghillie Tan", 40, 5000).Atth("GhillieHood_Tan").Atth("GhillieSuit_Tan").Atth("GorkaPants_Summer").Atth("GorkaEJacket_Summer").Atth("Sneakers_Black"));
		
		result.Insert((new DmEquipmentPresset()).Init("santa", "Santa", 0, 0).Atth("SantasBeard").Atth("SantasHat").Atth("QuiltedJacket_Red").Atth("JumpsuitPants_Red").Atth("CombatBoots_Black"));
		result.Insert((new DmEquipmentPresset()).Init("witch", "Witch", 50, 8000).Atth("WitchHat").Atth("WorkingGloves_Black").Atth("MiniDress_RedChecker").Atth("ShortJeans_Black").Atth("DressShoes_Black"));
		result.Insert((new DmEquipmentPresset()).Init("jack", "Jack", 50, 8000).Atth("PumpkinHelmet").Atth("FirefighterJacket_Beige").Atth("FirefightersPants_Beige").Atth("LeatherGloves_Beige").Atth("CombatBoots_Black"));
	}
};