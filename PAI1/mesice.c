#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#define INVALID_DATE (-1)
#endif /* __PROGTEST__ */
//-----------------------------------------------------------------------------
//urcuje zda je rok prestupny
int je_prestupny(const int year)
{
	if (year%4==0)
	{
		if(year%100==0 && year%400!=0)		
		{return 0;}

		if (year%4000==0)
		{
			return 0;
		}
		
		return 1;
	}
	return 0;
}
//----------------------------------------------------------------------------

//kontroluje zadana data
int is_y_m_input_ok(int year,int month)
{
	if (year<2000 || month>12 || month < 1)
	{
		return 1;
	}
	return 0;
}

int is_d_input_ok(const int month,const int d_input,const int days_in_year)
{
	if(d_input<0 || d_input>31)
		return 1;
	switch(month)
	{
		case 4:
			if (d_input>30)
				return 1;
		case 6:
			if (d_input>30)
				return 1;
		case 9:
			if (d_input>30)
				return 1;
		case 11:
			if (d_input>30)
				return 1;
	}

	if (days_in_year==366)
		if(month==2 && d_input>29)
			return 1;
	if (days_in_year==365)
		if (month==2 && d_input>28)
			return 1;
	return 0;
}

int input_check(const int y,const int m,const int d)
{
	int days_in_year;
	if(is_y_m_input_ok(y,m))
		return 0;

	if(je_prestupny(y))
  		days_in_year=366;
  	else
  		days_in_year=365;
	if (is_d_input_ok(m,d,days_in_year))
		return 0;

	return 1;	
}
//------------------------------------------------------------------------------

//pocita vteriny od 1.1.0. do prijateho data
void sec_since_1_1_0(const int year,const int month,const int day, uint64_t *celkem_in_sec)
{
  int february;
  if (je_prestupny(year)==1)
    february=29;
  else {february=28; }
  int months_array[]={31,february,31,30,31,30,31,31,30,31,30,31};

  int prestupny_rok=366;
  int normalni_rok=365;
  uint64_t year_in_sec=0;
  for (int i = 1; i<year; i++)
  {
    if(je_prestupny(i)==1)
    {
      year_in_sec += prestupny_rok;
    }
    else
      year_in_sec += normalni_rok;
  }
  year_in_sec= (year_in_sec*24*60*60) ;

  uint64_t month_in_sec=0;
  for (int i = 0; i < month-1; ++i)
  {
    month_in_sec += months_array[i];
  }
  month_in_sec=month_in_sec*24*60*60;

  uint64_t day_in_sec=day-1;
  day_in_sec=day_in_sec*24*60*60;

  *celkem_in_sec= day_in_sec+month_in_sec+year_in_sec;
}
//--------------------------------------------------------------------------------
//vypocita vzdalenost od 1.1.0 do referencniho uplnku
void referencni_uplnek(uint64_t *output)
{
	uint64_t moon_period=(29*24*60*60+12*60*60+44*60+3);
	sec_since_1_1_0(2000,7,16,output);
	*output += (13*60*60+55*60+12)-10*moon_period;
	
}
//---------------------------------------------------------------------------------
//date to sec
void sec_to_date(int *prevy,int *prevm,int *prevd, uint64_t input_sec)
{
  uint64_t input_days=input_sec/86400;
  const int nmb_of_years[]={4000,400,100,4,1};
  const uint64_t array[]={1460969, 146097, 36524,1461,365};
  const int max_nasobek[]={-1,-1,4,-1,4};
  int pocitadlo[]={0,0,0,0,0};

  //const int yearsAndDaysArrLength = sizeof(yearsAndDaysArr) / sizeof(*yearsAndDaysArr);

  
  for(int i=0;i<5; i++)
  {
    while (input_days >= array[i])
    {   
      input_days -= array[i];
      pocitadlo[i]++;
    }
      if (pocitadlo[i]==max_nasobek[i])
      {
       pocitadlo[i]--;
       input_days += array[i];
      }

  }

  int estimated_year=0;
  for (int i = 0; i < 5; ++i)
    estimated_year = estimated_year + nmb_of_years[i]*pocitadlo[i];
  
  *prevy=estimated_year+1;

  //mesice
  int february;
  if (je_prestupny(estimated_year+1)==1)
    february=29;
  else 
    february=28;
  int months_array[]={31,february,31,30,31,30,31,31,30,31,30,31};
  int estimated_month=1;

  for(int i=0;i<12; i++)
  {
    if (input_days>=(uint64_t)months_array[i])
    {
      estimated_month++;
      input_days -= months_array[i];
    } else {
        break;
    }
  }
//  printf("year%d ", estimated_year+1);
//  printf("| month%d ", estimated_month);
  *prevm=estimated_month;
//a ted den
    *prevd=input_days+1;
//  printf("| day%d\n\n", input_days+1);
}


int isFullMoon (const int y,const int m,const int d )
{
	//kontrola platnosti zadaneho data
	if(input_check(y,m,d)==0)
		return INVALID_DATE;
	//vypocet
	uint64_t SecondsSinceYear0=0;
	sec_since_1_1_0(y,m,d,&SecondsSinceYear0);

	uint64_t From0ToFirstMoonInSeconds = 0;
	referencni_uplnek( &From0ToFirstMoonInSeconds );
	
	uint64_t moon_period=(29*24*60*60+12*60*60+44*60+3);
	

	while( From0ToFirstMoonInSeconds < SecondsSinceYear0 )
	{
		From0ToFirstMoonInSeconds += moon_period;
	}	
	if ((From0ToFirstMoonInSeconds - SecondsSinceYear0) <= 86400)
		return 1;
	
	// uint64_t output= (SecondsSinceYear0- From0ToFirstMoonInSeconds)/moon_period;
	// if( (SecondsSinceYear0- From0ToFirstMoonInSeconds)-(output*moon_period%moon_period)<=86400)	
	// 	return 1;
	
	return 0;

}

//-----------------------------------------------------------------------------


int prevFullMoon ( int y, int m, int d,
                   int * prevY, int * prevM, int * prevD )
{
		if(input_check(y,m,d)==0)
				return INVALID_DATE;

	uint64_t moon_period=(29*24*60*60+12*60*60+44*60+3);

	if(isFullMoon(y,m,d)==1)
	{	
		uint64_t From0toObtainedYear=0;
		sec_since_1_1_0(y,m,d,&From0toObtainedYear);


		uint64_t From0ToFirstMoonInSeconds = 0;
		referencni_uplnek( &From0ToFirstMoonInSeconds );
		while( From0ToFirstMoonInSeconds < From0toObtainedYear )
		{
			From0ToFirstMoonInSeconds += moon_period;
		}
		uint64_t prevmoon= From0ToFirstMoonInSeconds - moon_period;
		sec_to_date(prevY,prevM,prevD,prevmoon);
		return 1;
	}
	else
	{
		//uryvek kodu z funkce isFullMoon
		uint64_t From0toObtainedYear=0;
		sec_since_1_1_0(y,m,d,&From0toObtainedYear);

		uint64_t From0ToFirstMoonInSeconds = 0;
		referencni_uplnek( &From0ToFirstMoonInSeconds );
		while( From0ToFirstMoonInSeconds < From0toObtainedYear )
		{
			From0ToFirstMoonInSeconds += moon_period;
		}
		uint64_t prevmoon= From0ToFirstMoonInSeconds - moon_period;
		sec_to_date(prevY,prevM,prevD,prevmoon);
	}
	return 1;
}

int nextFullMoon ( int y, int m, int d,
                   int * nextY, int * nextM, int * nextD )
{
  if(input_check(y,m,d)==0)
				return INVALID_DATE;

	uint64_t moon_period=(29*24*60*60+12*60*60+44*60+3);
	
	//printf("%d\n", isFullMoon(y,m,d));

	if(isFullMoon(y,m,d)==1)
	{	
		uint64_t From0toObtainedYear=0;
		sec_since_1_1_0(y,m,d,&From0toObtainedYear);


		uint64_t From0ToFirstMoonInSeconds = 0;
		referencni_uplnek( &From0ToFirstMoonInSeconds );
		while( From0ToFirstMoonInSeconds < From0toObtainedYear )
		{
			From0ToFirstMoonInSeconds += moon_period;
		}
		uint64_t prevmoon= From0ToFirstMoonInSeconds + moon_period;
		sec_to_date(nextY,nextM,nextD,prevmoon);
		return 1;
	}
	else
	{
		//uryvek kodu z funkce isFullMoon
		uint64_t From0toObtainedYear=0;
		sec_since_1_1_0(y,m,d,&From0toObtainedYear);

		uint64_t From0ToFirstMoonInSeconds = 0;
		referencni_uplnek( &From0ToFirstMoonInSeconds );
		while( From0ToFirstMoonInSeconds < From0toObtainedYear )
		{
			From0ToFirstMoonInSeconds += moon_period;
		}
		uint64_t prevmoon= From0ToFirstMoonInSeconds;
		sec_to_date(nextY,nextM,nextD,prevmoon);
	}
	return 1;




}

//-------------------------------------------------------------------------------
#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  int y, m, d;


  assert ( prevFullMoon ( 2017, 1, 12, &y, &m, &d ) == 1 && y == 2016 && m == 12 && d == 14 );

  assert ( isFullMoon ( 2000, 7, 16 ) == 1 );
  assert ( isFullMoon ( 2000, 7, 15 ) == 0 );
  assert ( isFullMoon ( 2000, 7, 14 ) == 0 );
  assert ( isFullMoon ( 2000, 6, 30 ) == 0 );
  assert ( isFullMoon ( 2000, 2, 2 ) == 0 );
  assert ( isFullMoon ( 2017, 1, 11 ) == 0 );
  assert ( isFullMoon ( 2017, 1, 12 ) == 1 );
  assert ( isFullMoon ( 2017, 1, 13 ) == 0 );
  assert ( isFullMoon ( 2017, 11, 3 ) == 0 );
  assert ( isFullMoon ( 2017, 11, 4 ) == 1 );
  assert ( isFullMoon ( 2017, 11, 5 ) == 0 );
  assert ( isFullMoon ( 2019, 12, 11 ) == 1 );
  assert ( isFullMoon ( 2019, 12, 12 ) == 0 );
  assert ( isFullMoon ( 2019, 12, 13 ) == 0 );
  assert ( prevFullMoon ( 2017, 1, 11, &y, &m, &d ) == 1 && y == 2016 && m == 12 && d == 14 );
  assert ( prevFullMoon ( 2017, 1, 12, &y, &m, &d ) == 1 && y == 2016 && m == 12 && d == 14 );
  assert ( prevFullMoon ( 2017, 1, 13, &y, &m, &d ) == 1 && y == 2017 && m == 1 && d == 12 );
  assert ( prevFullMoon ( 2017, 11, 3, &y, &m, &d ) == 1 && y == 2017 && m == 10 && d == 5 );
  assert ( prevFullMoon ( 2017, 11, 4, &y, &m, &d ) == 1 && y == 2017 && m == 10 && d == 5 );
  assert ( prevFullMoon ( 2017, 11, 5, &y, &m, &d ) == 1 && y == 2017 && m == 11 && d == 4 );
  assert ( prevFullMoon ( 2019, 12, 11, &y, &m, &d ) == 1 && y == 2019 && m == 11 && d == 12 );
  assert ( prevFullMoon ( 2019, 12, 12, &y, &m, &d ) == 1 && y == 2019 && m == 12 && d == 11 );
  assert ( prevFullMoon ( 2019, 12, 13, &y, &m, &d ) == 1 && y == 2019 && m == 12 && d == 11 );
  assert ( nextFullMoon ( 2017, 1, 11, &y, &m, &d ) == 1 && y == 2017 && m == 1 && d == 12 );
  assert ( nextFullMoon ( 2017, 1, 12, &y, &m, &d ) == 1 && y == 2017 && m == 2 && d == 11 );
  assert ( nextFullMoon ( 2017, 1, 13, &y, &m, &d ) == 1 && y == 2017 && m == 2 && d == 11 );
  assert ( nextFullMoon ( 2017, 11, 3, &y, &m, &d ) == 1 && y == 2017 && m == 11 && d == 4 );
  assert ( nextFullMoon ( 2017, 11, 4, &y, &m, &d ) == 1 && y == 2017 && m == 12 && d == 3 );
  assert ( nextFullMoon ( 2017, 11, 5, &y, &m, &d ) == 1 && y == 2017 && m == 12 && d == 3 );
  assert ( nextFullMoon ( 2019, 12, 11, &y, &m, &d ) == 1 && y == 2020 && m == 1 && d == 10 );
  assert ( nextFullMoon ( 2019, 12, 12, &y, &m, &d ) == 1 && y == 2020 && m == 1 && d == 10 );
  assert ( nextFullMoon ( 2019, 12, 13, &y, &m, &d ) == 1 && y == 2020 && m == 1 && d == 10 );
  assert ( isFullMoon ( 2017, 1, 12 ) == 1 );
  assert ( isFullMoon ( 2017, 2, 11 ) == 1 );
  assert ( isFullMoon ( 2017, 3, 12 ) == 1 );
  assert ( isFullMoon ( 2017, 4, 11 ) == 1 );
  assert ( isFullMoon ( 2017, 5, 10 ) == 1 );
  assert ( isFullMoon ( 2017, 6, 9 ) == 1 );
  assert ( isFullMoon ( 2017, 7, 9 ) == 1 );
  assert ( isFullMoon ( 2017, 8, 7 ) == 1 );
  assert ( isFullMoon ( 2017, 9, 6 ) == 1 );
  assert ( isFullMoon ( 2017, 10, 5 ) == 1 );
  assert ( isFullMoon ( 2017, 11, 4 ) == 1 );
  assert ( isFullMoon ( 2017, 12, 3 ) == 1 );
  assert ( isFullMoon ( 2018, 1, 2 ) == 1 );
  assert ( isFullMoon ( 2018, 1, 31 ) == 1 );
  assert ( isFullMoon ( 2018, 3, 2 ) == 1 );
  assert ( isFullMoon ( 2018, 3, 31 ) == 1 );
  assert ( isFullMoon ( 2018, 4, 30 ) == 1 );
  assert ( isFullMoon ( 2018, 5, 29 ) == 1 );
  assert ( isFullMoon ( 2018, 6, 28 ) == 1 );
  assert ( isFullMoon ( 2018, 7, 27 ) == 1 );
  assert ( isFullMoon ( 2018, 8, 26 ) == 1 );
  assert ( isFullMoon ( 2018, 9, 25 ) == 0 );
  assert ( isFullMoon ( 2018, 10, 24 ) == 1 );
  assert ( isFullMoon ( 2018, 11, 23 ) == 1 );
  assert ( isFullMoon ( 2018, 12, 22 ) == 1 );
  assert ( isFullMoon ( 2019, 1, 21 ) == 1 );
  assert ( isFullMoon ( 2019, 2, 19 ) == 1 );
  assert ( isFullMoon ( 2019, 3, 21 ) == 1 );
  assert ( isFullMoon ( 2019, 4, 19 ) == 1 );
  assert ( isFullMoon ( 2019, 5, 18 ) == 0 );
  assert ( isFullMoon ( 2019, 6, 17 ) == 1 );
  assert ( isFullMoon ( 2019, 7, 16 ) == 0 );
  assert ( isFullMoon ( 2019, 8, 15 ) == 1 );
  assert ( isFullMoon ( 2019, 9, 14 ) == 1 );
  assert ( isFullMoon ( 2019, 10, 13 ) == 1 );
  assert ( isFullMoon ( 2019, 11, 12 ) == 1 );
  assert ( isFullMoon ( 2019, 12, 12 ) == 0 );
  assert ( isFullMoon ( 2000, 14, 10 ) == INVALID_DATE );
 assert ( prevFullMoon ( 2000, 11, 31, &y, &m, &d ) == INVALID_DATE );
  assert ( nextFullMoon ( 2001, 2, 29, &y, &m, &d ) == INVALID_DATE );
  assert ( isFullMoon ( 2004, 2, 29 ) == 0 );
  assert ( prevFullMoon ( 2100, 2, 29, &y, &m, &d ) == INVALID_DATE );
  assert ( nextFullMoon ( 2000, 2, 29, &y, &m, &d ) == 1 && y == 2000 && m == 3 && d == 20 );
  return 0;
}
#endif /* __PROGTEST__ */
