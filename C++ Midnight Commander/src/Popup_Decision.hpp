/**
 * @brief Function to catch confirmation of order
 */
bool Popup_Decision()
{
	while( 1 )
	{
		int ch = getch();
		if( ch == 'y' || ch == 'Y' )
			return true;
		else if( ch == 'n' || ch == 'N' )
			return false;
	}
}