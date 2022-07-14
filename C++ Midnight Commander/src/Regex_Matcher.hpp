/**
 * @brief Search if string matches according to regex rules
 */
bool Regex_Matcher( const std::string & file_name , const std::string & input_regex )
{
	std::regex reg(input_regex);
	if( std::regex_match( file_name.begin() , file_name.end() , reg ) )
		return true;
	return false;
}