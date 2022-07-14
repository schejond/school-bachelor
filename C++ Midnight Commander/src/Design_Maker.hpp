#pragma once
#include <ncurses.h>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include "Popup_Type.hpp"
#include "Item.hpp"
/**
 * @brief Object managing visual representation of the program, contains Methods that are used for Making graphic visualization of all sort of things needed for our program
 */
class Design_Maker
{
public:
	Design_Maker();
	void Close()const;
	/**
 	 * @brief Method checks if the current terminal size is big enough to display the program correctly
 	 */
	bool Check_Layout_Size()const;

	void Paint_Layout( const std::string & folder_path , const int position , const int files_count )const;
	void Paint_Error_Message( const std::string & message = "" )const;
	/**
 	 * @brief Method displays obtained order and dialog that is neccesary for finishing the order
 	 */
	void Paint_Popup( const Popup_Type choice , std::string & answer , const Item * file )const;
private:
	WINDOW * window_ptr = nullptr;
	WINDOW * folder_ptr = nullptr;

	void Paint_Up_Arrow()const;
	void Paint_Down_Arrow()const;
	void Paint_Yes_No( const int starty , const int startx )const;
	void Catch_Path( std::string & answer , int starty , int startx , WINDOW *popup_win )const;
};