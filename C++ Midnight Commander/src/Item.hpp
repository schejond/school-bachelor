#pragma once
#include <string>
#include <sstream>
/**
 * @brief Object representing actual objects in computer, holds path to the Itema and it's name
 */
class Item
{
public:
	Item( const std::string & path , const std::string & name );
	virtual ~Item() {}
	/**
 	* @brief methods that need to be implemented according to specific type of the Item
	  @return whether operation was successful
 	*/
	virtual bool Delete()const = 0;
	virtual bool Move( const std::string & user_input , const std::string & current_folder )const = 0;
	virtual bool Copy( const std::string & user_input , const std::string & current_folder )const = 0;
	virtual bool Is_Folder()const;

	std::string Get_Path()const;
	std::string Get_Name()const;
protected:
	std::string path;
	std::string name;
};