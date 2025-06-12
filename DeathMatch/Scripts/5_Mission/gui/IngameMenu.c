modded class InGameMenu
{
	override void OnClick_Respawn()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && player.m_dmConnectSyncCtx && !player.m_dmConnectSyncCtx.m_allowManualRespawn)
		{
			return;
		}
		
		super.OnClick_Respawn();
	};
	
	override void GameRespawn(bool random)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && player.m_dmConnectSyncCtx && !player.m_dmConnectSyncCtx.m_allowManualRespawn)
		{
			return;
		}
		
		super.GameRespawn(random);
	};
};