#pragma once
#include "Item.hpp"
/**
 * @brief Object of type Item representing Folder
 */
class Folder : public Item
{
public:
	Folder( const std::string & path , const std::string & name );
	/**
	 * @brief Methods from Item customized for Folder
	 */
	bool Is_Folder()const;
	bool Move( const std::string & user_input , const std::string & current_folder )const;
	bool Copy( const std::string & user_input , const std::string & current_folder )const;
	bool Delete()const;

private:
};