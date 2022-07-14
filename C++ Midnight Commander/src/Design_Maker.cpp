#include "Design_Maker.hpp"
//---------------------------------------------------------------------------------------------------------------------------
Design_Maker::Design_Maker()
{
	initscr();
	
	window_ptr = newwin(LINES,COLS,0,0);
	folder_ptr = newwin(21,COLS-4,5,2);

	start_color();
	init_pair( 1 , COLOR_WHITE, COLOR_BLUE );//normal files + background
	init_pair( 2 , COLOR_RED , COLOR_BLACK );//pop_up/error message
	init_pair( 3 , COLOR_WHITE , COLOR_RED );//comments
	init_pair( 4 , COLOR_GREEN , COLOR_WHITE );//for links
	init_pair( 5 , COLOR_RED , COLOR_WHITE );//for folders
	init_pair( 6 , COLOR_BLUE , COLOR_WHITE );//background for displaying file names
	init_pair( 7 , COLOR_BLACK , COLOR_GREEN );//arrows
	init_pair( 8 , COLOR_WHITE , COLOR_BLACK );//input_line

	attrset( COLOR_PAIR(1) );

	noecho();
	cbreak();
	keypad(stdscr, TRUE);
}
//---------------------------------------------------------------------------------------------------------------------------
bool Design_Maker::Check_Layout_Size()const
{
	wbkgd( window_ptr , COLOR_PAIR(1) );
	if( COLS < 89 || LINES < 27 )
	{
		move( LINES/2 , 0 );
		printw("Resize Window!!" ,COLS,LINES);
		wrefresh( window_ptr );
		return false;
	}
	return true;
}

void Design_Maker::Paint_Layout( const std::string & folder_path , const int position , const int files_count )const
{
	wbkgd( window_ptr , COLOR_PAIR(1) );
	wbkgd( folder_ptr , COLOR_PAIR(6) );
	attrset( COLOR_PAIR(1) | A_STANDOUT );

	//header
	move(0,0);
	printw("  F2 MOVE | F3 COPY | F4 DELETE | F5 MAKE LINK | F6 CREATE FOLDER | F9 SEARCH | F12 EXIT");
	move(1,0);
	move( 2 , COLS/2 - folder_path.size()/2 - 9 );
	attrset( COLOR_PAIR(3) | A_STANDOUT );
	printw("You are now in: %s",folder_path.c_str() );
	

	attrset( COLOR_PAIR(1) );

	//printing of each file name		
	std::stringstream ss;

	DIR *folder = opendir( folder_path.c_str() );
	struct dirent *ent;
	struct stat buf;

	std::vector<std::string> sorted_names;
	while( (ent = readdir(folder) ) != NULL )
	{
		sorted_names.push_back(ent->d_name);
	}
	closedir(folder);
	folder = opendir( folder_path.c_str() );
	std::sort( sorted_names.begin() , sorted_names.end() );
	std::vector<std::string>::iterator it = sorted_names.begin();

	int skip = 0;
	if( position > 18 )
		skip = position - 18;
	int start_sum = skip;
	int sum = skip;
	int current_LINE = 6;

	while( (ent = readdir(folder) ) != NULL )
	{
		ss.str("");
		ss << folder_path << "/" << *it;
		lstat( ss.str().c_str() , &buf );
		
		move( current_LINE , 3 );
		if( skip != 0 )
		{
			skip--;
			it++;
			continue;
		}

		if( ( sum - start_sum ) == 19 )
		{
			break;
		}

		//chooses correct color for the given data type
		if( S_ISLNK(buf.st_mode) )
		{
			if( sum == position )
				attrset( COLOR_PAIR(4) | A_STANDOUT );
			else
				attrset( COLOR_PAIR(4) );
		}
		else if( S_ISDIR(buf.st_mode) )
		{
			if( sum == position )
				attrset( COLOR_PAIR(5) | A_STANDOUT );
			else
				attrset( COLOR_PAIR(5) );
		}
		else// if( S_ISREG(buf.st_mode) )
		{
			if( sum == position )
				attrset( COLOR_PAIR(6) | A_STANDOUT );
			else
				attrset( COLOR_PAIR(6) );
		}
		printw("%s",(*it).c_str() );
		if( sum == position )//marks position of the cursor
		{
			attrset( COLOR_PAIR(6) );
			printw("  ");
			attrset( COLOR_PAIR(3) | A_STANDOUT );
			printw("<- Your position");
		}
		sum ++;
		current_LINE++;
		it++;
	}

	if( start_sum != 0 )
		Paint_Up_Arrow();

	if( ( sum - start_sum ) == 19 && sum < files_count )
		Paint_Down_Arrow();

	closedir(folder);

	attrset( COLOR_PAIR(1) );
	move( 4 , COLS - 12 );
	printw("©schejond");

	move(26,2);
	printw("Number of files in current folder: %d", files_count );


	wrefresh( window_ptr );
	wrefresh( folder_ptr );
	attrset( COLOR_PAIR(1) | A_STANDOUT );
	move(0,0);
}
//---------------------------------------------------------------------------------------------------------------------------
void Design_Maker::Close()const
{
	delwin( window_ptr );
	delwin( folder_ptr );
	endwin();
}
//---------------------------------------------------------------------------------------------------------------------------
void Design_Maker::Paint_Popup( const Popup_Type choice , std::string & answer , const Item * file )const
{
	const int height = 14;
	const int width = 80;
	const int starty = LINES/2 - height/2;
	const int startx = COLS/2 - width/2;

	WINDOW *popup_win = newwin( height, width , starty , startx );
	box( popup_win , 0 , 0 );
	wbkgd( popup_win , COLOR_PAIR(2) );
	attrset( COLOR_PAIR(2) | A_STANDOUT );
	wrefresh( popup_win );
	switch( choice )
	{
		case Popup_Type::MOVE:
		{
			move( starty + 1 , startx + width/2 - 2 );
			printw("MOVE");

			move( starty + 4 , startx + width/2 - file->Get_Name().size()/2 - 13 );
			printw("Do you want to move: \"%s\" ?",file->Get_Name().c_str() );
			move(starty + 6 , startx + 2 );
			printw("Down here you can write target destination for the file");
			move(starty + 7 , startx + 2);
			printw("Hint: you are now moving: %s",file->Get_Path().c_str() );

			move(starty + 9 , startx + 2 );
			printw("To: ");
			Catch_Path( answer , starty + 9 , startx + 6 , popup_win );
			move( starty + 10 ,  startx + 5 );
			printw("Move to: %s ?",answer.c_str() );
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			Paint_Yes_No( starty , startx );
			break;
		}
		case Popup_Type::COPY:
		{
			move( starty + 1 , startx + width/2 - 2 );
			printw("COPY");

			move( starty + 4 , startx + width/2 - file->Get_Name().size()/2 - 13 );
			printw("Do you want to copy: \"%s\" ?",file->Get_Name().c_str() );
			move(starty + 6 , startx + 2 );
			printw("Down here you can write target destination for the copy");
			move(starty + 7,startx + 2);
			printw("Hint: you are now copying: %s",file->Get_Path().c_str() );
			
			move( starty + 9 , startx + 2 );
			printw("To: ");
			Catch_Path( answer , starty + 9 , startx + 6 , popup_win );
			move(starty + 10 , startx + 5 );
			printw("Copy to: %s ?",answer.c_str() );
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			Paint_Yes_No( starty , startx );
			break;
		}
		case Popup_Type::DELETE:
		{
			move( starty + 1 , startx + width/2 - 3 );
			printw("DELETE");

			move( starty + 4 , startx + width/2 - file->Get_Name().size()/2 - 14 );
			printw("Do you want to delete: \"%s\" ?",file->Get_Name().c_str() );
			Paint_Yes_No( starty , startx );
			break;
		}
		case Popup_Type::MAKE_LINK:
		{
			std::string answers_struct;

			move( starty + 1 , startx + width/2 - 5 );
			printw("MAKE LINK");

			move( starty + 4 , startx + 2 );
			printw("Where do you want to save your link?");

			move(starty + 5 ,startx + 2 );
			printw("To: ");
			Catch_Path( answer , starty + 5 ,startx + 6 , popup_win );
			answers_struct = answer;
			move( starty + 6 , startx + 2 );
			printw("Create link at: %s ?",answer.c_str() );
			
			answers_struct += "#";
			move( starty + 7 , startx + 2 );
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			printw("On what should your link point?");

			move( starty + 8 , startx + 2 );
			printw("On: ");
			Catch_Path( answer , starty + 8 , startx + 6 , popup_win );
			answers_struct += answer;
			move( starty + 9 , startx + 2 );
			printw("Create link pointing to: %s ?",answer.c_str() );
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			answer = answers_struct;
			Paint_Yes_No( starty , startx );
			break;
		}
		case Popup_Type::CREATE_FOLDER:
		{
			move( starty + 1 , startx + width/2 - 7 );
			printw("CREATE FOLDER");
			move( starty + 4 , startx + width/2 - 16 );
			printw("Do you want to create a folder?");
			move( starty + 6 , startx + 2 );
			printw("Down here you can write target destination + name for the folder");
			move( starty + 7 , startx + 2 );
			std::string current_folder_name = file->Get_Path();
			while( current_folder_name.back() != '/' )
			{
				current_folder_name.pop_back();
			}
			printw("Hint: you are now in: %s",current_folder_name.c_str() );

			move( starty + 9 , startx + 2 );
			printw("To: ");
			Catch_Path( answer , starty + 9 , startx + 6 , popup_win );
			move( starty + 10 , startx + 5 );
			printw("Create: %s ?",answer.c_str() );
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			Paint_Yes_No( starty , startx );
			break;
		}
		case Popup_Type::SEARCH:
		{
			move( starty + 1 , startx + width/2 - 3 );
			printw("SEARCH");

			move( starty + 4 , startx + 2 );
			printw("Enter ");
			attrset( COLOR_PAIR(2) | A_STANDOUT | A_UNDERLINE );
			printw("regular expression");
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			printw(" to put into group");

			move( starty + 6 , startx + 2 );
			printw("Regex: ");
			Catch_Path( answer , starty + 6 , startx + 9 , popup_win );
			move( starty + 7 , startx+5 );
			printw("Search: %s ?",answer.c_str() );
		 	Paint_Yes_No( starty , startx );
			attrset( COLOR_PAIR(2) | A_STANDOUT );
		 	break;
		}
		case Popup_Type::GROUP:
		{
			move( starty + 1 , startx + width/2 - 3 );
			printw("GROUP");

			move( starty + 3 , startx + 2);
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			printw("I have found %s file/s",answer.c_str() );
			if( answer == "0" )
			{
				getch();
				break;
			}
			move( starty + 4 , startx + 2);
			printw("What do you want to do with found files?");
			move( starty + 6 , startx + width/2 - 20 );
			printw("F2 MOVE | F3 COPY | F4 DELETE | F12 ABORT");
			answer.clear();
			//cycle for getting users choice
			int ch;
			while( ( ch = getch() ) )
			{
				if( ch == KEY_F(2) )
				{
					move( starty + 8 , startx + 2 );
					printw("Move to: ");
					Catch_Path( answer , starty + 8 , startx + 11 , popup_win );
					move( starty + 9 , startx + 2 );
					printw("Move group to: %s ?",answer.c_str() );
					attrset( COLOR_PAIR(2) | A_STANDOUT );
					answer.push_back('M');
					Paint_Yes_No( starty , startx );
					break;
				}
				if( ch == KEY_F(3) )
				{
					move( starty + 8 , startx + 2);
					printw("Copy to: ");
					Catch_Path( answer , starty + 8 , startx + 11 , popup_win );
					move( starty + 9 , startx + 2 );
					printw("Copy group to: %s ?",answer.c_str() );
					attrset( COLOR_PAIR(2) | A_STANDOUT );
					answer.push_back('C');
					Paint_Yes_No( starty , startx );
					break;
				}
				if( ch == KEY_F(4) )
				{
					answer.push_back('D');
					move( starty + 8 , startx + width/2 -20 );
					printw("Do you want to remove all files in group?");
					Paint_Yes_No( starty , startx );
					break;
				}
				if( ch == KEY_F(12) )
				{
					break;
				}
			}
			break;
		}
		case Popup_Type::CREATE_FILE:
		{
			move( starty + 1 , startx + width/2 - 6 );
			printw("CREATE FILE");
			move( starty + 4 , startx + width/2 - 14 );
			printw("Do you want to create a file?");
			move( starty + 6 , startx + 2 );
			printw("Down here you can write target destination + name for the folder");
			move( starty + 8 , startx + 2 );
			printw("To: ");
			Catch_Path( answer , starty + 8 , startx + 6 , popup_win );
			move( starty + 9 , startx + 5 );
			printw("Create: %s ?",answer.c_str() );
			attrset( COLOR_PAIR(2) | A_STANDOUT );
			Paint_Yes_No( starty , startx );
			break;
		}
	}

	wrefresh( popup_win );
	
	attrset( COLOR_PAIR(2) );
	
	attrset( COLOR_PAIR(1) );

	delwin( popup_win );
}
//---------------------------------------------------------------------------------------------------------------------------
void Design_Maker::Paint_Up_Arrow()const
{
	attrset( COLOR_PAIR(7) );
	move(5,1);
	printw("^");

	attrset( COLOR_PAIR(1) );
}
//---------------------------------------------------------------------------------------------------------------------------
void Design_Maker::Paint_Down_Arrow()const
{
	attrset( COLOR_PAIR(7) );
	move(25,1);
	printw("v");

	attrset( COLOR_PAIR(1) );
}
//---------------------------------------------------------------------------------------------------------------------------
void Design_Maker::Paint_Yes_No( const int starty , const int startx )const
{
	attrset( COLOR_PAIR(2) | A_STANDOUT );
	move( starty + 12 , startx + 2 );
	printw("Y/y to Confirm");
	attrset( COLOR_PAIR(2) );
	printw("                                                 ");
	attrset( COLOR_PAIR(2) | A_STANDOUT );
	printw("N/n to Cancel");
}
//---------------------------------------------------------------------------------------------------------------------------
void Design_Maker::Paint_Error_Message( const std::string & message )const
{
	const int height = 12;
	const int width = 40;
	const int starty = LINES/2 - height/2;
	const int startx = COLS/2 - width/2;
	WINDOW *error_win = newwin( height, width , starty , startx );
	attrset( COLOR_PAIR(2) );
	box( error_win , 0 , 0 );
	wbkgd( error_win , COLOR_PAIR(2) );

	attrset( COLOR_PAIR(2) | A_STANDOUT );
	move( starty + 1 , startx + width/2 - 3 );
	printw("ERROR!");
	move( starty + height/2 , startx + width/2 - 15 );
	printw("Invalid operation! Try again.");
	move( starty + height/2 + 1  , startx + width/2 - message.size()/2 );
	printw("%s",message.c_str() );

	wrefresh( error_win );
	getch();
	attrset( COLOR_PAIR(1) );
	delwin( error_win );
}
//---------------------------------------------------------------------------------------------------------------------------
void Design_Maker::Catch_Path( std::string & answer , int starty , int startx , WINDOW *popup_win )const
{
	int ch;
	char c;
	std::string written;
	attrset( COLOR_PAIR(8) );

	while( ( ch = getch() ) != 10 )//KEY_F(12) )
	{
		move( starty, startx );
		if( ch == KEY_BACKSPACE && !written.empty() )
		{
			startx--;
			move( starty , startx );
			printw(" ");
			written.pop_back();
			wrefresh( popup_win );
			continue;
		}
		c = ch;
		printw("%c",c);
		startx++;
		written.push_back(c);
		wrefresh( popup_win );
	}

	answer = written;
}