#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

// enable only if your implementation supports Add ( ) with more than three parameters
#define MULTIPLE_STOPS
//================================================================
//================================================================
// t jsou mesta e jsou spoje
template <typename _T, typename _E>
class CAccess{
public:
	CAccess<_T,_E> & Add( const _E & route , const _T & city1 ,const _T & city2 );	
	template <typename ...Args>
	CAccess<_T,_E> & Add( const _E & route , const _T & city1 , const _T & city2 , const _T & city3 , const Args & ... otherCities );

	map<_T,int> Find( const _T & treasure , const int max = -1 , function<bool (const _E &)> filter = NULL )const;

private:
	map< _T , vector< pair<_E , _T> >  > Data;

	void recursion( deque< pair<_T , int> > & Que , set<_T> & Set , map<_T,int> & result , unsigned int position , const int max = -1 , function<bool (const _E &)> filter = NULL )const;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------
//definice CAccess
template <typename _T, typename _E>
CAccess<_T,_E> & CAccess<_T,_E>::Add( const _E & route , const _T & city1 ,const _T & city2 )
{
	Data[city1].push_back(make_pair( route , city2 ) );
	Data[city2].push_back(make_pair( route , city1 ) );
	return *this;
}

template <typename _T, typename _E>
template <typename ...Args>
CAccess<_T,_E> & CAccess<_T,_E>::Add( const _E & route , const _T & city1 , const _T & city2 , const _T & city3 , const Args & ... otherCities )
{
	vector<_T> vect{city1,city2,city3,otherCities...};
	auto it = vect.begin();
	for( ; it != vect.end() ; it++)
	{
		if( (it+1) != vect.end() )
		{
			auto it_temp = it+1;

			for( ; it_temp != vect.end() ; it_temp++ )
			{
				Add(route,*it,*it_temp);
			}
		}
	}
	return *this;
}

template <typename _T, typename _E>
void CAccess<_T,_E>::recursion( deque< pair<_T , int> > & Que , set<_T> & Set , map<_T,int> & result , unsigned int position , const int max, function<bool (const _E &)> filter )const
{
	auto Que_it = Que.begin();
	Que_it = Que_it + position;
	int distance = Que_it->second + 1 ;
	if( max != -1 && distance > max )
	{
		position++;
		if( position !=  Que.size() )
			recursion( Que , Set , result , position , max , filter);
		return;
	}

	auto place_to_discover = Data.find( Que_it->first );
	for( auto vec_it = place_to_discover->second.begin() ; vec_it != place_to_discover->second.end() ; vec_it++ )
	{
		if( filter != NULL && !filter(vec_it->first) )
			continue;

		int size_before = Set.size();
		Set.insert( vec_it->second );
		int size_after = Set.size();
		if( size_before == size_after )
			continue;	

		Que.push_back( make_pair( vec_it->second , distance ) );
		result.insert( make_pair(vec_it->second , distance) );
	}

	position++;
	if( position ==  Que.size() )
		return;
		
	recursion( Que , Set , result , position , max , filter);
}

template <typename _T, typename _E>
map<_T,int> CAccess<_T,_E>::Find( const _T & treasure , const int max , function<bool (const _E &)> filter )const
{
	if( Data.find(treasure) == Data.end() )
	{
		string error_message = "unknown ";
		
		stringstream buffer;
		streambuf * old = std::cout.rdbuf(buffer.rdbuf());
		cout << treasure;
		error_message.append( buffer.str() );
		cout.rdbuf( old );
		
		throw invalid_argument(error_message);
	}


	map<_T,int> result;
	deque< pair<_T, int> > Que;
	set<_T> Set;
	
	Set.insert(treasure);
	Que.push_back( make_pair(treasure,0) );
	result[treasure] = 0;
	
	recursion( Que , Set , result , 0 , max , filter );

	return result;
}

#ifndef __PROGTEST__
//================================================================
//================================================================
class CTrain{
public:
	CTrain( const string & company , int speed ) : m_Company(company) , m_Speed(speed) {}

	string m_Company;
	int m_Speed;
};
//================================================================
class TrainFilterCompany{
public:
	TrainFilterCompany( const set<string> & companies ) : m_Companies(companies) {}

	bool operator () ( const CTrain & train )const
	{
		return m_Companies.find(train.m_Company) != m_Companies.end();
	}
private:
	set <string> m_Companies;
};
//================================================================
class TrainFilterSpeed
{
public:
	TrainFilterSpeed( int min , int max ) : m_Min(min) , m_Max(max) {}

	bool operator () ( const CTrain & train )const
	{
		return train.m_Speed >= m_Min && train.m_Speed <= m_Max;
	}
private:
	int m_Min;
	int m_Max;
};
//================================================================
bool NurSchnellzug( const CTrain & zug )
{
	return (zug.m_Company == "OBB" || zug.m_Company == "DB" ) && zug.m_Speed > 100;
}
//================================================================
//================================================================
//================================================================
int main()
{
  CAccess<string,CTrain> lines;
  lines . Add ( CTrain (   "DB", 120 ), "Berlin", "Prague"   )
        . Add ( CTrain (   "CD",  80 ), "Berlin", "Prague"   )
        . Add ( CTrain (   "DB", 160 ), "Berlin", "Dresden"  )
        . Add ( CTrain (   "DB", 160 ), "Dresden", "Munchen" )
        . Add ( CTrain (   "CD",  90 ), "Munchen", "Prague"  )
        . Add ( CTrain (   "DB", 200 ), "Munchen", "Linz"    )
        . Add ( CTrain (  "OBB",  90 ), "Munchen", "Linz"    )
        . Add ( CTrain (   "CD",  50 ), "Linz", "Prague"     )
        . Add ( CTrain (   "CD", 100 ), "Prague", "Wien"     )
        . Add ( CTrain (  "OBB", 160 ), "Linz", "Wien"       )
        . Add ( CTrain ( "SNCF", 300 ), "Paris", "Marseille" )
        . Add ( CTrain ( "SNCF", 250 ), "Paris", "Dresden"   )
        . Add ( CTrain ( "SNCF", 200 ), "London", "Calais"   );

  assert ( lines . Find ( "Berlin" ) == (map<string,int>
  {
    make_pair ( "Berlin", 0 ),
    make_pair ( "Dresden", 1 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Marseille", 3 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "London" ) == (map<string,int>
  {
    make_pair ( "Calais", 1 ),
    make_pair ( "London", 0 )
  }) );
  assert ( lines . Find ( "Wien" ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Marseille", 5 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Paris", 4 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 0 )
  }) );
  assert ( lines . Find ( "Wien", 3 ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 0 )
  }) );

  assert ( lines . Find ( "Wien", 5, NurSchnellzug ) == (map<string,int>
  {
    make_pair ( "Berlin", 4 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Prague", 5 ),
    make_pair ( "Wien", 0 )
  }) );
  assert ( lines . Find ( "Prague", 3,  TrainFilterCompany ( { "CD", "DB" } ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 1 ),
    make_pair ( "Dresden", 2 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 1 ),
    make_pair ( "Prague", 0 ),
    make_pair ( "Wien", 1 )
  }) );
  assert ( lines . Find ( "Munchen", 4, TrainFilterSpeed ( 160, 250 ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 1 ),
    make_pair ( "Linz", 1 ),
    make_pair ( "Munchen", 0 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "Munchen", 4, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Munchen", 0 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "London", 20, [] ( const CTrain & x ) { return x . m_Company == "CD"; } ) == (map<string,int>
  {
    make_pair ( "London", 0 )
  }) );
  try
  {
     auto res = lines . Find ( "Salzburg" );
     assert ( "No exception thrown" == NULL );
  }
  catch ( const invalid_argument & e )
  {
    assert ( string ( e . what () ) == "unknown Salzburg" );
  }
 #ifdef MULTIPLE_STOPS
   lines . Add ( CTrain ( "RZD",  80 ), "Prague", "Kiev", "Moscow", "Omsk", "Irkutsk", "Vladivostok" );
   lines . Add ( CTrain ( "LAV", 160 ), "Malaga", "Cordoba", "Toledo", "Madrid", "Zaragoza", "Tarragona", "Barcelona", "Montpellier", "Marseille" );
   assert ( lines . Find ( "Madrid" ) == (map<string,int>
   {
    make_pair ( "Barcelona", 1 ),
    make_pair ( "Berlin", 4 ),
    make_pair ( "Cordoba", 1 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 6 ),
    make_pair ( "Kiev", 6 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Madrid", 0 ),
    make_pair ( "Malaga", 1 ),
    make_pair ( "Marseille", 1 ),
    make_pair ( "Montpellier", 1 ),
    make_pair ( "Moscow", 6 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 6 ),
    make_pair ( "Paris", 2 ),
    make_pair ( "Prague", 5 ),
    make_pair ( "Tarragona", 1 ),
    make_pair ( "Toledo", 1 ),
    make_pair ( "Vladivostok", 6 ),
    make_pair ( "Wien", 6 ),
    make_pair ( "Zaragoza", 1 )
  }) );
  assert ( lines . Find ( "Omsk", 4 ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 2 ),
    make_pair ( "Moscow", 1 ),
    make_pair ( "Munchen", 2 ),
    make_pair ( "Omsk", 0 ),
    make_pair ( "Paris", 4 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 ),
    make_pair ( "Wien", 2 )
  }) );
  assert ( lines . Find ( "Moscow", 10, TrainFilterCompany ( { "RZD", "DB" } ) ) == (map<string,int>
  {
    make_pair ( "Berlin", 2 ),
    make_pair ( "Dresden", 3 ),
    make_pair ( "Irkutsk", 1 ),
    make_pair ( "Kiev", 1 ),
    make_pair ( "Linz", 5 ),
    make_pair ( "Moscow", 0 ),
    make_pair ( "Munchen", 4 ),
    make_pair ( "Omsk", 1 ),
    make_pair ( "Prague", 1 ),
    make_pair ( "Vladivostok", 1 )
  }) );
#endif /* MULTIPLE_STOPS */
  return 0;
}
#endif  /* __PROGTEST__ */