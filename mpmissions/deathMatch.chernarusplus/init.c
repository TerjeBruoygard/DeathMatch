void main()
{
	GetGame().GetWorld().SetDate(2022, 9, 17, 12, 0);
};

Mission CreateCustomMission(string path)
{
	return new MissionServer();
};