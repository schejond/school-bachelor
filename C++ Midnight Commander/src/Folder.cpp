#include "Folder.hpp"
//---------------------------------------------------------------------------------------------------------------------------
Folder::Folder( const std::string & path , const std::string & name )
	: Item(path,name) {}
//---------------------------------------------------------------------------------------------------------------------------
bool Folder::Delete()const
{
	std::stringstream ss;
	ss << "rm -rf " << path;
	if( std::system( ss.str().c_str() ) )
		return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Folder::Is_Folder()const
{
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Folder::Copy( const std::string & user_input , const std::string & current_folder )const
{
	std::stringstream ss;
	if( user_input.front() == '/' )
	{
		ss << "cp " << path << " " << user_input << " -r";
	}
	else
	{
		ss << "cp " << path << " " << current_folder << "/" << user_input << " -r";
	}
	if( std::system( ss.str().c_str() ) )
		return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Folder::Move( const std::string & user_input , const std::string & current_folder )const
{
	if( !Copy( user_input , current_folder ) || !Delete() )
		return false;
	return true;
}