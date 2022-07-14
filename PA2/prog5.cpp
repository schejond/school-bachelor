#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
using namespace std;

class CDate 
{
public:
	CDate ( int y, int m, int d )
	: m_Y ( y ) , m_M ( m ) , m_D ( d ) {}

	int CompareTo ( const CDate & other ) const
	{
		if ( m_Y != other . m_Y )
			return ( other . m_Y < m_Y ) - ( m_Y < other . m_Y );
		if ( m_M != other . m_M )
			return ( other . m_M < m_M ) - ( m_M < other . m_M );
		return ( other . m_D < m_D ) - ( m_D < other . m_D );  
	}

	friend ostream & operator << ( ostream & os, const CDate & d )
	{
		return os << d . m_Y << '-' << d . m_M << '-' << d . m_D;
	}

private:
	int m_Y;
	int m_M;
	int m_D;
};
enum class ESortKey
{
	NAME,
	BIRTH_DATE,
	ENROLL_YEAR
};
#endif /* __PROGTEST__ */
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
#include <sstream>

string transform_name(const string & original)
{
	string copy = original;
	//cout << "pred transformaci: " << copy << endl;
	transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
	
	vector<string> Data;

	string bit;
	istringstream iss(copy);

	while( iss >> bit )
		Data.push_back(bit);

	sort( Data.begin() , Data.end() );

	vector<string>::const_iterator it = Data.begin();
	copy.clear();

	while( it != Data.end() )
	{
		copy.append(*it);
		copy.append(1,32);
		it++;
	}

	//cout << "po transformaci: " << copy << endl;
	return copy;
}

set<string> lower_and_put_to_set(const string & original)
{
	string copy = original;
	transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
	
	set<string> Data;

	string bit;
	istringstream iss(copy);

	while( iss >> bit )
		Data.insert(bit);

	return Data;
}

static uint64_t counter_lifeSpawn = 0;

class CStudent
{
public:
	CStudent ( const string & name, const CDate & born, int enrolled );

	bool operator == ( const CStudent  & other ) const;
	bool operator != ( const CStudent  & other ) const;

	struct HashIt
	{
		size_t operator()(const CStudent & k)const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			stringstream buffer;
			streambuf * old = std::cout.rdbuf(buffer.rdbuf());
			cout << k.date;

			string text = buffer.str();

			cout.rdbuf( old );

			return ( (hash<string>()(k.fullName) ^ (hash<string>()(text) << 1)) >> 1 )
			^ (hash<int>()(k.enroll) << 1);
		}
	};

	friend class CStudyDept;
	friend class CFilter;
	friend class CSort;

private:
	string fullName;
	CDate date;
	int enroll;
	uint64_t id;
};
//----------------------------------------------------------------------------------------------
//CStudent definice
CStudent::CStudent( const string & name, const CDate & born, int enrolled )
	: fullName(name) , date(born) , enroll(enrolled) , id(counter_lifeSpawn++) {}

bool CStudent::operator == ( const CStudent & other )const
{
	return !date.CompareTo(other.date) && enroll == other.enroll && fullName == other.fullName;
}

bool CStudent::operator != ( const CStudent  & other ) const
{
	return !(*this == other);
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
class CFilter
{
public:
	CFilter () {}
	CFilter & Name ( const string & name ) { Data_filter.insert( transform_name(name) ); return *this;}//prinana rovnou transformavane jmeno
	CFilter & BornBefore ( const CDate & date ) { CDate_MAX = date; return *this; }
	CFilter & BornAfter ( const CDate & date ) { CDate_MIN = date; return *this; }
	CFilter & EnrolledBefore ( int year ) { Enroll_MAX = year; return *this; }
	CFilter & EnrolledAfter ( int year ) { Enroll_MIN = year; return *this; }

	bool operator () ( const CStudent &src )const;
private:
	int64_t Enroll_MIN = numeric_limits<int>::min();
	int64_t Enroll_MAX = numeric_limits<int>::max();
	CDate CDate_MIN = CDate( numeric_limits<int>::min() , 0 , 0 );
	CDate CDate_MAX = CDate( numeric_limits<int>::max() , 12 , 31 );
	set<string> Data_filter;

	bool compare_names( const string & src )const;
};
//----------------------------------------------------------------------------------------------
bool CFilter::operator () ( const CStudent &src )const
{
	const bool name_valid = compare_names(src.fullName);
	return ( ( Enroll_MIN < src.enroll && Enroll_MAX > src.enroll )
			&& ( CDate_MAX.CompareTo(src.date) == 1 && CDate_MIN.CompareTo(src.date) == -1  )
			&& name_valid );
}

bool CFilter::compare_names( const string & src )const
{
	if( Data_filter.size() == 0 )
		return true;
	string transformed_src = transform_name(src);
	for( auto i : Data_filter )
	{
		if( transformed_src == i )
			return true;
	}
	return false;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
class CSort
{
public:
	CSort & AddKey( ESortKey key, bool ascending );
	bool operator () ( const CStudent & left , const CStudent & right )const;

private:
	vector<pair<ESortKey,bool> > Sort_criteria;
};
//----------------------------------------------------------------------------------------------
//CSort definice
CSort & CSort::AddKey( ESortKey key, bool ascending )
{
	Sort_criteria.emplace_back(key,ascending);
	return *this;
}

bool CSort::operator () ( const CStudent & left , const CStudent & right )const
{
	vector<pair<ESortKey,bool>>::const_iterator it = Sort_criteria.begin();

	for(unsigned int i = 0; i < Sort_criteria.size(); i++)
	{
		switch( (it+i)->first )
		{
			case ESortKey::ENROLL_YEAR :
			{
				if( left.enroll < right.enroll )
					return (it+i)->second;
				else if( left.enroll > right.enroll )
					return !( (it+i)->second );
				break;
			}
			case ESortKey::BIRTH_DATE :
			{
				if( (left.date).CompareTo(right.date) < 0 )
					return (it+i)->second;
				else if( (left.date).CompareTo(right.date) > 0 )
					return !( (it+i)->second );
				break;
			}
			case ESortKey::NAME :
			{
				int res = (left.fullName).compare(right.fullName);
				if( res < 0  )
					return (it+i)->second;
				else if( res > 0 )
					return !( (it+i)->second );
				break;
			}
		}
	}

	return left.id < right.id;
}
//----------------------------------------------------------------------------------------------
multiset<string> cut_string(const string & src )
{
	multiset<string> Data;

	string copy = src;
	transform(copy.begin(), copy.end(), copy.begin(), ::tolower);

	string bit;
	istringstream iss(copy);

	while( iss >> bit )
		Data.insert(bit);

	return Data;
}
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
class CStudyDept 
{
public:
	CStudyDept ();
	bool AddStudent ( const CStudent  & x );
	bool DelStudent ( const CStudent  & x );
	list<CStudent> Search ( const CFilter & flt, const CSort & sortOpt ) const;
	set<string> Suggest ( const string & name ) const;

private:
	unordered_set<CStudent , CStudent::HashIt > Students;
};
//----------------------------------------------------------------------------------------------
//CStudyDept definice
CStudyDept::CStudyDept() {}

bool CStudyDept::AddStudent( const CStudent & x )
{
	return Students.insert(x).second;
}

bool CStudyDept::DelStudent( const CStudent & x )
{
	return Students.erase(x);
}

list<CStudent> CStudyDept::Search ( const CFilter & flt, const CSort & sortOpt )const
{
	vector<CStudent> Found_Students;
	copy_if( Students.begin() , Students.end() , back_inserter(Found_Students) , flt );
	sort( Found_Students.begin() , Found_Students.end() , sortOpt );

	return list<CStudent>( Found_Students.begin() , Found_Students.end() );
}

set<string> CStudyDept::Suggest ( const string & name ) const
{
	set<string> cut_name = lower_and_put_to_set(name);
	
	multiset<string> Multi_student;
	set<string> results;

	for( auto i : Students)
	{
		Multi_student = cut_string(i.fullName);

		if( includes( Multi_student.begin() , Multi_student.end() , cut_name.begin() , cut_name.end() ) )
			results.insert(i.fullName);
	}
	return results;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
int main ( void )
{
  CStudyDept x0;
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) ) );
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2010 ) ) );
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2010 ) ) );
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1980, 4, 11), 2016 ) ) );
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1980, 4, 11), 2016 ) ) );
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2010 ) ) );
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "James Bond", CDate ( 1997, 6, 17), 2016 ) ) );
	assert ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) != CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) );
	assert ( ! ( CStudent ( "James Bond", CDate ( 1980, 4, 11), 2010 ) == CStudent ( "Peter Peterson", CDate ( 1997, 6, 17), 2016 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ) ) );
	assert ( x0 . AddStudent ( CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ) ) );
	
  assert ( x0 . Search ( CFilter (), CSort () ) == (list<CStudent>
  {
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
  }) );
  assert ( x0 . Search ( CFilter (), CSort () . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
  {
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
  }) );
  assert ( x0 . Search ( CFilter (), CSort () . AddKey ( ESortKey::NAME, false ) ) == (list<CStudent>
  {
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 ),
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 )
  }) );
  assert ( x0 . Search ( CFilter (), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
  {
    CStudent ( "Peter John Taylor", CDate ( 1984, 1, 17), 2017 ),
    CStudent ( "John Peter Taylor", CDate ( 1983, 7, 13), 2014 ),
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 ),
    CStudent ( "Peter Taylor", CDate ( 1982, 2, 23), 2011 )
  }) );
  assert ( x0 . Search ( CFilter () . Name ( "james bond" ), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
  {
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 )
  }) );
  assert ( x0 . Search ( CFilter () . BornAfter ( CDate ( 1980, 4, 11) ) . BornBefore ( CDate ( 1983, 7, 13) ) . Name ( "John Taylor" ) . Name ( "james BOND" ), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
  {
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
  }) );
  assert ( x0 . Search ( CFilter () . Name ( "james" ), CSort () . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
  {
  }) );
  assert ( x0 . Suggest ( "peter" ) == (set<string>
  {
    "John Peter Taylor",
    "Peter John Taylor",
    "Peter Taylor"
  }) );
  assert ( x0 . Suggest ( "bond" ) == (set<string>
  {
    "Bond James",
    "James Bond"
  }) );
  assert ( x0 . Suggest ( "peter joHn" ) == (set<string>
  {
    "John Peter Taylor",
    "Peter John Taylor"
  }) );
  assert ( x0 . Suggest ( "peter joHn bond" ) == (set<string>
  {
  }) );
  assert ( x0 . Suggest ( "pete" ) == (set<string>
  {
  }) );
  assert ( x0 . Suggest ( "peter joHn PETER" ) == (set<string>
  {
    "John Peter Taylor",
    "Peter John Taylor"
  }) );
  assert ( ! x0 . AddStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
  assert ( x0 . DelStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
  assert ( x0 . Search ( CFilter () . BornAfter ( CDate ( 1980, 4, 11) ) . BornBefore ( CDate ( 1983, 7, 13) ) . Name ( "John Taylor" ) . Name ( "james BOND" ), CSort () . AddKey ( ESortKey::ENROLL_YEAR, false ) . AddKey ( ESortKey::BIRTH_DATE, false ) . AddKey ( ESortKey::NAME, true ) ) == (list<CStudent>
  {
    CStudent ( "James Bond", CDate ( 1982, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 8, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 17), 2013 ),
    CStudent ( "Bond James", CDate ( 1981, 7, 16), 2013 ),
    CStudent ( "James Bond", CDate ( 1981, 7, 16), 2012 ),
    CStudent ( "John Taylor", CDate ( 1981, 6, 30), 2012 )
  }) );
  assert ( ! x0 . DelStudent ( CStudent ( "James Bond", CDate ( 1981, 7, 16), 2013 ) ) );
  return 0;
}
#endif /* __PROGTEST__ */
