#pragma once
#include "Design_Maker.hpp"
#include "Files_Wrapper.hpp"
/**
 * @brief Object representing Main Screen, catches orders from the use
 */
class Window
{
public:
	Window();
	/**
	 * @brief Checks if the screen is big enough for the program to view correctly and prepares it's class members for the Catch_Orders method,where it catches users orders and commits them
		@return true if terminal size is ok
	 */
	bool Initialize();
	void Catch_Orders();

private:
	Design_Maker design;
	std::string current_folder_path;
	size_t selected_line;
	Files_Wrapper wrapper;
	Files_Wrapper search_results_wrapper;
};