#include "Files_Wrapper.hpp"
#include "Regex_Matcher.hpp"
#include "Find_Link_Target.hpp"
#include "Fill_Vector_With_Sorted_Names.hpp"
//---------------------------------------------------------------------------------------------------------------------------
void Files_Wrapper::Fill_Wrapper( const std::string & folder_path , const std::string & input_regex )
{
	std::stringstream ss;
	DIR *folder = opendir( folder_path.c_str() );
	struct dirent *ent;
	struct stat buf;

	std::vector<std::string> sorted_names;
	Fill_Vector_With_Sorted_Names( sorted_names , folder_path );
	std::vector<std::string>::iterator it = sorted_names.begin();

	while( (ent = readdir(folder)) != NULL )//divides files according to their data type
	{
		if( input_regex.size() > 0 )
		{
			if( !Regex_Matcher(*it,input_regex) )
			{
				it++;
				continue;
			}
		}

		ss.str("");
		ss << folder_path << "/" << *it;
		lstat( ss.str().c_str() , &buf );

		if( S_ISLNK(buf.st_mode) )
		{
			std::stringstream ss2;
			ss2 << "readlink -f " << folder_path << "/" << *it;
			std::string link_target = Find_Link_Target( ss2.str().c_str() );
			link_target.pop_back();
			Item * ptr = new Link( ss.str() , *it , link_target );
			files.push_back(ptr);
		}
		else if( S_ISDIR(buf.st_mode) )
		{
			Item * ptr = new Folder( ss.str() , *it );
			files.push_back(ptr);
		}
		else// if( S_ISREG(buf.st_mode) )
		{
			Item * ptr = new File( ss.str() , *it );
			files.push_back(ptr);
		}
		it++;
	}
	closedir(folder);
}
//---------------------------------------------------------------------------------------------------------------------------
size_t Files_Wrapper::Get_Size()const
{
	return files.size();
}
//---------------------------------------------------------------------------------------------------------------------------
bool Files_Wrapper::Is_Folder( const int position )const
{
	std::vector<Item*>::const_iterator it = files.begin();
	it += position;
	return (*it)->Is_Folder();
}
//---------------------------------------------------------------------------------------------------------------------------
std::string Files_Wrapper::Get_Name( const int position )const
{
	std::vector<Item*>::const_iterator it = files.begin();
	it += position;
	return (*it)->Get_Name();
}
//---------------------------------------------------------------------------------------------------------------------------
void Files_Wrapper::Delete_Wrapped_Items()
{
	for( auto del : files )
	{
		delete del;
	}

	files.clear();
}
//---------------------------------------------------------------------------------------------------------------------------
Item * Files_Wrapper::Get_Item( const int position )const
{
	std::vector<Item*>::const_iterator it = files.begin();
	it += position;
	return *it;
}
//---------------------------------------------------------------------------------------------------------------------------
void Files_Wrapper::Delete_Permanently()const
{
	for( auto it : files )
	{
		if( it->Get_Name() == "." || it->Get_Name() == ".." )
			continue;
		it->Delete();
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void Files_Wrapper::Move_Permanently( const std::string & command , const std::string & current_folder_path )const
{
	for( auto it : files )
	{
		if( it->Get_Name() == "." || it->Get_Name() == ".." )
			continue;
		it->Move( command , current_folder_path );
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void Files_Wrapper::Copy_Permanently( const std::string & command , const std::string & current_folder_path )const
{
	for( auto it : files )
	{
		if( it->Get_Name() == "." || it->Get_Name() == ".." )
			continue;
		it->Copy( command , current_folder_path );
	}
}