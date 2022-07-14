/**
 * @brief Function that checks if user has inputted an string that matches regex rules
 */
bool Check_Regex_Syntax( const std::string & input_regex)
{
	try
	{
		std::regex tmp(input_regex);
	}
	catch (const std::regex_error& e)
	{
		return false;
	}
	return true;
}