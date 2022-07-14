#include <stdio.h>
#include <stdbool.h>
#define maxInputLenght 25000

typedef struct value{
int loaded_value;
int start_position;
int string_end;	
int string_lenght;
} value;
//-------------------------------------------------------------------------------------------------------
//globalni promenne
value array[maxInputLenght];
int nmb_of_inputs=0;
int longest_string=0;
//-------------------------------------------------------------------------------------------------------
bool LoadInput()
{
	char input;	
	while((input=scanf("%d",&array[nmb_of_inputs].loaded_value))==1)
	{
		if (nmb_of_inputs>=maxInputLenght)
		{
			return false;
		}
		array[nmb_of_inputs].start_position=nmb_of_inputs;
		nmb_of_inputs++;
	}
	nmb_of_inputs--;

	return input==EOF && nmb_of_inputs>0;
	// 	return true;
	// else
	// 	return false;
}

void set_longest_string(const int inserted_string)
{

	if (longest_string<inserted_string)
		longest_string=inserted_string;

}

void finding_strings()
{
	for (int k = 0; k <= (nmb_of_inputs-longest_string+1) ; ++k)
		{	
		
		for (int i = nmb_of_inputs; i > k; --i)
		{
			if (array[i].loaded_value>=array[k].loaded_value)
			{
				array[k].string_end=i;
				array[k].string_lenght=i-k+1;
				set_longest_string(array[k].string_lenght);
				break;
			}
		}
	}	
}

bool lenght_test()
{
 	if (longest_string<=1)
 		return false;
 	return true;
}

int print_longest_strings()
{
	int options=1;
	for (int i = 0; i <= nmb_of_inputs ; ++i)
	{
		if (array[i].string_lenght==longest_string)
		{
			printf("%d: %d - %d\n", array[i].string_lenght, array[i].start_position, array[i].string_end);
			options++;
		}
	}
	options--;
	return options;
}
//---------------------------------------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
	printf("Hodnoty:\n");
	
	if( !LoadInput() )
	{
		printf("Nespravny vstup.\n");
		return 1;
	}
	
	finding_strings();
	
	if( !lenght_test() )
	{
		printf("Nelze najit.\n");
		return 1;
	}

	int options=print_longest_strings();
	printf("Moznosti: %d\n", options);
	return 0;
}