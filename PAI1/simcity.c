#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

char *pArray;
int tisk=false;
int sum=0;

void print()
{
	char *posuvnik;
	posuvnik=pArray;
	if( tisk )
	{
		printf("[" );
		while(*posuvnik)
		{
			printf("%c", *(posuvnik) );
			posuvnik++;
		}
		printf("]\n");
	}
}

int rec( const int modul,int position,const int houses,int park,int lastComer, int Indus,char minuly)
{
	if(minuly !='P' && park > modul)
		return 0;
	if ( (minuly!='I' && Indus==1) )
		return 0;
	pArray[ position - 2 ] = minuly;
	if (position == houses+1)
		{
			pArray[position-1]=0;
			print();
			return 1;
		}
	sum += rec(modul,position +1, houses,0,lastComer,Indus,'P');
	sum += rec(modul,position +1, houses,park+1,lastComer,Indus,'R');
	if( position-lastComer-1 >= modul )
		sum += rec(modul,position+1,houses,park+1,position,Indus,'C');

	if( Indus==1 || (Indus==0 && minuly=='I' ) )
	{
		sum += rec(modul,position+1,houses,park+1,lastComer,0,'I');
	}
	else 
	{
		if (houses-position+1 >=2 )
		{
			sum += rec(modul,position+1,houses,park+1,lastComer,1,'I');
		}
	}
	return 0;
}

void count_houses_recursive(const int modul,int position,const int houses)
{
	sum += rec(modul,position+1,houses,0,-66,0,'P');
	sum += rec(modul,position+1,houses,1,position,0,'C');
	sum += rec(modul,position+1,houses,1,-66,0,'R');
	if(houses-position+1 >= 2)
			sum += rec(modul,position+1,houses,1,-66,1,'I');
}
bool load_modul(int *modul)
{
	if (scanf("%d",modul) == 1 && *modul >= 1 && *modul <= 10 )
			return true;
	return false;
}

bool load_command()
{
	char key_word[10];
	char eof;
	eof=scanf("%8s",key_word);
	if(eof==EOF)
		return false;
	if ( strcmp("count",key_word) && strcmp("list",key_word) )
	{
		printf("Nespravny vstup.\n");
		return false;
	}
	if (!strcmp("list",key_word))
		tisk=true;
	else
		tisk=false;
	return true;
}

bool load_houses(int *houses)
{
	double test;
	if (scanf("%lf",&test)!= 1 || test <= 0)
	{
		printf("Nespravny vstup.\n");
		return false;
	}
	*houses=test;
	if (*houses!=test)
	{
		printf("Nespravny vstup.\n");
		return false;
	}
	return true;
}

void create_array(const int houses)
{
	pArray=(char*)malloc( (houses+2) * sizeof( *pArray ) );
}

int main(int argc, char const *argv[])
{
	int modul,houses;
	printf("Modul:\n");
	if( !load_modul(&modul) )
	{
		printf("Nespravny vstup.\n");
		return 1;
	}
	printf("Vypocty:\n");
	while( load_command() )
	{
		if( !load_houses(&houses) )
			return 1;
		create_array(houses);
		count_houses_recursive(modul,1,houses);
		printf("=> %d\n", sum);
		sum=0;
		if (tisk)
			free (pArray);
	}
	return 0;
}