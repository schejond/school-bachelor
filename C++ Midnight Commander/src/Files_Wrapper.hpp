#pragma once
#include <vector>
#include <dirent.h>
#include <ncurses.h>
#include <regex>
#include <sys/stat.h>
#include "Item.hpp"
#include "File.hpp"
#include "Folder.hpp"
#include "Link.hpp"
/**
 * @brief Object representing a group of files. Holds information and data types of all files in "group"
 */
class Files_Wrapper
{
public:
	/**
 	 * @brief Methods focussed to obtain or delete information about files
 	 */
	void Fill_Wrapper( const std::string & folder_path , const std::string & input_regex = "" );
	void Delete_Wrapped_Items();
	
	bool Is_Folder( const int position )const;
	std::string Get_Name( const int position )const;
	size_t Get_Size()const;
	Item * Get_Item( const int position )const;
	/**
 	 * @brief Methods that call specific order on each member of "group"
 	 */
	void Move_Permanently( const std::string & command , const std::string & current_folder_path )const;
	void Copy_Permanently( const std::string & command , const std::string & current_folder_path )const;
	void Delete_Permanently()const;

private:
	std::vector<Item*> files;
};