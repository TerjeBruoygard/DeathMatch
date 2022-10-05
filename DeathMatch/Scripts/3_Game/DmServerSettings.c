class DM_ServerSettings
{
	ref array<ref DM_AreaTrail> m_trails = new array<ref DM_AreaTrail>;
	float m_expandStep = 10;
	float m_healthRegen = 1.0;
	float m_healthOutOfZoneDmg = 0.1;
	float m_healthRegenTimeoutOnHit = 10;
	int m_startMoney = 100;
	int m_moneyPerKill = 10;
	float m_expIncPerKill = 0.2;
	float m_expDecPerDeath = 0.1;
	float m_areaMoveSpeed = 0.2;
	int m_databaseSaveTime = 600;
	int m_databaseWipePlayerTimeout = 1209600; // 14 days
	int m_leaderBoardTopSize = 20;
	int m_lockWorldTime = 1;
	float m_deadBodyDespawnTime = 5;
	int m_clearDroppedItemsOnGround = 1;
	
	void InitDefaultTrails()
	{
		ref DM_AreaTrail trail = new DM_AreaTrail;
		trail.m_points.Insert("5281.88 0 9798.75");
		trail.m_points.Insert("5048.44 0 9757.50");
		trail.m_points.Insert("4755.00 0 10384.69");
		trail.m_points.Insert("4504.69 0 10818.75");
		trail.m_points.Insert("4153.13 0 11025.00");
		trail.m_points.Insert("4020.94 0 11762.81");
		trail.m_points.Insert("4153.13 0 11025.00");
		trail.m_points.Insert("4504.69 0 10818.75");
		trail.m_points.Insert("4755.00 0 10384.69");
		trail.m_points.Insert("5048.44 0 9757.50");
		m_trails.Insert(trail);
	}
};

class DM_AreaTrail
{
	ref array<vector> m_points = new array<vector>;
	float m_minRadius = 120;
	float m_maxRadius = 1500;
};