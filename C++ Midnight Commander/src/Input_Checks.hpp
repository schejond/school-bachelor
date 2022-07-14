/**
 * @brief Various functions each checking different type of wrong entrys
 */
bool Root_Check( const std::string & name , const Design_Maker & design )
{
	if( name == "." || name == ".." )
	{
		design.Paint_Error_Message("Bad target");
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Check_Path_Correct_Syntax( const std::string & user_input )
{
	std::string tmp = user_input;
	while( tmp.back() != '/' )
	{
		tmp.pop_back();
	}

	struct stat info;

	if( stat( tmp.c_str() , &info ) != 0 )
    	return false;
    return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Empty_Or_Not_Existing_File( const std::string & name , const Design_Maker & design )
{
	if( name.empty() )
	{
		design.Paint_Error_Message("You did not enter anything");
		return false;
	}
	
	if( name.front() == '/' && !Check_Path_Correct_Syntax(name) )
	{	
		design.Paint_Error_Message("Not existing path");
		return false;
	}
	return true;
}