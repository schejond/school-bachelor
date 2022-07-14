#include "Window.hpp"

int main(int argc, char const *argv[])
{
	Window a;
	if( !a.Initialize() )
		return 0;
	
	a.Catch_Orders();
	
	return 0;
}