modded class IngameHud
{
	override void InitBadgesAndNotifiers()
	{
		super.InitBadgesAndNotifiers();
		m_HudPanelWidget.FindAnyWidget("NotifierDivider").Show(false);
		m_HudPanelWidget.FindAnyWidget("BadgeNotifierDivider").Show(false);
		
		int i = 0;
		int key = 0;
		for ( i = 0; i < m_StatesWidgetNames.Count(); i++ )
		{
			string widget_name = m_StatesWidgetNames.GetElement(i);
			key = m_StatesWidgetNames.GetKey(i);			
			ImageWidget w;
			Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( "Icon" + widget_name ) ) );

			w.SetAlpha(0);
			w.SetColor( ARGB( 0, 0, 0, 0 ) );
			
			for ( int x = 1; x < 4; x++ )
			{
				Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( widget_name + "ArrowUp" + x.ToString() ) ) );
				w.SetAlpha(0);
				w.SetColor( ARGB( 0, 0, 0, 0 ) );
				
				Class.CastTo(w,  m_Notifiers.FindAnyWidget( String( widget_name + "ArrowDown" + x.ToString() ) ) );
				w.SetAlpha(0);
				w.SetColor( ARGB( 0, 0, 0, 0 ) );
			}
		}
		
		for ( i = 0; i < m_BadgesWidgetNames.Count(); i++ )
		{
			string badge_name = m_BadgesWidgetNames.GetElement(  i);
			key = m_BadgesWidgetNames.GetKey( i );
			ImageWidget badge_widget;
			Class.CastTo(badge_widget,  m_Badges.FindAnyWidget( badge_name ) );
			badge_widget.SetAlpha(0);
			badge_widget.SetColor( ARGB( 0, 0, 0, 0 ) );
		}
	};
};