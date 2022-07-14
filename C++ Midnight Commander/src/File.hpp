#include "Item.hpp"
/**
 * @brief Object of type Item representing basic File
 */
class File : public Item
{
public:
	File( const std::string & path , const std::string & name );
	/**
	 * @brief Methods from Item customized for Folder
	 */
	bool Move( const std::string & user_input , const std::string & current_folder )const;
	bool Copy( const std::string & user_input , const std::string & current_folder )const;
	bool Delete()const;

private:
};