#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
//-----------------------------------------------------------------------------------------------
//globalni promenne
long long int needed_cars;
int *pArray;
long long int lenght,count;
//----------------------------------------------------------------------------------------------
//lcm
long long int lcm(const long long int first, const int *second)
{
	long long int tmp1,tmp2,t;
 	tmp1 = first;
  	tmp2 = *second;
 
  	while (tmp2 != 0) 
   	{
   		t = tmp2;
  		tmp2 = tmp1 % tmp2;
  		tmp1 = t;
  	}
	return (first/tmp1)*(*second);
}
//--------------------------------------------------------------------------------------------------------
//nacteni a vypocet potrebnych aut
void count_needed_cars(const long long int starting_route, const long long int ending_route)
{
 	needed_cars=1;
	for (int i = starting_route; i < ending_route ; ++i)
	{
		needed_cars=lcm(needed_cars,pArray+i);
	}
}

bool loadRoutes()
{
	long long int starting_route, ending_route;
	char eof;
	if( (eof=scanf("%lli %lli",&starting_route,&ending_route))==2
		 && starting_route  <  ending_route
		 && starting_route >=  0
		 && ending_route   <=  count  )
	{
		count_needed_cars(starting_route,ending_route);
		return true;
	}

	if (eof==EOF)
		return false;

	printf("Nespravny vstup.\n");
	return false;
}
//-----------------------------------------------------------------------------------------
//prace s dynamicky polem
void stretch()
{
	lenght *= 2;
	int *new_array=(int*)malloc(lenght*sizeof*pArray);
	memcpy(new_array , pArray , count*sizeof*pArray);
	free(pArray);
	pArray=new_array;
}

void create_array()
{	
	lenght=200;
	pArray=(int*)malloc(lenght*sizeof*pArray);
	count=0;
}
//-----------------------------------------------------------------------------------------
//nacteni, kontrola vstupu a umisteni do pole
bool loadInput1()
{
	char zavorka1;
	if(!(scanf(" %c",&zavorka1)==1 && zavorka1=='{'))
		return false;

	char carka;
	while(scanf("%d %c",pArray+count,&carka)==2 && *(pArray+count)>0)
	{
		count++;
		if (count==lenght)
			stretch();
		
		if(carka==',')
			continue;

		if (carka=='}')
			return true;

		return false;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
	create_array();
	printf("Pocty pruhu:\n");

	if (!loadInput1())
	{
		printf("Nespravny vstup.\n");
		free(pArray);
		return 1;
	}
	printf("Trasy:\n");
	while (loadRoutes())
	{
		printf("Vozidel: %lli\n", needed_cars);
	}
	free(pArray);
	return 0;
}