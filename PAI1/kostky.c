#include <stdio.h>
int pole[500000];

//secteni obsahu poli
void modifying_array()
{	
	for (int i = 0; i < 500000-1; ++i)
	{
		pole[i+1]=pole[i]+pole[i+1];		
	}
}

void creating_array()
{
	int a,b,c,objem;
	for (a = 1; a < 80; ++a)
		{
			for (b = a; b < 708; ++b)
			{
				for (c = b; c < 500000; ++c)
				{
					objem=a*b*c;
					if(objem>500000)
					{
						break;
					}
					else
					{
					pole[objem]++;;
					}
				}
			}
		}
		modifying_array();
}

int load_input(int *start,int *end)
{
	char prvni_zavorka, druha_zavorka;
	const int loaded_variables = scanf(" %c %d ;%d %c",&prvni_zavorka,start,end,&druha_zavorka);

	if(loaded_variables==EOF)
	{
		return 0;
	}

	if(loaded_variables!=4 || prvni_zavorka!='<' || druha_zavorka!='>' || *start<1
		|| *end > 500000 || (*start)>(*end))
	{
		printf("Nespravny vstup.\n");
		return 0;
	}

	return 1;
}

int main(int argc, char const *argv[])
{
	int start,end;
	creating_array();
	printf("Intervaly:\n");
while(load_input(&start,&end))
	{
		printf("Ruznych kvadru: %d\n", pole[end]-pole[start-1]);	
	}
	return 0;
}