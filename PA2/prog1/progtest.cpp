#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <climits>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;

const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;

#endif /* __PROGTEST__ */

struct header{
	uint16_t endian;
	uint16_t width;
	uint16_t length;
	uint16_t pixel;
};

	unsigned char *pArray = NULL;
	unsigned char *pArrayOUT = NULL;

	unsigned char *pPole64 = NULL;
	unsigned char *pPole32 = NULL;
	unsigned char *pPole16 = NULL;
	unsigned char *pPole8 =NULL;
	unsigned char *pPole4 =NULL;
	unsigned char *pPole2 =NULL;
	unsigned char *pPole1 =NULL;

int pole64_length,pole32_length,pole16_length,pole8_length,pole4_length,pole2_length,pole1_length;

int cnt_pole64,cnt_pole32,cnt_pole16,cnt_pole8,cnt_pole4,cnt_pole2,cnt_pole1;
int cnt_ArrayOUT, cnt_ArrayIN;

bool header_control( int interleave , uint16_t & byteOrder )
{
	if( byteOrder != ENDIAN_LITTLE && byteOrder != ENDIAN_BIG )
		return false;
	//pridano pro BIG ENDIAAN
	if( byteOrder == ENDIAN_BIG )
		return false;
	//------------
	if ( interleave < 1 || interleave > 64 || ( (interleave % 2) != 0 && interleave != 1 ) )
		return false;
	if( interleave != 1 && interleave != 2 && interleave != 4 && interleave != 8 && interleave != 16 && interleave != 32 && interleave != 64 )
		return false;
	return true;
}

void decider_prokladani( uint16_t &src, int & prokladani, int & bity_na_kanal, int & kanaly )
{
	uint16_t tmp1,tmp2,tmp3;

	tmp1 = src/( pow(2,5) );
	tmp2 = (src - ( tmp1 * pow(2,5) ) )/( pow(2,2) );
	tmp3 = src - ( tmp1 * pow(2,5) ) - (tmp2 * pow(2,2) ) ;

	switch(tmp1)
	{
		case 0:
			prokladani = 1;break;
		case 1:
			prokladani = 2;break;
		case 2:
			prokladani = 4;break;
		case 3:
			prokladani = 8;break;
		case 4:
			prokladani = 16;break;
		case 5:
			prokladani = 32;break;
		case 6:
			prokladani = 64;break;
		default:
			prokladani = -1;break;
	}

	switch( tmp2 )
	{
		case 0:
			bity_na_kanal = 1;break;
		case 3:
			bity_na_kanal = 8;break;
		case 4:
			bity_na_kanal = 16;break;
		default:
			bity_na_kanal = -1;break;
	}

	switch( tmp3 )
	{
		case 0:
			kanaly = 1;break;
		case 2:
			kanaly = 3;break;
		case 3:
			kanaly = 4;break;
		default:
			kanaly = -1;break;
	}
}

unsigned char *create_array_pool(int & delka)
{
	delka = 21;
	unsigned char *pole = new unsigned char[delka];
	return pole;
}

bool load_input(ifstream & FILE , int & cnt_ArrayIN , int & delka_arrayIN, const int & nmb_to_load )
{
	if( !FILE.read( (char*)(pArray) , nmb_to_load * sizeof(unsigned char) ) )
		return false;

	cnt_ArrayIN = nmb_to_load;

	return true;
}

void stretch_pool(int & delka,const int & switcher)
{
	int puvodni_delka = delka;
 	delka *= 2; 
 	unsigned char *pole = new unsigned char[delka];
	switch( switcher )
	{
		case 64:
		 	memcpy(pole , pPole64 , puvodni_delka * sizeof(unsigned char) );		
 	 		delete[] pPole64;
 	 		pPole64 = pole;
			break;
		case 32:
		 	memcpy(pole , pPole32 , puvodni_delka * sizeof(unsigned char) );		
 	 		delete[] pPole32;
 	 		pPole32 = pole;
			break;
		case 16:
		 	memcpy(pole , pPole16 , puvodni_delka * sizeof(unsigned char) );		
 	 		delete[] pPole16;
 	 		pPole16 = pole;
			break;
		case 8:
		 	memcpy(pole , pPole8 , puvodni_delka * sizeof(unsigned char) );		
 	 		delete[] pPole8;
 	 		pPole8 = pole;
			break;
		case 4:
		 	memcpy(pole , pPole4 , puvodni_delka * sizeof(unsigned char) );		
 	 		delete[] pPole4;
 	 		pPole4 = pole;
			break;
		case 2:
		 	memcpy(pole , pPole2 , puvodni_delka * sizeof(unsigned char) );		
 	 		delete[] pPole2;
 	 		pPole2 = pole;
			break;
		case 1:
		 	memcpy(pole , pPole1 , puvodni_delka * sizeof(unsigned char) );		
 	 		delete[] pPole1;
 	 		pPole1 = pole;
			break;
	}
}

void fill_it(const int switcher , int & cnt , int & delka , const int & balicek, int & index)
{
	const int pozice = balicek*index;
	switch( switcher )
	{
		case 66:
		{
			int sem = index * balicek;
			for( int i = 0 ; i < balicek ; i++ )
			{
				pArrayOUT[sem] = pArray[delka];
				cnt++;
				delka++;
				sem++;
			}
			break;
		}
		case 64:
			for(int i = 0 ; i < balicek ; i++)
			{
				pPole64[cnt] = pArray[pozice+i];
				cnt += 1;
				if(cnt == delka)
					stretch_pool(delka,64);
			}
			break;
		case 32:
			for(int i = 0 ; i < balicek ; i++)
			{
				pPole32[cnt] = pArray[pozice+i];
				cnt += 1;
				if(cnt == delka)
					stretch_pool(delka,32);
			}
			break;
		case 16:
			for(int i = 0 ; i < balicek ; i++)
			{
				pPole16[cnt] = pArray[pozice+i];
				cnt += 1;
				if(cnt == delka)
					stretch_pool(delka,16);
			}
			break;
		case 8:
			for(int i = 0 ; i < balicek ; i++)
			{
				pPole8[cnt] = pArray[pozice+i];
				cnt += 1;
				if(cnt == delka)
					stretch_pool(delka,8);
			}
			break;
		case 4:
			for(int i = 0 ; i < balicek ; i++)
			{
				pPole4[cnt] = pArray[pozice+i];
				cnt += 1;
				if(cnt == delka)
					stretch_pool(delka,4);
			}
			break;
		case 2:
			for(int i = 0 ; i < balicek ; i++)
			{
				pPole2[cnt] = pArray[pozice+i];
				cnt += 1;
				if(cnt == delka)
					stretch_pool(delka,2);
			}
			break;
		case 1:
			for(int i = 0 ; i < balicek ; i++)
			{
				pPole1[cnt] = pArray[pozice+i];
				cnt += 1;
				if(cnt == delka)
					stretch_pool(delka,1);
			}
			break;
	}
}

void rearrange( const int & strop , const int & balicek , const header & details)
{
	int radek = 1;
	const int sirka = details.width;
	const int vyska = details.length;
	int pozice = 0;//celkova
	bool trigger64 = false,trigger32 = false,trigger16 = false,trigger8 = false,trigger4 = false,trigger2 = false;

	cnt_pole64 = 0,cnt_pole32 = 0,cnt_pole16 = 0,cnt_pole8 = 0,cnt_pole4 = 0,cnt_pole2 = 0,cnt_pole1 = 0;

	int index_v_rade;
	switch(strop)
	{
		case 64:
			trigger64 = true;
		case 32:
			trigger32 = true;
		case 16:
			trigger16 = true;
		case 8:
			trigger8 = true;
		case 4:
			trigger4 = true;
		case 2:
			trigger2 = true;
			break;
	}

	while(1)
	{	
		index_v_rade = 0;
		for (int i = pozice; i < radek*sirka; i++)
		{
			if( trigger64 && index_v_rade%64 == 0 )
				{ fill_it(64,cnt_pole64,pole64_length, balicek,i); index_v_rade++;continue; }
			
			if( trigger32 && index_v_rade%32 == 0 )
				{ fill_it(32,cnt_pole32,pole32_length, balicek,i); index_v_rade++;continue; }
			
			if( trigger16 && index_v_rade%16 == 0 )
				{ fill_it(16,cnt_pole16,pole16_length, balicek,i); index_v_rade++;continue; }
			
			if(trigger8 && index_v_rade%8 == 0 )
				{ fill_it(8,cnt_pole8,pole8_length, balicek,i); index_v_rade++;continue; }
			
			if(trigger4 && index_v_rade%4 == 0 )
				{ fill_it(4,cnt_pole4,pole4_length, balicek,i); index_v_rade++;continue; }
			
			if(trigger2 && index_v_rade%2 == 0 )
				{ fill_it(2,cnt_pole2,pole2_length, balicek,i); index_v_rade++;continue; }
			else
				{ fill_it(1,cnt_pole1,pole1_length, balicek,i); index_v_rade++;continue; }
		}
	trigger64 = false,trigger32 = false,trigger16 = false,trigger8 = false,trigger4 = false,trigger2 = false;
	pozice = radek*sirka;
	radek++;
	if(radek == vyska+1)
		break;
	if( (radek-1)%64 == 0  && strop >= 64)
		{trigger64=true;trigger32 = true; trigger16 = true; trigger8 = true;trigger4=true;trigger2=true;}
	else if( (radek-1)%32 == 0 && strop >= 32)
		{trigger32 = true; trigger16 = true; trigger8 = true;trigger4=true;trigger2=true;}
	else if( (radek-1)%16 == 0 && strop >= 16)
		{trigger16 = true; trigger8 = true;trigger4=true;trigger2=true;}
	else if( (radek-1)%8 == 0 && strop >= 8)
		{trigger8 = true;trigger4=true;trigger2=true;}
	else if( (radek-1)%4 == 0 && strop >= 4)
		{trigger4=true;trigger2=true;}
	else if( (radek-1)%2 == 0 && strop >= 2)
		{trigger2=true;}
	}
}

bool fill_arrayOUT(ofstream & FILE, const int & interleave)
{
	switch( interleave )
	{
		case 64:
			if( !FILE.write( (char*)(pPole64) , cnt_pole64 * sizeof(unsigned char) ) )
				return false;
		case 32:
			if( !FILE.write( (char*)(pPole32) , cnt_pole32 * sizeof(unsigned char) ) )
				return false;
		case 16:
			if( !FILE.write( (char*)(pPole16) , cnt_pole16 * sizeof(unsigned char) ) )
				return false;
		case 8:
			if( !FILE.write( (char*)(pPole8) , cnt_pole8 * sizeof(unsigned char) ) )
				return false;
		case 4:
			if( !FILE.write( (char*)(pPole4) , cnt_pole4 * sizeof(unsigned char) ) )
				return false;
		case 2:
			if( !FILE.write( (char*)(pPole2) , cnt_pole2 * sizeof(unsigned char) ) )
				return false;
		case 1:
			if( !FILE.write( (char*)(pPole1) , cnt_pole1 * sizeof(unsigned char) ) )
				return false;
			break;
	}
	return true;
}

void write_header(ofstream & OUTFILE , header & details, const int & interleave)
{
	OUTFILE << (char)details.endian << (char)(details.endian/( pow(2,8) ) );
	OUTFILE << (char)details.width << (char)(details.width/( pow(2,8) ) ) ;
	OUTFILE << (char)details.length << (char)(details.length/( pow(2,8) ) );
	int binarne = 0;
	switch(interleave)
	{
		case 64:
			binarne = 6;break;
		case 32:
			binarne = 5;break;
		case 16:
			binarne = 4;break;
		case 8:
			binarne = 3;break;
		case 4:
			binarne = 2;break;
		case 2:
			binarne = 1;break;
		case 1:
			binarne = 0;break;
	}
	uint16_t odecist = (details.pixel/pow(2,5) );
	uint16_t nova_hlavicka = details.pixel - odecist*pow(2,5) + binarne*pow(2,5) ;
	OUTFILE << (char)nova_hlavicka << (char)(details.pixel/( pow(2,8) ) );
}

void decode( const int & strop , const int & balicek , const header & details )
{
	int radek = 1;
	const int sirka = details.width;
	const int vyska = details.length;
	bool trigger64 = false,trigger32 = false,trigger16 = false,trigger8 = false,trigger4 = false,trigger2 = false,trigger1 = true;
	switch(strop)
	{
		case 64:
			trigger64 = true;
		case 32:
			trigger32 = true;
		case 16:
			trigger16 = true;
		case 8:
			trigger8 = true;
		case 4:
			trigger4 = true;
		case 2:
			trigger2 = true;
			break;
	}

	for( int i = 0 ; i < balicek ; i++)
	{
		pArrayOUT[i] = pArray[i];
		cnt_ArrayOUT++;
	}

	int posuvnik_Array = balicek;

	if(trigger64)
	{
		radek = 1;
		while( radek <= vyska )
		{
			for (int i = ( (radek-1)  * sirka); i < sirka*radek ; i += 64 )
			{
				if( i == 0 )
					continue;
				fill_it(66,cnt_ArrayOUT,posuvnik_Array,balicek,i);
			}
			radek += 64;
		}
	}//.....
	if(trigger32)
	{
		radek = 1;
		while( radek <= vyska )
		{
			for (int i = ( (radek-1)  * sirka); i < sirka*radek ; i += 32 )
			{
				if( i == 0 )
					continue;
				if( trigger64 )	
					if( (radek-1)%64 == 0 && i%64 == 0 )
						continue;
				fill_it(66,cnt_ArrayOUT,posuvnik_Array,balicek,i);
			}
			radek += 32;
		}
	}//.....
	if(trigger16)
	{
		radek = 1;
		while( radek <= vyska )
		{
			for (int i = ( (radek-1)  * sirka); i < sirka*radek ; i += 16 )
			{
				if( i == 0 )
					continue;
				if( trigger32 )
					if( (radek-1)%32 == 0 && i%32 == 0 )
						continue;
				fill_it(66,cnt_ArrayOUT,posuvnik_Array,balicek,i);
			}
			radek += 16;
		}
	}//.....
	if(trigger8)
	{
		radek = 1;
		while( radek <= vyska )
		{
			for (int i = ( (radek-1)  * sirka); i < sirka*radek ; i += 8 )
			{
				if( i == 0 )
					continue;
				if( trigger16 )
					if( (radek-1)%16 == 0 && i%16 == 0)
						continue;
				fill_it(66,cnt_ArrayOUT,posuvnik_Array,balicek,i);
			}
			radek += 8;
		}
	}//.....
	if(trigger4)
	{
		radek = 1;
		while( radek <= vyska )
		{
			for (int i = ( (radek-1)  * sirka); i < sirka*radek ; i += 4 )
			{
				if( i == 0 )
					continue;
				if( trigger8 )
					if( (radek-1)%8 == 0 && i%8 == 0 )
						continue;
				fill_it(66,cnt_ArrayOUT,posuvnik_Array,balicek,i);
			}
			radek += 4;
		}
	}//.....
	if(trigger2)
	{
		radek = 1;
		while( radek <= vyska )
		{
			for (int i = ( (radek-1) * sirka); i < sirka*radek ; i += 2 )
			{
				if( i == 0 )
					continue;
				if( trigger4 )
					if( (radek-1)%4 == 0 && i%4 == 0 )
						continue;
				fill_it(66,cnt_ArrayOUT,posuvnik_Array,balicek,i);
			}
			radek += 2;
		}
	}//.....
	if(trigger1)
	{
		radek = 1;
		while( radek <= vyska )
		{
			for (int i = ( (radek-1)  * sirka); i < sirka*radek ; i += 1 )
			{
				if( i == 0 )
					continue;
				if( (radek-1)%2 == 0 && i%2 == 0 )
					continue;
				fill_it(66,cnt_ArrayOUT,posuvnik_Array,balicek,i);
			}
			radek++;
		}
	}
}

void dealocate()
{
	delete[] pPole64;
	delete[] pPole32;
	delete[] pPole16;
	delete[] pPole8;
	delete[] pPole4;
	delete[] pPole2;
	delete[] pPole1;
}

bool recodeImage ( const char  * srcFileName,
                   const char  * dstFileName,
                   int           interleave,
                   uint16_t      byteOrder )
{
	if( !header_control(interleave,byteOrder) )//kontrola spravnosti parametru
	{
		if( header_control(2,byteOrder) )
			{
				ofstream OUTFILE;
				OUTFILE.open(dstFileName,ios::binary | ios:: out);
				if( !OUTFILE.is_open() )
					return false;
				OUTFILE.close();

				return false;
			}
		else
			return false;
	}

	ifstream INFILE;
	INFILE.open( srcFileName , ios::in | ios::binary);

	INFILE.seekg (0, INFILE.end);
	const int puvodni_velikost = INFILE.tellg();
	INFILE.seekg (0, INFILE.beg);

	if( !INFILE.is_open() )
		return false;

	header details;//zde uchovam hlavicku

	if ( !(INFILE.read( (char*)( &details) , sizeof(details) ) ) )
		return false;

	if( !header_control(2,details.endian) || details.width <= 0 || details.length <= 0 )//kontrola spatny header
		{INFILE.close();return false;}

	if ( details.pixel >= 224 )
		{INFILE.close();return false;}
	int prokladani = -1 ,kanaly = -1 ,bity_na_kanal = -1 ;
	decider_prokladani(details.pixel,prokladani,bity_na_kanal,kanaly);//z 2B formatu ziska dulezite info
	if( prokladani <= 0 || kanaly <= 0 || bity_na_kanal <= 0 )
		{INFILE.close();return false;}

	//ted zkontroluji jestli je tam dostatek pixelu na rozmery
	const int pixel_cnt = details.width * details.length; // pocet pixelu kolik jich tam ma byt
	const int balicek = ((bity_na_kanal/8) * kanaly);//vyjadri kolik charu (bajtu) ma jeden pixel
	const int ted = INFILE.tellg();
	if( (pixel_cnt * balicek) != (puvodni_velikost - ted) )
		{INFILE.close();return false;}

	//zde fkce co nahraje hlavicku do souboru
	ofstream OUTFILE;
	OUTFILE.open( dstFileName , ios::out | ios:: binary );
	if( !OUTFILE.is_open() )
		{INFILE.close();return false;}

	write_header(OUTFILE,details,interleave);

	if( bity_na_kanal == 1 )//jeste neumi pro tohle to resit -> a nebudu :D
	{
		OUTFILE.close();
		INFILE.close();
		return false;
	}

	int delka_arrayIN;
	cnt_ArrayIN = 0;

	pArray = new unsigned char[puvodni_velikost - ted];

	//fkce pro nacteni do pArray z input_file
	if( !load_input(INFILE , cnt_ArrayIN , delka_arrayIN, puvodni_velikost - ted) )
		return false;


	if( prokladani == interleave )
	{
		if( !OUTFILE.write( (char*)(pArray) , cnt_ArrayIN * sizeof(unsigned char) ) )
			return false;
		
		OUTFILE.close();
		INFILE.close();
		delete[] pArray;
		return true;
	}

	pPole64 = create_array_pool(pole64_length);
	pPole32 = create_array_pool(pole32_length);
	pPole16 = create_array_pool(pole16_length);
	pPole8 = create_array_pool(pole8_length);
	pPole4 = create_array_pool(pole4_length);
	pPole2 = create_array_pool(pole2_length);
	pPole1 = create_array_pool(pole1_length);

	INFILE.close();
	
	if(prokladani == 1)
	{
		rearrange(interleave,balicek,details);
		if( !fill_arrayOUT(OUTFILE , interleave) )
			return false;
		delete[] pArray;
	}
	else
	{
		pArrayOUT = new unsigned char[cnt_ArrayIN];
		cnt_ArrayOUT = 0;
		decode(prokladani , balicek , details);
		if( interleave == 1 )
		{
			if( !OUTFILE.write( (char*)(pArrayOUT) , cnt_ArrayOUT * sizeof(unsigned char) ) )
				return false;

			dealocate();
			delete[] pArrayOUT;
			delete[] pArray;
			OUTFILE.close();
			return true;
		}
		delete[] pArray;
		pArray = pArrayOUT;
		rearrange(interleave,balicek,details);
		if( !fill_arrayOUT(OUTFILE , interleave) )
			return false;
		delete[] pArrayOUT;
	}

	dealocate();

	OUTFILE.close();
	return true;
}

#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1,
                      const char * fileName2 )
{
	return true;
}

int main ( void )
{
  assert ( recodeImage ( "input_00.img", "output_00.img", 1, ENDIAN_LITTLE )
          && identicalFiles ( "output_00.img", "ref_00.img" ) );
  assert ( recodeImage ( "input_01.img", "output_01.img", 8, ENDIAN_LITTLE )
          && identicalFiles ( "output_01.img", "ref_01.img" ) );

  assert ( recodeImage ( "input_02.img", "output_02.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "output_02.img", "ref_02.img" ) );

  assert ( recodeImage ( "input_03.img", "output_03.img", 2, ENDIAN_LITTLE )
           && identicalFiles ( "output_03.img", "ref_03.img" ) );

  assert ( recodeImage ( "input_04.img", "output_04.img", 1, ENDIAN_LITTLE )
           && identicalFiles ( "output_04.img", "ref_04.img" ) );

  assert ( recodeImage ( "input_05.img", "output_05.img", 1, ENDIAN_LITTLE )
           && identicalFiles ( "output_05.img", "ref_05.img" ) );

  assert ( recodeImage ( "input_06.img", "output_06.img", 8, ENDIAN_LITTLE )
           && identicalFiles ( "output_06.img", "ref_06.img" ) );

  assert ( recodeImage ( "input_07.img", "output_07.img", 4, ENDIAN_LITTLE )
           && identicalFiles ( "output_07.img", "ref_07.img" ) );

 assert ( recodeImage ( "input_08.img", "output_08.img", 8, ENDIAN_LITTLE )
          && identicalFiles ( "output_08.img", "ref_08.img" ) );

  assert ( ! recodeImage ( "input_09.img", "output_09.img", 1, ENDIAN_LITTLE ) );

  assert ( ! recodeImage ( "input_10.img", "output_10.img", 5, ENDIAN_LITTLE ) );

  assert ( recodeImage( "input_11.img", "output_11.img", 32, ENDIAN_LITTLE) );

  // extra inputs (optional & bonus tests)
  // assert ( recodeImage ( "extra_input_00.img", "extra_out_00.img", 8, ENDIAN_LITTLE )
  //          && identicalFiles ( "extra_out_00.img", "extra_ref_00.img" ) );
  // assert ( recodeImage ( "extra_input_01.img", "extra_out_01.img", 4, ENDIAN_BIG )
  //          && identicalFiles ( "extra_out_01.img", "extra_ref_01.img" ) );
  // assert ( recodeImage ( "extra_input_02.img", "extra_out_02.img", 16, ENDIAN_BIG )
  //          && identicalFiles ( "extra_out_02.img", "extra_ref_02.img" ) );
  // assert ( recodeImage ( "extra_input_03.img", "extra_out_03.img", 1, ENDIAN_LITTLE )
  //          && identicalFiles ( "extra_out_03.img", "extra_ref_03.img" ) );
  // assert ( recodeImage ( "extra_input_04.img", "extra_out_04.img", 8, ENDIAN_LITTLE )
  //          && identicalFiles ( "extra_out_04.img", "extra_ref_04.img" ) );
  // assert ( recodeImage ( "extra_input_05.img", "extra_out_05.img", 4, ENDIAN_LITTLE )
  //          && identicalFiles ( "extra_out_05.img", "extra_ref_05.img" ) );
  // assert ( recodeImage ( "extra_input_06.img", "extra_out_06.img", 16, ENDIAN_BIG )
  //          && identicalFiles ( "extra_out_06.img", "extra_ref_06.img" ) );
  // assert ( recodeImage ( "extra_input_07.img", "extra_out_07.img", 1, ENDIAN_BIG )
  //          && identicalFiles ( "extra_out_07.img", "extra_ref_07.img" ) );
  // assert ( recodeImage ( "extra_input_08.img", "extra_out_08.img", 8, ENDIAN_LITTLE )
  //          && identicalFiles ( "extra_out_08.img", "extra_ref_08.img" ) );
  // assert ( recodeImage ( "extra_input_09.img", "extra_out_09.img", 4, ENDIAN_LITTLE )
  //          && identicalFiles ( "extra_out_09.img", "extra_ref_09.img" ) );
  // assert ( recodeImage ( "extra_input_10.img", "extra_out_10.img", 16, ENDIAN_BIG )
  //          && identicalFiles ( "extra_out_10.img", "extra_ref_10.img" ) );
  // assert ( recodeImage ( "extra_input_11.img", "extra_out_11.img", 1, ENDIAN_BIG )
  //          && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );
  return 0;
}
#endif /* __PROGTEST__ */