#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#define prime 71
//--------------------------------------------------------------------------------------------------------------
//globalni promenne
char *pHay;
char *pNeedle;
long long int *pLines;
long long int *pNewLines;
long long int *pRes;
long long int cntHay=0,cntLines=0,rest=0,cntNeedle=1,cntNewLines=1,cntRes=0;//pocitadla v jednotlivych polich
long long int lengthLines,lengthHay,lengthNeedle,lengthRes;//delky poli
int end_of_entry_warning=0,just_spec_chars=0;

//--------------------------------------------------------------------------------------------------------------
//prace s dynamickymi poly
void create_arrays()
{
	lengthHay=200;
	pHay=(char*)malloc(lengthHay*sizeof(*pHay) );
	*pHay=32;
	cntHay=1;

	lengthLines=20;
	pLines=(long long int*)malloc(lengthLines*sizeof(*pLines) );
	*pLines=0;
}
void create_array_needle_NewLength()
{
	lengthNeedle=100;
	pNeedle=(char*)malloc(lengthNeedle*sizeof(*pNeedle) );
	*pNeedle=32;//na zacatek pole dam mezeru

	pNewLines=(long long int*)malloc(lengthLines*sizeof(*pNewLines) );
}
void allock_results()
{
	lengthRes=30;
	pRes=(long long int*)malloc(lengthRes*sizeof(*pRes) );
}

void Stretch(const int switcher)
{
	//1 zvetsuje pHay || 2 zvetsuje pLines || 3 zvetsuje pNeedle || 4 pRes
	switch(switcher)
	{
		case 1:
		{
			lengthHay *= 2;
			pHay=(char*)realloc( pHay, lengthHay * sizeof(*pHay) );
			break;
		}
		case 2:
		{
			lengthLines *= 2;
			pLines=(long long int*)realloc( pLines, lengthLines * sizeof(*pLines) );
			break;
		}
		case 3:
		{
			lengthNeedle *= 2;
			pNeedle=(char*)realloc( pNeedle, lengthNeedle * sizeof(*pNeedle) );
			break;
		}
		case 4:
		{
			lengthRes *= 2;
			pRes=(long long int*)realloc( pRes, lengthRes * sizeof(*pRes) );
			break;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------
//nacitani vstupniho textu
//zapise delku radku (pri zadani 'space'ahoj\n je delka prvniho radku (index 1) pouze 5 )
void add_Length(const long long int length)
{
	cntLines++;
	if (cntLines==lengthLines-1)
		Stretch(2);
	*(pLines+cntLines)=length;
}
bool CheckHayLength()
{
	cntHay=strlen(pHay);
	if ( cntHay == lengthHay-1 )
		Stretch(1);
	if ( (cntHay>2 && *(pHay+cntHay-1) =='\n' && *(pHay+cntHay-2) =='\n') || (cntHay==2 && *(pHay+1) =='\n') )
	{
		end_of_entry_warning=1;//tuto promenou pouzivam na vyskoceni z while cyklu nacitani
		return true;
	}
	long long int sum=0;
//tuto cast provedu pouze pokud jsem jeste nezapsal zadny radek -> osetruje nespravny vstup (prvni radka jen spec.znaky/cisla)
	if(cntLines==0)
	{
		for (long long int i = 0; i < cntHay; ++i)
		{
			if ( !isalpha( *(pHay+i) ) )
				sum++;
		}
		if ( sum == cntHay )
		{
			just_spec_chars=1;
			end_of_entry_warning=1;
			return true;
		}
	}
	if( *(pHay+cntHay-1) =='\n')
	{
		add_Length(cntHay-1);
		rest=0;
		return true;
	}
	//pokud nebyla predchozi podminka splnena,znamena to,ze fgets naplnil pole ale jeste nedokoncil cteni radky
	//proto sem realocoval a predam rest 
	rest= cntHay -1 - *(pLines+cntLines);
	return false;
}
//---------------------------------------------------------------------------------------------------------
//obecne operace s polem znaku
void Modify_new_Lengths(const long long int length)//
{
	*(pNewLines+cntNewLines)=length;
	cntNewLines++;
}
void DestroySpecChars(const long long int arraySize,char *array)
{
	for (long long int i = 0; i < arraySize; ++i)
		if ( !isalpha(*(array+i)) )
			*(array+i)=32;
}
long long int PositionOLD(const long long int where)//kontroluje pozici v poli se starymi delkami
{
	for (long long int i = 1; i <= cntLines; ++i)
		if(where <= *(pLines+i) )
			return i;
	return 666;
}
void SqueezeSpaces_RewriteLenghts(char *array)//squezuje mezery mezi slovy (na okrajich vzdy 1 necha)
{
	char *edit;
	char *original;
	int WasSpace = 0;
	long long int e=0,o=0;//e a o slouzi k zapamatovani si pozice a pomoci nich zapisuji nove delky radku

  	edit= pHay;
  	original= pHay;

	while ( *original )
	{
		if ( WasSpace )
		{ 
    		if ( *original == 32)
    			cntHay -= 1;

    		if ( *original != 32 )
      		{
        		*edit++ = *original;
        		e++;
        		WasSpace = 0;
      		}
    	}
    	else
    	{
			*edit++ = *original;
			e++;
      		if ( *original == 32 ) 
        		WasSpace = 1;
    	} 
    	original ++;
    	o++;

    	if (PositionOLD(o) == PositionOLD(o-1)+1)
   			Modify_new_Lengths(e-1);
   		if (PositionOLD(o)==666 && !*original)
   		{
   			Modify_new_Lengths(e-1);
   		}
  	}
  	*edit = 0;
}

void SqueezeSpaces_Basic(char *array,long long int *size)//to same jako minula funkce jen bez prepisovani delek
{
	char *edit;
	char *original;
	long long int WasSpace = 0;

  	edit= pNeedle;
  	original= pNeedle;

	while ( *original )
	{
		if ( WasSpace )
		{ 
    		if (*original ==32)
    		{
    			*size= *size-1;
    		}	
    		if ( *original != 32 )
      		{
        		*edit++ = *original;
        		WasSpace = 0;
      		}
    	}
    	else
    	{
			*edit++ = *original;
      		if ( *original == 32 ) 
        		WasSpace = 1;
    	} 
    	original ++;
  	}
  	*edit = 0;
}
void GrowAlpha(const long long int count,char *array)
{
	for (long long int i = 0; i < count; ++i)
		if ( isalpha(*(array+i)) )
			*(array+i)=toupper( *(array+i) );
}
long long int WhichLine(const long long int position,const long long int end)//po nalezeni vysledku urci ve ktere je radce
{
	for (long long int i = 1; i < end; ++i)
		if(position < *(pNewLines+i) )//tady by mi logicky sedelo <= ale s tim to nefunguje...nedoresil jsem tento problem
			return i;
	return end-1;//sem moc nevim co jsem mel napsat
}
//------------------------------------------------------------------------------------------------
//nacteni hledaneho textu
bool CheckNeedle()
{
	cntNeedle=strlen(pNeedle);
	if ( cntNeedle == lengthNeedle-1 )
		Stretch(3);
	if(cntNeedle==2 && *(pNeedle+1)=='\n')
	{
		end_of_entry_warning++;
		return true;
	}
	long long int sum=0;//opet kontrola pokud je cela radka naplnena jen specialnimi znaky
	for (long long int i = 0; i < cntNeedle; ++i)
	{
		if ( !isalpha( *(pNeedle+i) ) )
			{sum++;}
	}
	if (*(pNeedle+cntNeedle-1)=='\n')
	{
		rest=0;
		if ( sum == cntNeedle )
			end_of_entry_warning=1;
		return true;
	}

	rest=cntNeedle-1;//opet stejny postup jako s restem z minuleho nacitani
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
//search algorithm
long long int CreateHash(char *array,const long long int end)
{
	long long int hash=0;
	for (long long int i = 0; i < end; ++i)
		hash += *(array+i)*pow(prime,i);
	return hash;
}
long long int REDO_hash(char *array,const long long int oldstart,const long long int newend, long long int oldhash,const long long int needle_length)
{
	long long int newHash=oldhash-*(array+oldstart);
	newHash= newHash/prime;
	newHash += *(array+newend-1)*pow(prime,needle_length-1);
	return newHash;
}
bool checkStrings(char *text,long long int start1,long long int end1, char *needle,long long int start2,long long int end2)
{
	if(end1 - start1 != end2 - start2)//asi zbytecny ale pro jistotu
	{
		return false;
	}
	while(start1 < end1 && start2 < end2)
	{
		if( *(text+start1) != *(needle+start2) )
		{
			return false;
		}
		start1++;
		start2++;
	}
	return true;
}
long long int Search(const long long int needle_length,long long int skip)//using rabin karp
{
	long long int needleHash=CreateHash(pNeedle,needle_length);
	long long int textHash=CreateHash(pHay,needle_length);
	for (long long int i = 1; i <= cntHay-cntNeedle+1 ; i++)
	{
		if (needleHash==textHash && checkStrings(pHay,i-1,i-1+cntNeedle,pNeedle,0,cntNeedle) )
		{	if (skip==0)
				return i-1;
			else
				skip--;
		}
		if (i<cntHay-cntNeedle+1)
			textHash=REDO_hash(pHay,i-1,cntNeedle+i,textHash,cntNeedle);
	}
	return -1;
}
//--------------------------------------------------------------------------------------------------------------
//tests
void testLoad()
{
	printf("Testing extending of Hay:\n");
	for (long long int i = 0; i < cntHay; ++i)
		printf("%c", *(pHay+i) );
	printf("\n");
}
void test_Needle()
{
	printf("TESTING CONTENT OF NEEDLE!\n");
	for (long long int i = 0; i < cntNeedle; ++i)
		printf("%c|", *(pNeedle+i));
	printf("\n");
}
// void testHash()
// {
// 	//works if prime is 3
// 	long long int result1=5080;
// 	long long int result2=6926;
// 	char array[10]={"ahoj\nA"};
// 	int start=0;
// 	int end=4;
// 	int delka=4;
// 	long long int hash=CreateHash(array,end);
// 	printf("Testing create hash:\n");
// 	if (hash==result1)
// 		printf("SUCCESS\n");
// 	else
// 		{printf("FAILED\n");printf("hash:%lld\n",hash);}
	
// 	long long int newhash=REDO_hash(array,start,end+1,hash,delka);
// 	printf("Testing REDO hash:\n");
// 	if (newhash==result2)
// 		printf("SUCCESS\n");
// 	else
// 		printf("FAILED\nnewhash:%lld\n",newhash);
// }
// void testStringsComparing()
// {
// 	printf("Testing Comaparison of strings:\n");
// 	char array[10]={"Ahoj\n"};
// 	char key[15]={"nocusAhoj\n"};
// 	int result=checkStrings(array,0,4,key,5,9);
// 	if (result)
// 		printf("SUCCESS\n");
// 	else
// 		printf("FAILED\n");
// }
void test_porovnani()
{
	printf("Test porovnani:\n");
	printf("ve funkci:%lld\n", cntNewLines);
	for (long long int i = 1; i < cntNewLines; ++i)
	{
		printf("OLD: %lld vs NEW: %lld\n", *(pLines+i) , *(pNewLines+i) );
	}
}
//--------------------------------------------------------------------------------------------------------------
int main(void)
{
	create_arrays();
	printf("Text:\n");
	while( fgets( pHay+1 + *(pLines+cntLines) +rest , lengthHay - cntHay, stdin ) != NULL )//ta +1 za pHay je kvuli tomu,ze prvni znak je mnou dany -> mezera
	{
		if( !CheckHayLength() )//davam continue pokud kvuli velikosti pole se nepovedlo nacist celou radku
			continue;
		if (end_of_entry_warning)
			break;
	}
	if ( feof (stdin) || cntHay==2 || just_spec_chars)
 	{
 		printf ( "Nespravny vstup.\n" );
 		free (pHay);
		free (pLines);
 		return 1;
	}
	DestroySpecChars(cntHay,pHay);
	create_array_needle_NewLength();
	SqueezeSpaces_RewriteLenghts(pHay);
	*(pNewLines+cntNewLines-1) += 1;
	free(pLines);
	GrowAlpha(cntHay,pHay);	
//----------------------------------------------------------
	printf("Hledani:\n");
	end_of_entry_warning=0;
	rest=0;
	allock_results();//alokace pole do ktereho budu zapisovat vysledky
	while ( fgets( pNeedle+1+rest , lengthNeedle - cntNeedle , stdin ) != NULL)
	{
		if(!CheckNeedle())
			continue;
		if (end_of_entry_warning)
		{
			printf("Neplatny dotaz\n");
			end_of_entry_warning=0;
			continue;
		}

		DestroySpecChars(cntNeedle,pNeedle);
		SqueezeSpaces_Basic(pNeedle,&cntNeedle);
		GrowAlpha(cntNeedle,pNeedle);
		cntRes=0;
		long long int x;
		long long int skip=0;

		while( (x=Search(cntNeedle,skip) ) >=0 )
		{
			*(pRes+cntRes)=x;
			cntRes++;
			if (cntRes == lengthRes-1)
				Stretch(4);
			skip++;//Search mi vraci prvni nalezenou schodu...diky skipu by mel najit vsechny
		}
		if (/*x==-1 && */cntRes==0)//asi tam to -1 nemusi byt proot jsem to zakryl
			{printf("Nenalezeno\n");continue;}
		printf("Nalezeno: " );
		for (long long int i = 0; i < cntRes; ++i)
		{
			printf("%lld", WhichLine(*(pRes+i),cntNewLines) );
			if (i < cntRes-1)
				printf(", ");
		}
		printf("\n");
	}
	free (pHay);
	free (pNeedle);
	free (pNewLines);
	free (pRes);
	return 0;
}