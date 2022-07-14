/**
 * @brief Function that obtains path to a Item, where the symbolic link is pointing to
 */
std::string Find_Link_Target( const char* cmd )
{
	char buffer[256];
	std::string result = "";
	FILE* pipe = popen( cmd , "r" );
	while ( !feof(pipe) )
	{
		if (fgets( buffer , 256, pipe ) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;
}