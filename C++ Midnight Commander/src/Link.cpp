#include "Link.hpp"
//---------------------------------------------------------------------------------------------------------------------------
Link::Link( const std::string & path , const std::string & name , const std::string & ptr_to_path )
	: Item(path,name) , ptr_to_path(ptr_to_path){}
//---------------------------------------------------------------------------------------------------------------------------
bool Link::Delete()const//deletes just the link,not the target
{
	std::stringstream ss;
	ss << "rm -f " << path;
	if( std::system( ss.str().c_str() ) )
		return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Link::Copy( const std::string & user_input , const std::string & current_folder )const
{
	std::stringstream ss;
	if( user_input.front() == '/' )
		ss << "ln -s " << ptr_to_path << " " << user_input;
	else
		ss << "ln -s " << ptr_to_path << " " << current_folder << "/" << user_input;
	if( user_input.back() == '/' )
		ss << name;
	if( std::system( ss.str().c_str() ) )
		return false;
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Link::Move( const std::string & user_input , const std::string & current_folder )const
{
	if( !Copy( user_input , current_folder ) || !Delete() )
		return false;
	return true;
}