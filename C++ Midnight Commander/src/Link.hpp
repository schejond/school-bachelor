#pragma once
#include "Item.hpp"
/**
 * @brief Object of type Item representing symbolic Link
 */
class Link : public Item
{
public:
	Link( const std::string & path , const std::string & name , const std::string & ptr_to_path );
	/**
	 * @brief Methods from Item customized for Link
	 */
	bool Delete()const;
	bool Move( const std::string & user_input , const std::string & current_folder )const;
	bool Copy( const std::string & user_input , const std::string & current_folder )const;
private:
	std::string ptr_to_path;
};