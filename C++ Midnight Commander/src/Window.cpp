#include "Window.hpp"
#include "Popup_Decision.hpp"
#include "Check_Regex_Syntax.hpp"
#include "Input_Checks.hpp"
//---------------------------------------------------------------------------------------------------------------------------
Window::Window()
	: current_folder_path( getenv("HOME") ) , selected_line(0) {}
//---------------------------------------------------------------------------------------------------------------------------
bool Window::Initialize()
{
	wrapper.Fill_Wrapper( current_folder_path );
	if( !design.Check_Layout_Size() )
	{
		getch();
		wrapper.Delete_Wrapped_Items();
		design.Close();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------
void Window::Catch_Orders()
{
	int ch;	
	design.Paint_Layout( current_folder_path , selected_line , wrapper.Get_Size() );
	std::string user_input;

	while( ( ch = getch() ) != KEY_F(12)  )
	{
		user_input = "";
		switch(ch)
		{
			case KEY_F(2)://MOVE
			{
				if( !Root_Check( wrapper.Get_Name( selected_line ) , design ) )
					break;

				design.Paint_Popup( Popup_Type::MOVE , user_input , wrapper.Get_Item( selected_line ) );
				
				if( Popup_Decision() )
				{
					if( !Empty_Or_Not_Existing_File( user_input , design ) )
						break;

					if( !( wrapper.Get_Item( selected_line )->Move( user_input , current_folder_path ) ) )
						design.Paint_Error_Message("Couldn't fulfill your order");
				}
				break;
			}
			case KEY_F(3)://COPY
			{
				if( !Root_Check( wrapper.Get_Name( selected_line ) , design ) )
					break;

				design.Paint_Popup( Popup_Type::COPY , user_input , wrapper.Get_Item( selected_line ) );
				
				if( Popup_Decision() )
				{
					if( !Empty_Or_Not_Existing_File( user_input , design ) )
						break;

					if( !( wrapper.Get_Item( selected_line )->Copy( user_input , current_folder_path ) ) )
						design.Paint_Error_Message("Couldn't fulfill your order");
				}
				break;	
			}
			case KEY_F(4)://DELETE
			{
				if( !Root_Check( wrapper.Get_Name( selected_line ) , design ) )
					break;

				design.Paint_Popup( Popup_Type::DELETE , user_input , wrapper.Get_Item( selected_line ) );

				if( Popup_Decision() )
					if( !( wrapper.Get_Item( selected_line )->Delete() ) )
						design.Paint_Error_Message("Couldn't fulfill your order");
				break;
			}
			case KEY_F(5)://MAKE LINK
			{
				design.Paint_Popup( Popup_Type::MAKE_LINK , user_input , wrapper.Get_Item( selected_line ) );//obtains 2 string from user and connects them into one for better handling
				if( Popup_Decision() )
				{
					std::string target;
					std::string name_for_link;
					std::string::iterator it = user_input.begin();
					while( *it != '#' )
					{
						name_for_link.push_back( *it );
						it++;
					}
					it++;
					while( it != user_input.end() )
					{
						target.push_back( *it );
						it++;
					}

					if( !Empty_Or_Not_Existing_File( target , design ) || !Empty_Or_Not_Existing_File( name_for_link , design ) )
						break;

					std::stringstream ss;
					ss << "ln -s ";
					if( target.front() != '/' )
						ss << current_folder_path << "/" << target;
					else
						ss << target;
					ss << " ";
					if( name_for_link.front() != '/' )
						ss << current_folder_path << "/" << name_for_link;
					else
						ss << name_for_link;
					std::system( ss.str().c_str() );
				}
				break;
			}
			case KEY_F(6)://CREATE FOLDER
			{
				design.Paint_Popup( Popup_Type::CREATE_FOLDER , user_input , wrapper.Get_Item( selected_line ) );
				if( Popup_Decision() )
				{
					if( !Empty_Or_Not_Existing_File( user_input , design ) )
						break;

					std::stringstream ss;
					ss << "mkdir -p ";
					if( user_input.front() == '/' )
						ss << user_input;
					else
						ss << current_folder_path << "/" << user_input; 
					std::system( ss.str().c_str() );
				}
				break;
			}
			case KEY_F(9)://SEARCH
			{
				design.Paint_Popup( Popup_Type::SEARCH , user_input , wrapper.Get_Item( selected_line ) );
				if( Popup_Decision() )
				{
					if( Check_Regex_Syntax( user_input ) && !user_input.empty() )
					{
						search_results_wrapper.Fill_Wrapper( current_folder_path , user_input );//fill wrapper with files according to regex
						std::string command = std::to_string( search_results_wrapper.Get_Size() );
						design.Paint_Popup( Popup_Type::GROUP , command , wrapper.Get_Item( selected_line ) );
						switch( command.back() )
						{
							case 'C':
							{
								command.pop_back();
								bool commit = false;
								if( (commit = Popup_Decision() ) && (command.back() != '/' || ( command.front() == '/' && !Check_Path_Correct_Syntax(command) ) ) )
								{
									design.Paint_Error_Message("Not existing path");
									break;
								}
								if( commit )
									search_results_wrapper.Copy_Permanently( command , current_folder_path );
									
								break;
							}
							case 'M':
							{
								command.pop_back();
								bool commit = false;
								if( ( commit = Popup_Decision() ) && ( command.back() != '/' || ( command.front() == '/' && !Check_Path_Correct_Syntax(command) ) ) )
								{
									design.Paint_Error_Message("Not existing path");
									break;
								}
								if( commit )
									search_results_wrapper.Move_Permanently( command , current_folder_path );
									
								break;
							}
							case 'D':
							{
								if( Popup_Decision() )
									search_results_wrapper.Delete_Permanently();
								break;
							}
							default:
								break;
						}
						search_results_wrapper.Delete_Wrapped_Items();
					}
					else
						design.Paint_Error_Message("Wrong regex");
				}
				break;
			}
			case KEY_F(7)://CREATE FILE
			{
				design.Paint_Popup( Popup_Type::CREATE_FILE , user_input , wrapper.Get_Item( selected_line ) );
				if( Popup_Decision() )
				{
					if( !Empty_Or_Not_Existing_File( user_input , design ) )
						break;
					std::stringstream ss;
					ss << "touch ";
					if( user_input.front() == '/' )
						ss << user_input;
					else
						ss << current_folder_path << "/" << user_input; 
					std::system( ss.str().c_str() );
				}
				break;
			}
			case KEY_DOWN:
				if( selected_line != ( wrapper.Get_Size() - 1 ) )
					selected_line++;
				break;
			case KEY_UP:
				if( selected_line != 0 )
					selected_line--;
				break;
			case 10: // representing KEY_ENTER
				if( wrapper.Is_Folder( selected_line ) )
				{
					if( wrapper.Get_Name( selected_line ) == "." )
						break;
					if( wrapper.Get_Name( selected_line ) == ".." )
					{
						if( current_folder_path == "/" )
							break;
						while( current_folder_path.back() != '/' )
						{
							current_folder_path.pop_back();
						}
						if( current_folder_path != "/" )
							current_folder_path.pop_back();
						selected_line = 0;
						break;
					}
					//checking for permissions
					std::string try_access = current_folder_path;
					if( try_access.back() != '/' )
						try_access.push_back('/');
					try_access.append( wrapper.Get_Name( selected_line ) );
					std::stringstream ss;
					ss << "cd " << try_access;
					if( std::system(ss.str().c_str()) )
					{
						design.Paint_Error_Message("No Permission");
						break;
					}
					current_folder_path = try_access;
					selected_line = 0;
				}
				break;
			default:
				break;
		}
		wrapper.Delete_Wrapped_Items();
		wrapper.Fill_Wrapper( current_folder_path );

		while( selected_line > ( wrapper.Get_Size() - 1 ) )//to always correctly display selected line
		{
			selected_line--;
		}
		design.Paint_Layout( current_folder_path , selected_line , wrapper.Get_Size() );
	}

	wrapper.Delete_Wrapped_Items();
	design.Close();
}