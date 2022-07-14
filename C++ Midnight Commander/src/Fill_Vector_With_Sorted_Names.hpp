/**
 * @brief Function, which gets name from current folder and sorts them in vector
 */
void Fill_Vector_With_Sorted_Names( std::vector<std::string> & vecty , const std::string & folder_path )
{
	DIR *folder = opendir( folder_path.c_str() );
	struct dirent *ent;
	while( ( ent = readdir(folder) ) != NULL )
	{
		vecty.push_back(ent->d_name);
	}
	closedir(folder);
	std::sort( vecty.begin() , vecty.end() );
}