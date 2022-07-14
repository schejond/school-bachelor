#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */

void Create_array(char **pDecompressed,const long long int length)
{
	*pDecompressed=(char*)malloc(length*sizeof(**pDecompressed) );
}
void Stretch (char **pDecompressed, long long int *length)
{
	*length *= 2;
	*pDecompressed=(char*)realloc( *pDecompressed, *length * sizeof(**pDecompressed) );
}
char * RLEDecompress ( const char * src )
{
  long long int Decompressed_Length=21;
  char *pDecompressed;
  Create_array(&pDecompressed,Decompressed_Length);

  long long int e=0;
  char memory=0;
  int count=0;

  while( *src )
  {
  	if ( *src < 48 || *src > 57 )
  	{
  		*(pDecompressed+e)=*src;
  		e++;
  		if(e==Decompressed_Length-1)
  			Stretch(&pDecompressed,&Decompressed_Length);
  		memory=*src;
  	}
  	else
  	{
  		count=0;
  		if(memory==0)
  		{
  			memory=*src;
  			src++;
  			count=1;
  		}
  		count += atoi(src);
  		while ( *src >= 48 && *src <= 57 )
  			src++;
  		if(count==0)
  			e--;
  		else
  		{
  			for (int i = 1; i < count; ++i)
  			{
				*(pDecompressed+e)=memory;
  				e++;
  				if(e==Decompressed_Length-1)
  					Stretch(&pDecompressed,&Decompressed_Length);
  			}
  		}
  		src--;
  	}
  	src++;
  }
   *(pDecompressed+e)=0;
//   while(*pDecompressed)
//   {
//   	printf("%c", *(pDecompressed));
//   	pDecompressed++;
//   }
// printf("\n");
return pDecompressed;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  char * res;

  assert ( ! strcmp ( 
    (res = RLEDecompress ( "Hello world!" )),
    "Hello world!" ));
  free ( res );

  assert ( ! strcmp ( 
    (res = RLEDecompress ( "Hello 30world!" )),
    "Hello                              world!" ));
  free ( res );

  assert ( ! strcmp ( 
    (res = RLEDecompress ( "Hel2o world!10" )),
    "Hello world!!!!!!!!!!" ));
  free ( res );

  assert ( ! strcmp ( 
    (res = RLEDecompress ( "H2e6l8o15 35w5o6r-2d0!" )),
    "HHeeeeeellllllllooooooooooooooo                                   wwwwwoooooor--!" ));
  free ( res );

  assert ( ! strcmp ( 
    (res = RLEDecompress ( "321" )),
    "333333333333333333333" ));
  free ( res );

  return 0;
}
#endif /* __PROGTEST__ */