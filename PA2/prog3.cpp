#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
class InvalidDateException
{
};
#endif /* __PROGTEST__ */
//-------------------------------------------------------------------------------------------------
const int DAYS_IN4k = 1460969 ;
const int DAYS_IN400 = 146097 ;
const int DAYS_IN100 = 36524 ;
const int DAYS_IN4 = 1461 ;
//-------------------------------------------------------------------------------------------------
struct zasobnik{
  int den;
  int mesic;
  int rok;
};

  vector<zasobnik> Array;
  class Year;
  class Month;
  class Day;


class Day{
public:
	Day( const int day ) : m_Day(day) {}
//-------------------------------------------------
  // friend  vector<zasobnik> operator +( Day y , Month x );
Day operator -()const
  {
    Day novy(-(this->m_Day) );
    return novy;
  }
  vector<zasobnik> operator +(const  Month x )const ;
  vector<zasobnik> operator +(const  Year x )const ;
  vector<zasobnik> operator -(const  Month x )const ;
  vector<zasobnik> operator -(const  Year x )const ;
  
  vector<zasobnik> operator +(const  Day x )const 
  {
    zasobnik novy;
    novy.rok = 0;
    novy.den = x.m_Day + this->m_Day;
    novy.mesic = 0;
    Array.push_back(novy);
    return Array;
  }
  vector<zasobnik> operator -(const  Day x )const
  {
    zasobnik novy;
    if( this->m_Day > x.m_Day )
    {
      novy.den = this->m_Day - x.m_Day;
    }
    else
    {
      novy.den = -( this->m_Day - x.m_Day );
    }
    novy.rok = 0;
    novy.mesic = 0;
    Array.push_back(novy);
    return Array;
  }
//-------------------------------------------------
	int m_Day;
};

class Month{
public:
	Month(const  int month ) : m_Month(month) {}
//-------------------------------------------------
Month operator -()const 
  {
    Month novy(-(this->m_Month) );
    return novy;
  }
  vector<zasobnik> operator +(const  Day x )const 
  {
    zasobnik novy;
    novy.rok = 0;
    novy.den = x.m_Day;
    novy.mesic = this->m_Month;
    Array.push_back(novy);
    return Array;
  }
  vector<zasobnik> operator -(const  Day x )const 
  {
    zasobnik novy;
    novy.rok = 0;
    novy.den = -(x.m_Day);
    novy.mesic = this->m_Month;
    Array.push_back(novy);
    return Array;
  }

  vector<zasobnik> operator +(const  Month x )const 
  {
    zasobnik novy;
    novy.rok = 0;
    novy.den = 0;
    novy.mesic = ( this->m_Month + x.m_Month );
    Array.push_back(novy);
    return Array;
  }
  vector<zasobnik> operator -(const  Month x )const 
  {
    zasobnik novy;
    novy.rok = 0;
    novy.den = 0;
    if( this->m_Month > x.m_Month )
    {
      novy.mesic = this->m_Month - x.m_Month;
    }
    else
    {
      novy.mesic = -( this->m_Month - x.m_Month );
    }

    Array.push_back(novy);
    return Array;
  }


  vector<zasobnik> operator +(const  Year x )const ;
  vector<zasobnik> operator -(const  Year x )const ;
//-------------------------------------------------
	int m_Month;
};

class Year{
public:
	Year(const  int year ) : m_Year(year) {}
//-------------------------------------------------
  vector<zasobnik> operator +(const  Day x )const 
  {
    zasobnik novy;
    novy.rok = this->m_Year;
    novy.den = x.m_Day;
    novy.mesic = 0;
    Array.push_back(novy);
    return Array;
  }
  vector<zasobnik> operator +(const  Month x )const 
  {
    zasobnik novy;
    novy.rok = this->m_Year;
    novy.den = 0;
    novy.mesic = x.m_Month;
    Array.push_back(novy);
    return Array;
  }
  //--
  vector<zasobnik> operator -(const  Day x )const 
  {
    zasobnik novy;
    novy.rok = this->m_Year;
    novy.den = -(x.m_Day);
    novy.mesic = 0;
    Array.push_back(novy);
    return Array;
  }
  vector<zasobnik> operator -(const  Month x )const 
  {
    zasobnik novy;
    novy.rok = this->m_Year;
    novy.den = 0;
    novy.mesic = -(x.m_Month);
    Array.push_back(novy);
    return Array;
  }
  //----
  vector<zasobnik> operator +(const  Year x )const 
  {
    zasobnik novy;
    novy.rok = this->m_Year + x.m_Year;
    novy.den = 0;
    novy.mesic = 0;
    Array.push_back(novy);
    return Array;
  }
  vector<zasobnik> operator -(const  Year x )const 
  {
    zasobnik novy;
    novy.mesic = 0;
    novy.den = 0;
    if( this->m_Year > x.m_Year )
    {
      novy.rok = this->m_Year - x.m_Year;
    }
    else
    {
      novy.rok = -( this->m_Year - x.m_Year );
    }

    Array.push_back(novy);
    return Array;
  }

  //--
  Year operator -()const 
  {
    Year novy(-(this->m_Year) );
    return novy;
  }
//-------------------------------------------------
	int m_Year;
};
//-------------------------------------------------------------------------------------------------
bool Months_days_check(const int & month , const int & days)
{
	if( month == 4 || month == 6 || month == 9 || month == 11 )
	{
		if( days > 30 )
			return false;
	}
	return true;
}

bool isLeapYear(const int & year )
{
	if( year%4000 == 0 )
		return false;
	if( year%400 == 0 )
		return true;
	if( year%100 == 0 )
		return false;
	if( year%4 == 0 )
		return true;

	return false;
}

bool isDateValid( const Day & StDay , const Month & StMonth , const Year & StYear )
{
	if( StDay.m_Day > 31 || StDay.m_Day <= 0 )
		return false;
	if( StMonth.m_Month > 12 || StMonth.m_Month <= 0 )
		return false;
	if( StYear.m_Year < 1600 )
		return false;

	bool leap = isLeapYear(StYear.m_Year);
	if( StMonth.m_Month == 2 && ( ( leap && StDay.m_Day > 29) || ( !leap && StDay.m_Day > 28 ) ) )// dej do jedne podminky
			return false;
	if( !Months_days_check( StMonth.m_Month , StDay.m_Day ) )
		return false;
	return true;
}

int how_many_days_in_cur_month( const int & month , const int & year )
{
  if( month == 2 )
  {
    if( isLeapYear(year) )
      return 29;
    else
      return 28;
  }
  if( month == 4 || month == 6 || month == 9 || month == 11 )
    return 30;
  return 31;
}

class CDate{
public:
    CDate( const int year , const int month ,const  int day )
    	: date_Day(day) , date_Month(month) , date_Year(year)
    	{
    		if ( !isDateValid( date_Day , date_Month , date_Year ) )
    			throw InvalidDateException();
    	} 
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
    CDate operator + (const  Year value )const 
    {
    	Year copy(value);
    	copy.m_Year += date_Year.m_Year;
    	if( !isDateValid( date_Day , date_Month , copy ) )
    	{	
    		throw InvalidDateException();
    		return *this;
    	}
    	// date_Year.m_Year = copy.m_Year;
    	// return *this;
      CDate result( copy.m_Year , date_Month.m_Month , date_Day.m_Day );
      return result;
    }
    CDate operator + (const  Month value )const 
    {
//cout << "Dostal jsem-> rok: " << date_Year.m_Year << " mesic: " << date_Month.m_Month << " mesic k odectu: " << value.m_Month << endl;
    	Month copyMonth(value);
    	Year copyYear(date_Year.m_Year);
    	if( copyMonth.m_Month > 0 )
        copyMonth.m_Month += date_Month.m_Month;
    		
      int years_to_add = copyMonth.m_Month/12;
    	copyYear.m_Year += years_to_add;
    	copyMonth.m_Month = copyMonth.m_Month - (years_to_add*12);
    	if( copyMonth.m_Month == 0 )
    	{
    		copyMonth.m_Month = 12;
    		copyYear.m_Year -= 1;
    	}
//cout << "RADEK147-> rok: " << copyYear.m_Year << " mesic: " << date_Month.m_Month << " mesic k odectu: " << copyMonth.m_Month << endl;
     if( copyMonth.m_Month < 0 ){ 
        int remove = date_Month.m_Month;
        while( copyMonth.m_Month < 0 )
        {
          remove--;
          copyMonth.m_Month++;
          if( remove == 0 )
          {
            remove = 12;
            copyYear.m_Year--;
          }
        } 
      copyMonth.m_Month = remove;
    }
    	if( !isDateValid( date_Day , copyMonth , copyYear ) )
    	{	
    		throw InvalidDateException();
    		return *this;
    	}
    	// date_Month.m_Month = copyMonth.m_Month;
    	// date_Year.m_Year = copyYear.m_Year;
    	// return *this;
      CDate result( copyYear.m_Year , copyMonth.m_Month , date_Day.m_Day );
      return result;
    }
    CDate operator + (const  Day value )const 
    {
		  CDate copy( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
	if(value.m_Day > 0){
  	  long long int dny = value.m_Day + copy.date_Day.m_Day - 1 ;
      //cout << "na zacatku dnu: " << dny << endl;

      while( !isLeapYear(copy.date_Year.m_Year) && dny >= 365 )
      {
        dny -= 365;;
        copy.date_Year.m_Year++;
      }
      while( copy.date_Year.m_Year%100 != 0 && dny >= DAYS_IN4 )
      {
        dny -= DAYS_IN4;
        copy.date_Year.m_Year += 4;
      }
      while( copy.date_Year.m_Year%400 != 0 && dny >= DAYS_IN100 )
      {
        dny -= DAYS_IN100;
        copy.date_Year.m_Year += 100;
      }
      while( copy.date_Year.m_Year%4000 != 0 && dny >= DAYS_IN400 )
      {
        dny -= DAYS_IN400;
        copy.date_Year.m_Year += 400;
      }


      while( dny >= DAYS_IN4k && copy.date_Year.m_Year%4000 == 0 )
      {
        copy.date_Year.m_Year += 4000;
        dny -= DAYS_IN4k;
        //cout << "4k"<< endl;
      } 
      while( copy.date_Year.m_Year%400 == 0 && dny >= DAYS_IN400 )
      {
       copy.date_Year.m_Year += 400;
       dny -= DAYS_IN400;
       //cout << "400" << endl;
      }
      while( copy.date_Year.m_Year%100 == 0 && dny >= DAYS_IN100 )
      {
        copy.date_Year.m_Year += 100;
        dny -= DAYS_IN100;
        //cout << "100" << endl;
      }
      while( copy.date_Year.m_Year%4 == 0 && dny >= DAYS_IN4 )
      {
        copy.date_Year.m_Year += 4;
        dny -= DAYS_IN4;
        //cout << "4" << endl;
      }
      while( dny >= 365 )
      {
        copy.date_Year.m_Year += 1;
        dny -= 365;
      }

      while( dny >= 0 )
      {
        int days_in_month = how_many_days_in_cur_month( copy.date_Month.m_Month , copy.date_Year.m_Year ); 
        if( dny >= days_in_month )
        {
          dny -= days_in_month;
          copy.date_Month.m_Month++;
          if( copy.date_Month.m_Month > 12 )
          {
            copy.date_Year.m_Year++;
            copy.date_Month.m_Month = 1;
          }
        }
        else
        {
          copy.date_Day.m_Day = dny + 1;
          dny = -1;
        }
      }
    }
    else
    {
      long long int dny = value.m_Day;
      while( dny < 0 )
      {
        copy.date_Day.m_Day--;
        dny++;
        if( copy.date_Day.m_Day == 0 )
        {
          copy.date_Month.m_Month--;
          if( copy.date_Month.m_Month == 0 )
          {
            copy.date_Month.m_Month = 12;
            copy.date_Year.m_Year--;
          }
          copy.date_Day.m_Day = how_many_days_in_cur_month( copy.date_Month.m_Month , copy.date_Year.m_Year ); 
        }
      }
    }
      if( !isDateValid( copy.date_Day , copy.date_Month , copy.date_Year ) )
      {  
        throw InvalidDateException();
        return *this;
      }
      // date_Year.m_Year = copy.date_Year.m_Year;
      // date_Month.m_Month = copy.date_Month.m_Month;
      // date_Day.m_Day = copy.date_Day.m_Day;
      CDate result( copy.date_Year.m_Year , copy.date_Month.m_Month , copy.date_Day.m_Day );
      return result;
      //return *this;
    }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
    CDate operator - (const Year value)const 
    {
      Year copy(date_Year.m_Year);
      copy.m_Year -= value.m_Year;
      if( !isDateValid( date_Day , date_Month , copy ) )
      { 
        throw InvalidDateException();
        return *this;
      }
      //date_Year.m_Year = copy.m_Year;
      CDate result( copy.m_Year , date_Month.m_Month , date_Day.m_Day );
      //return *this;
      return result;
    }

    CDate operator - (const  Month value)const 
    {
      Month copyMonth(value);
      Year copyYear(date_Year.m_Year);
      if( copyMonth.m_Month > 12 )
      {
        int years_to_remove = copyMonth.m_Month/12;
        copyYear.m_Year -= years_to_remove;
        copyMonth.m_Month = copyMonth.m_Month - (years_to_remove*12);
      }
      int remove = date_Month.m_Month;
      while( copyMonth.m_Month > 0 )
      {
        remove--;
        copyMonth.m_Month--;
        if( remove == 0 )
        {
          remove = 12;
          copyYear.m_Year--;
        }
      }
      while( copyMonth.m_Month < 0 )
      {
        remove++;
        copyMonth.m_Month++;
        if(remove == 13)
        {
          remove = 1;
          copyYear.m_Year++;
        } 
      }
      copyMonth.m_Month = remove;
      if( !isDateValid( date_Day , copyMonth , copyYear ) )
      { 
        throw InvalidDateException();
        return *this;
      }
      //date_Month.m_Month = copyMonth.m_Month;
      //date_Year.m_Year = copyYear.m_Year;
      CDate result( copyYear.m_Year , copyMonth.m_Month , date_Day.m_Day );
      return result;
      //return *this;
    }

    CDate operator - (const  Day value )const 
    {
      CDate copy( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
      if( value.m_Day > 0 )
      {
      long long int dny = value.m_Day;
//cout << "PUVODNE->rok: " << copy.date_Year.m_Year << " mesic: " << copy.date_Month.m_Month << " den: " << copy.date_Day.m_Day << " dny k odecteni: " << dny << endl;      
      while( copy.date_Day.m_Day > 1 && dny > 0 )
      {
        copy.date_Day.m_Day--;
        dny--;
      }
//cout << "rok: " << copy.date_Year.m_Year << " mesic: " << copy.date_Month.m_Month << " den: " << copy.date_Day.m_Day << " dny k odecteni: " << dny << endl;
      int kolik;
      while( copy.date_Month.m_Month > 1 && dny > 0)
      {
        if( copy.date_Month.m_Month >= 2 )
          {
            kolik = how_many_days_in_cur_month( copy.date_Month.m_Month-1 , copy.date_Year.m_Year );
            if( kolik <= dny )
              {
                dny -= kolik;
                copy.date_Month.m_Month--;
              }
            else
              break;
          }
        else if( copy.date_Month.m_Month == 1 )
          break;
      }
//cout << "radek 386 ->rok: " << copy.date_Year.m_Year << " mesic: " << copy.date_Month.m_Month << " den: " << copy.date_Day.m_Day << " dny k odecteni: " << dny << endl;
      while( dny >= 365 )
      {
        if( isLeapYear(copy.date_Year.m_Year-1) )
        {
          if( dny >= 366 )
          {
            copy.date_Year.m_Year--;
            dny -= 366;
          }
          else break;
        }
        else
        {
          copy.date_Year.m_Year--;
          dny -= 365;
        }
      }
//cout << "radek 404 ->rok: " << copy.date_Year.m_Year << " mesic: " << copy.date_Month.m_Month << " den: " << copy.date_Day.m_Day << " dny k odecteni: " << dny << endl;
      if(dny > 0)
      {
        if( copy.date_Month.m_Month == 1 )
        {
          copy.date_Year.m_Year--;
          copy.date_Month.m_Month = 12;        
          copy.date_Day.m_Day = 31;
          dny--;
        }
      }
//cout << "radek 414 ->rok: " << copy.date_Year.m_Year << " mesic: " << copy.date_Month.m_Month << " den: " << copy.date_Day.m_Day << " dny k odecteni: " << dny << endl;
      while( dny > 0 )
      {
        while( copy.date_Day.m_Day > 1 && dny > 0)
        {
          dny--;
          copy.date_Day.m_Day--;
        }
        if( copy.date_Day.m_Day == 1 )
        {
          copy.date_Month.m_Month--;
          copy.date_Day.m_Day = how_many_days_in_cur_month( copy.date_Month.m_Month , copy.date_Year.m_Year );
          dny--;
        }
      }
//cout << "radek 427 ->rok: " << copy.date_Year.m_Year << " mesic: " << copy.date_Month.m_Month << " den: " << copy.date_Day.m_Day << " dny k odecteni: " << dny << endl;
    }
    else
    {
      Day hodnota(value.m_Day);
      //hodnota.m_Day = value.m_Day;
      hodnota.m_Day = -hodnota.m_Day;
      long long int dny = hodnota.m_Day + copy.date_Day.m_Day - 1 ;
      //cout << "na zacatku dnu: " << dny << endl;

      while( !isLeapYear(copy.date_Year.m_Year) && dny >= 365 )
      {
        dny -= 365;;
        copy.date_Year.m_Year++;
      }
      while( copy.date_Year.m_Year%100 != 0 && dny >= DAYS_IN4 )
      {
        dny -= DAYS_IN4;
        copy.date_Year.m_Year += 4;
      }
      while( copy.date_Year.m_Year%400 != 0 && dny >= DAYS_IN100 )
      {
        dny -= DAYS_IN100;
        copy.date_Year.m_Year += 100;
      }
      while( copy.date_Year.m_Year%4000 != 0 && dny >= DAYS_IN400 )
      {
        dny -= DAYS_IN400;
        copy.date_Year.m_Year += 400;
      }


      while( dny >= DAYS_IN4k && copy.date_Year.m_Year%4000 == 0 )
      {
        copy.date_Year.m_Year += 4000;
        dny -= DAYS_IN4k;
        //cout << "4k"<< endl;
      } 
      while( copy.date_Year.m_Year%400 == 0 && dny >= DAYS_IN400 )
      {
       copy.date_Year.m_Year += 400;
       dny -= DAYS_IN400;
       //cout << "400" << endl;
      }
      while( copy.date_Year.m_Year%100 == 0 && dny >= DAYS_IN100 )
      {
        copy.date_Year.m_Year += 100;
        dny -= DAYS_IN100;
        //cout << "100" << endl;
      }
      while( copy.date_Year.m_Year%4 == 0 && dny >= DAYS_IN4 )
      {
        copy.date_Year.m_Year += 4;
        dny -= DAYS_IN4;
        //cout << "4" << endl;
      }
      while( dny >= 365 )
      {
        copy.date_Year.m_Year += 1;
        dny -= 365;
      }

      while( dny >= 0 )
      {
        int days_in_month = how_many_days_in_cur_month( copy.date_Month.m_Month , copy.date_Year.m_Year ); 
        if( dny >= days_in_month )
        {
          dny -= days_in_month;
          copy.date_Month.m_Month++;
          if( copy.date_Month.m_Month > 12 )
          {
            copy.date_Year.m_Year++;
            copy.date_Month.m_Month = 1;
          }
        }
        else
        {
          copy.date_Day.m_Day = dny + 1;
          dny = -1;
        }
      }
    }
      if( !isDateValid(copy.date_Day.m_Day , copy.date_Month.m_Month , copy.date_Year.m_Year ) )
      {
        throw InvalidDateException();
      }
      return copy;
  }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------  
    bool operator !=(const CDate &b) const
    {
      return( !(date_Year.m_Year == b.date_Year.m_Year && date_Month.m_Month == b.date_Month.m_Month && date_Day.m_Day == b.date_Day.m_Day) );
    }
    bool operator ==(const CDate &b) const
    {
      return( date_Year.m_Year == b.date_Year.m_Year && date_Month.m_Month == b.date_Month.m_Month && date_Day.m_Day == b.date_Day.m_Day );
    }
    bool operator <(const CDate &b) const
    {
      if( date_Year.m_Year < b.date_Year.m_Year )
        return true;
      else if( date_Year.m_Year == b.date_Year.m_Year )
      {
        if( date_Month.m_Month < b.date_Month.m_Month )
          return true;
        else if( date_Month.m_Month == b.date_Month.m_Month )
        {
          if( date_Day.m_Day < b.date_Day.m_Day )
            return true;
        }
      }
      return false;
    }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------  
    int operator -(const CDate x)const 
    {
      int sum = 0;
      bool minus;

      //cout << "odecitam: " << *this << " minus " << x << endl; 

      CDate right( x.date_Year.m_Year , x.date_Month.m_Month , x.date_Day.m_Day );
      CDate left( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
      CDate *vetsi = NULL;
      CDate *mensi = NULL;


      if( *this < x )
      {
        minus = true;
        // CDate vetsi( x.date_Year.m_Year , x.date_Month.m_Month , x.date_Day.m_Day );
        // CDate mensi( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
        vetsi = &right;
        mensi = &left;
      }
      else
      { 
        minus = false;
        vetsi = &left;
        mensi = &right;
        // CDate mensi( x.date_Year.m_Year , x.date_Month.m_Month , x.date_Day.m_Day );
        // CDate vetsi( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
      }

      while( mensi->date_Day.m_Day > 1 )
      {
        sum--;
        mensi->date_Day.m_Day--;
      }
      int kolik;
      while( mensi->date_Month.m_Month > 1 )
      {
        if( mensi->date_Month.m_Month >= 2 )
        {
          kolik = how_many_days_in_cur_month( mensi->date_Month.m_Month-1, mensi->date_Year.m_Year );
          sum -= kolik;
          mensi->date_Month.m_Month--;
        }
      }

      while( mensi->date_Year.m_Year != vetsi->date_Year.m_Year )
      {
        if( isLeapYear(mensi->date_Year.m_Year) )
        {
          sum += 366;
        }
        else
        {
          sum += 365;
        }
        mensi->date_Year.m_Year++;
      }

      while( mensi->date_Month.m_Month < vetsi->date_Month.m_Month )
      {
        kolik = how_many_days_in_cur_month( mensi->date_Month.m_Month, mensi->date_Year.m_Year );
        sum += kolik;
        mensi->date_Month.m_Month++;
      }

      while( mensi->date_Day.m_Day < vetsi->date_Day.m_Day )
      {
        sum += 1;
        mensi->date_Day.m_Day++;
      }

      if( minus )
      {
        sum = -sum;
      }

      return sum;
    }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------  
  CDate operator += (vector<zasobnik> pole)
  {
    if( !isDateValid( date_Day.m_Day , date_Month.m_Month , date_Year.m_Year ) )
    {
      throw InvalidDateException();
    }

    vector<zasobnik>::iterator ittArray = pole.begin();

    while( ittArray != pole.end() )
    {
      if( ittArray->den != 0 )
      {
        *this = *this + Day(ittArray->den); 
      }
      if( ittArray->mesic != 0 )
      {
        *this = *this + Month(ittArray->mesic);
      }
      if( ittArray->rok != 0 )
      {
        *this = *this + Year(ittArray->rok);
      }

      if( !isDateValid( date_Day.m_Day , date_Month.m_Month , date_Year.m_Year ) )
        throw InvalidDateException();

      ittArray++;
    }

    Array.clear();

    return *this;
  }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
  CDate operator += (const Year x)
  {
    CDate copy( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
    copy.date_Year.m_Year += x.m_Year;

    if( !isDateValid( date_Day , date_Month , copy.date_Year ) )
      throw InvalidDateException();

    this->date_Year.m_Year = copy.date_Year.m_Year;
    return *this;
  }

  CDate operator += (const Month x)
  {
    CDate copy( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
    Month copyMonth(x.m_Month);
 if(copyMonth.m_Month > 0)
 { 
    while( copyMonth.m_Month > 0 )
    {
      if( copy.date_Month.m_Month == 12 )
      {
        copy.date_Year.m_Year++;
        copy.date_Month.m_Month = 1;
        copyMonth.m_Month--;
      }
      else
      {
        copyMonth.m_Month--;
        copy.date_Month.m_Month++;
      }
    }
  }
  else
  {
    copyMonth.m_Month = -( copyMonth.m_Month );
    while( copyMonth.m_Month > 0 )
    {
      if( copy.date_Month.m_Month == 1 )
      {
        copy.date_Year.m_Year--;
        copy.date_Month.m_Month = 12;
        copyMonth.m_Month--;
      }
      else
      {
        copyMonth.m_Month--;
        copy.date_Month.m_Month--;
      }
    }
  }


    if( !isDateValid(copy.date_Day , copy.date_Month , date_Year ) )
      throw InvalidDateException();

    this->date_Month.m_Month = copy.date_Month.m_Month;
    this->date_Year.m_Year = copy.date_Year.m_Year;
    return *this;
  }

  CDate operator += (const Day x)
  {
    CDate copy( date_Year.m_Year , date_Month.m_Month , date_Day.m_Day );
    Day copyDay = x.m_Day;
    if( x.m_Day > 0 )
    {
      copy = copy + copyDay;
    }
    else
    {
      copy = copy - copyDay;
    }

    if( !isDateValid( copy.date_Day , copy.date_Month , copy.date_Year ) )
      throw InvalidDateException();

    this->date_Month.m_Month = copy.date_Month.m_Month;
    this->date_Year.m_Year = copy.date_Year.m_Year;
    this->date_Day.m_Day = copy.date_Day.m_Day;
    return *this;
  }



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

    friend ostream & operator << ( ostream &vystup, const CDate &x );
private:
    Day date_Day;
    Month date_Month;
    Year date_Year;
};
//-------------------------------------------------------------------------------------------------
//nasypavani do vektoru
  vector<zasobnik> Month:: operator +(const  Year x )const 
  {
    zasobnik novy;
    novy.rok = x.m_Year;
    novy.den = 0;
    novy.mesic = this->m_Month;
    Array.push_back(novy);
    return Array;
  }

  vector<zasobnik> Day:: operator +( const Year x )const 
  {
    zasobnik novy;
    novy.rok = x.m_Year;
    novy.den = this->m_Day;
    novy.mesic = 0;
    Array.push_back(novy);
    return Array;
  }

  vector<zasobnik> Day:: operator +(const  Month x )const 
  {
    zasobnik novy;
    novy.rok = 0;
    novy.den = this->m_Day;
    novy.mesic = x.m_Month;
    Array.push_back(novy);
    return Array;
  }

  vector<zasobnik> Month:: operator -(const  Year x )const 
  {
    zasobnik novy;
    novy.rok = -(x.m_Year);
    novy.den = 0;
    novy.mesic = this->m_Month;
    Array.push_back(novy);
    return Array;
  }

  vector<zasobnik> Day:: operator -(const  Year x )const 
  {
    zasobnik novy;
    novy.rok = -(x.m_Year);
    novy.den = this->m_Day;
    novy.mesic = 0;
    Array.push_back(novy);
    return Array;
  }

  vector<zasobnik> Day:: operator -(const  Month x )const 
  {
    zasobnik novy;
    novy.rok = 0;
    novy.den = this->m_Day;
    novy.mesic = -(x.m_Month);
    Array.push_back(novy);
    return Array;
  }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

ostream & operator << ( ostream &vystup, const CDate &x )
{
	vystup << x.date_Year.m_Year << "-";
	if( x.date_Month.m_Month < 10 )
		vystup << "0" << x.date_Month.m_Month << "-";
	else
		vystup << x.date_Month.m_Month << "-";

	if( x.date_Day.m_Day < 10 )
		vystup << "0" << x.date_Day.m_Day;
	else
		vystup << x.date_Day.m_Day;

	return vystup;
}

vector<zasobnik> operator +( vector<zasobnik> pole ,const  Month x )
{
  zasobnik novy;
  novy.rok = 0;
  novy.den = 0;
  novy.mesic = x.m_Month;
  pole.push_back(novy);
  return pole;
}


vector<zasobnik> operator +( vector<zasobnik> pole ,const  Day x ) 
{
  zasobnik novy;
  novy.rok = 0;
  novy.den = x.m_Day;
  novy.mesic = 0;
  pole.push_back(novy);
  return pole;
}


vector<zasobnik> operator +( vector<zasobnik> pole ,const  Year x )
{
  zasobnik novy;
  novy.rok = x.m_Year;
  novy.den = 0;
  novy.mesic = 0;
  pole.push_back(novy);
  return pole;
}

vector<zasobnik> operator -( vector<zasobnik> pole ,const  Month x ) 
{
  zasobnik novy;
  novy.rok = 0;
  novy.den = 0;
  novy.mesic = -(x.m_Month);
  pole.push_back(novy);
  return pole;
}


vector<zasobnik> operator -( vector<zasobnik> pole ,const  Day x )
{
  zasobnik novy;
  novy.rok = 0;
  novy.den = -(x.m_Day);
  novy.mesic = 0;
  pole.push_back(novy);
  return pole;
}


vector<zasobnik> operator -( vector<zasobnik> pole ,const  Year x ) 
{
  zasobnik novy;
  novy.rok = -(x.m_Year);
  novy.den = 0;
  novy.mesic = 0;
  pole.push_back(novy);
  return pole;
} 



//-------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
string             toString                                ( const CDate     & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int                main                                    ( void )
{
  CDate tmp ( 2000, 1, 1 );

  assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
  assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
  assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
  //-----------------------------------------------------
	try//MOOOJEEEE!!!
	{
		tmp = CDate ( 2000, 4 , 31 );
		assert ( "Missing exception" == NULL );
	}
	catch ( const InvalidDateException & e )
	{
	}
	try
	{
		tmp = CDate ( 2000, 1, 31 ) + Month ( 3 );
    	assert ( "Missing exception" == NULL );
	}
	catch ( const InvalidDateException & e )
	{
	}
  tmp = CDate ( 2018, 3, 15 );
  assert ( toString ( tmp + Month ( 9 ) ) == "2018-12-15" );
  assert ( toString ( tmp ) == "2018-03-15" );
	assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 9 ) ) == "2018-12-15" );
	assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 10 ) ) == "2019-01-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) - Year ( 20 ) ) == "1998-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) - Month ( 2 ) ) == "2018-01-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) - Month ( 3 ) ) == "2017-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) - Month ( 16 ) ) == "2016-11-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) - Year ( -16 ) ) == "2034-03-15" );
  assert ( toString ( CDate ( 2000, 6, 15 ) - Month ( -24 ) ) == "2002-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( -2 ) ) == "2016-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( -3 ) ) == "2017-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) - Day ( 15 ) ) == "2018-02-28" );
  assert ( toString ( CDate ( 5398, 5, 2 ) - Day ( 1234567 ) )  == "2018-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) - Day ( -1234567 ) )  == "5398-05-02" );
  assert ( toString ( CDate ( 5398, 5, 2 ) + Day ( -1234567 ) )  == "2018-03-15" );  
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 3 ) + Month ( -18 ) ) == "2019-09-15");
  //-----------------------------------------------------
   try
   {
     tmp = CDate ( 1900, 2, 29 );
     assert ( "Missing exception" == NULL );
   }
   catch ( const InvalidDateException & e )
   {
   }
   assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
   assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
   try
   {
     tmp = CDate ( 4000, 2, 29 );
     assert ( "Missing exception" == NULL );
   }
   catch ( const InvalidDateException & e )
   {
   }  
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 1 ) )  == "2019-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 7 ) )  == "2025-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Year ( 73 ) )  == "2091-03-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 3 ) )  == "2018-06-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 1 ) )  == "2018-04-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Month ( 285 ) )  == "2041-12-15" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1 ) )  == "2018-03-16" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 12 ) )  == "2018-03-27" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 1234567 ) )  == "5398-05-02" );
  assert ( toString ( CDate ( 2018, 3, 15 ) + Day ( 3 ) + Year ( 2 ) + Month ( 3 ) + Day ( 5 ) + Month ( 11 ) )  == "2021-05-23" );
	try
	{
		tmp = CDate ( 2000, 2, 29 ) + Year ( 300 );
	    assert ( "Missing exception" == NULL );
	}
	catch ( const InvalidDateException & e )
	{
	}
	try
	{
		tmp = CDate ( 2000, 3, 30 ) + Month ( 11 );
	    assert ( "Missing exception" == NULL );
	}
	catch ( const InvalidDateException & e )
	{
	}
  assert ( toString ( CDate ( 2001, 2, 20 ) + Day ( 9 ) )  == "2001-03-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 1999, 1, 1 ) + Year ( 1 ) + Day ( 28 ) + Month ( 1 ) )  == "2000-02-29" );
  assert ( toString ( CDate ( 2001, 1, 1 ) + Day ( 1095 ) + Day ( 28 ) + Month ( 1 ) )  == "2004-02-29" );
  try
  {
    tmp = CDate ( 2001, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Day ( 1095 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + Day ( 28 ) + Month ( 1 ) + Year ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  assert ( toString ( tmp - Year ( 5 ) + Month ( 2 ) )  == "1995-03-01" );
  assert ( toString ( tmp ) == "2000-01-01" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 1 ) - Month ( 3 ) - Day ( 10 ) )  == "1998-09-21" );
  assert ( toString ( CDate ( 2000, 1, 1 ) - Year ( 2 ) - Month ( -3 ) + Day ( -10 ) )  == "1998-03-22" );
  assert ( toString ( CDate ( 2000, 1, 1 ) + Day ( 59 ) - Month ( 1 ) - Year ( 2 ) )  == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 ) + Day ( 59 ) - Year ( 2 ) - Month ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
  tmp = CDate ( 2000, 1, 1 );
  tmp +=  - Year ( 2 ) - Month ( -3 ) + Day ( -10 );
  assert ( toString ( tmp ) == "1998-03-22" );
  tmp = CDate ( 2000, 1, 1 );
  tmp += Day ( 59 ) - Month ( 1 ) - Year ( 2 );
  assert ( toString ( tmp ) == "1998-01-29" );
  try
  {
    tmp = CDate ( 2000, 1, 1 );
    tmp += Day ( 59 ) - Year ( 2 ) - Month ( 1 );
    assert ( "Missing exception" == NULL );
  }
  catch ( const InvalidDateException & e )
  {
  }
tmp = CDate ( 2018, 3, 15 ) + Day ( -3 );
assert ( toString ( tmp ) == "2018-03-12" );
assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) == CDate ( 2018, 3, 15 ) ) );
assert ( CDate ( 2018, 3, 15 ) + Day ( 1 ) != CDate ( 2018, 3, 15 ) );
assert ( !( CDate ( 2018, 3, 15 ) + Day ( 1 ) < CDate ( 2018, 3, 15 ) ) );
assert ( !( CDate ( 2018, 3, 15 ) + Day ( -1 ) == CDate ( 2018, 3, 15 ) ) );
assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) != CDate ( 2018, 3, 15 ) );
assert ( CDate ( 2018, 3, 15 ) + Day ( -1 ) < CDate ( 2018, 3, 15 ) );
assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
assert ( CDate ( 2018, 3, 15 ) + Year ( 3 ) + Month ( -18 ) - CDate ( 2000, 1, 1 ) == 7197 );
assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//#ifdef TEST_LITERALS
   assert ( toString ( CDate ( 2000, 1, 1 ) )  == "2000-01-01" );
   assert ( toString ( CDate ( 2500, 12, 21 ) )  == "2500-12-21" );
   assert ( toString ( CDate ( 1685, 7, 11 ) )  == "1685-07-11" );
   try
   {
     tmp = CDate ( 1900, 2, 29 );
     assert ( "Missing exception" == NULL );
   }
   catch ( const InvalidDateException & e )
   {
   }
   assert ( toString ( CDate ( 2000, 2, 29 ) )  == "2000-02-29" );
   assert ( toString ( CDate ( 2004, 2, 29 ) )  == "2004-02-29" );
   try
   {
     tmp = CDate ( 4000, 2, 29 );
     assert ( "Missing exception" == NULL );
   }
   catch ( const InvalidDateException & e )
   {
   }
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 1_year )  == "2019-03-15" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 7_years )  == "2025-03-15" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 73_years )  == "2091-03-15" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 3_months )  == "2018-06-15" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 1_month )  == "2018-04-15" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 285_months )  == "2041-12-15" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 1_day )  == "2018-03-16" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 12_days )  == "2018-03-27" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 1234567_days )  == "5398-05-02" );
//   assert ( toString ( CDate ( 2018, 3, 15 ) + 3_days + 2_years + 3_months + 5_days + 11_months )  == "2021-05-23" );
//   try
//   {
//     tmp = CDate ( 2000, 2, 29 ) + 300_years;
//     assert ( "Missing exception" == NULL );
//   }
//   catch ( const InvalidDateException & e )
//   {
//   }
//   try
//   {
//     tmp = CDate ( 2000, 3, 30 ) + 11_months;
//     assert ( "Missing exception" == NULL );
//   }
//   catch ( const InvalidDateException & e )
//   {
//   }
//   assert ( toString ( CDate ( 2001, 2, 20 ) + 9_days )  == "2001-03-01" );
//   assert ( toString ( CDate ( 2000, 1, 1 ) + 28_days + 1_month )  == "2000-02-29" );
//   assert ( toString ( CDate ( 1999, 1, 1 ) + 1_year + 28_days + 1_month )  == "2000-02-29" );
//   assert ( toString ( CDate ( 2001, 1, 1 ) + 1095_days + 28_days + 1_month )  == "2004-02-29" );
//   try
//   {
//     tmp = CDate ( 2001, 1, 1 ) + 28_days + 1_month + 1095_days;
//     assert ( "Missing exception" == NULL );
//   }
//   catch ( const InvalidDateException & e )
//   {
//   }
//   try
//   {
//     tmp = CDate ( 2000, 1, 1 ) + 28_days + 1_month + 1_year;
//     assert ( "Missing exception" == NULL );
//   }
//   catch ( const InvalidDateException & e )
//   {
//   }
//   tmp = CDate ( 2000, 1, 1 );
//   assert ( toString ( tmp - 5_years + 2_months )  == "1995-03-01" );
//   assert ( toString ( tmp ) == "2000-01-01" );
//   assert ( toString ( CDate ( 2000, 1, 1 ) - 1_year - 3_months - 10_days )  == "1998-09-21" );
//   assert ( toString ( CDate ( 2000, 1, 1 ) - 2_years - ( -3_months ) + ( -10_days ) )  == "1998-03-22" );
//   assert ( toString ( CDate ( 2000, 1, 1 ) + 59_days - 1_month - 2_years )  == "1998-01-29" );
//   try
//   {
//     tmp = CDate ( 2000, 1, 1 ) + 59_days - 2_years - 1_month;
//     assert ( "Missing exception" == NULL );
//   }
//   catch ( const InvalidDateException & e )
//   {
//   }
//   tmp = CDate ( 2000, 1, 1 );
//   tmp +=  - 2_years - ( -3_months ) + ( -10_days );
//   assert ( toString ( tmp ) == "1998-03-22" );
//   tmp = CDate ( 2000, 1, 1 );
//   tmp += 59_days - 1_month - 2_years;
//   assert ( toString ( tmp ) == "1998-01-29" );
//   try
//   {
//     tmp = CDate ( 2000, 1, 1 );
//     tmp += 59_days - 2_years - 1_month;
//     assert ( "Missing exception" == NULL );
//   }
//   catch ( const InvalidDateException & e )
//   {
//   }
//   tmp = CDate ( 2018, 3, 15 ) + ( -3_days );
//   assert ( toString ( tmp ) == "2018-03-12" );
//   assert ( !( CDate ( 2018, 3, 15 ) == CDate ( 2000, 1, 1 ) ) );
//   assert ( CDate ( 2018, 3, 15 ) != CDate ( 2000, 1, 1 ) );
//   assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2000, 1, 1 ) ) );
//   assert ( CDate ( 2018, 3, 15 ) == CDate ( 2018, 3, 15 ) );
//   assert ( !( CDate ( 2018, 3, 15 ) != CDate ( 2018, 3, 15 ) ) );
//   assert ( !( CDate ( 2018, 3, 15 ) < CDate ( 2018, 3, 15 ) ) );
//   assert ( !( CDate ( 2018, 3, 15 ) + 1_day == CDate ( 2018, 3, 15 ) ) );
//   assert ( CDate ( 2018, 3, 15 ) + 1_day != CDate ( 2018, 3, 15 ) );
//   assert ( !( CDate ( 2018, 3, 15 ) + 1_day < CDate ( 2018, 3, 15 ) ) );
//   assert ( !( CDate ( 2018, 3, 15 ) + ( -1_day ) == CDate ( 2018, 3, 15 ) ) );
//   assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) != CDate ( 2018, 3, 15 ) );
//   assert ( CDate ( 2018, 3, 15 ) + ( -1_day ) < CDate ( 2018, 3, 15 ) );
//   assert ( CDate ( 2018, 3, 15 ) - CDate ( 2000, 1, 1 ) == 6648 );
//   assert ( CDate ( 2000, 1, 1 ) - CDate ( 2018, 3, 15 ) == -6648 );
//   assert ( CDate ( 2018, 3, 15 ) + 3_years + ( -18_months ) - CDate ( 2000, 1, 1 ) == 7197 );
//   assert ( CDate ( 5398, 5, 2 ) - CDate ( 2018, 3, 15 ) == 1234567 );
// ostringstream oss;
// oss << setfill ( 'x' ) << left << hex << CDate ( 2000, 1, 1 ) << ' ' << setw ( 10 ) << 65536;
// assert ( oss . str () == "2000-01-01 10000xxxxx" );
//#endif /* TEST_LITERALS */
  return 0;
}
#endif /* __PROGTEST__ */
