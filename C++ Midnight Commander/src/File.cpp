#include "File.hpp"
//---------------------------------------------------------------------------------------------------------------------------
File::File( const std::string & path , const std::string & name )
	: Item(path,name) {}
//---------------------------------------------------------------------------------------------------------------------------
bool File::Delete()const
{
	std::stringstream ss;
	ss << "rm -f " << path;
	if( std::system( ss.str().c_str() ) )
		return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool File::Copy( const std::string & user_input , const std::string & current_folder )const
{
	std::stringstream ss;
	if( user_input.front() == '/' )
		ss << "cp " << path << " " << user_input;
	else
		ss << "cp " << path << " " << current_folder << "/" << user_input;
	if( std::system( ss.str().c_str() ) )
		return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool File::Move( const std::string & user_input , const std::string & current_folder )const
{
	std::stringstream ss;
	if( user_input.front() == '/' )
		ss << "mv " << path << " " << user_input;
	else
		ss << "mv " << path << " " << current_folder << "/" << user_input;
	if( std::system( ss.str().c_str() ) )
		return false;
	return true;
}