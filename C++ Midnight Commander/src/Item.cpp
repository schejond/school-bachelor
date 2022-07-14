#include "Item.hpp"
//---------------------------------------------------------------------------------------------------------------------------
Item::Item( const std::string & path , const std::string & name )
	: path(path) , name(name) {}
//---------------------------------------------------------------------------------------------------------------------------
std::string Item::Get_Path()const
{
	return path;
}
//---------------------------------------------------------------------------------------------------------------------------
std::string Item::Get_Name()const
{
	return name;
}
//---------------------------------------------------------------------------------------------------------------------------
bool Item::Is_Folder()const
{
	return false;
}