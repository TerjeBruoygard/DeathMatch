modded class ChatInputMenu
{
	private const string TERJE_DBG_COMMAND = "/dm ";
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (finished)
		{
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			string text = m_edit_box.GetText();
			if ((text.IndexOf(TERJE_DBG_COMMAND) == 0) && player)
			{
				string cmd = text.Substring(TERJE_DBG_COMMAND.Length(), text.Length() - TERJE_DBG_COMMAND.Length());
				GetRPCManager().SendRPC("DM", "DM_AdminCommand", new Param1<string>(cmd), true, null, player); 
				m_edit_box.SetText("");
				m_close_timer.Run(0.1, this, "Close");
				return true;
			}
		}
		
		return super.OnChange(w, x, y, finished);
	}
}