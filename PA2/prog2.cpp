#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */
//------------------------------------------------------------------------------
class PERSON_name
{
public:
	PERSON_name( const string & name , const string & surname , const string & email , const unsigned int salary )
		: Per_name(name) , Per_surname(surname) , Per_email(email) , Per_salary(salary) {}
  PERSON_name( const string & name , const string & surname, const unsigned int salary = 0 )
    :  Per_name(name) , Per_surname(surname) , Per_salary(salary) {}


    bool operator < (const PERSON_name& x) const
    {
    	if(Per_surname == x.Per_surname)
    		return (Per_name < x.Per_name);
        return (Per_surname < x.Per_surname );
    }

    string Per_name;
    string Per_surname;
    string Per_email;
    unsigned int Per_salary;
};
class PERSON_email
{
public:
  PERSON_email( const string & name , const string & surname , const string & email , const unsigned int salary )
    : Per_name(name) , Per_surname(surname) , Per_email(email) , Per_salary(salary) {}
  PERSON_email( const string & email , const unsigned int salary = 0 )
    : Per_email(email) , Per_salary(salary) {}


    bool operator < (const PERSON_email& x) const
    {
        return (Per_email < x.Per_email );
    }

    string Per_name;
    string Per_surname;
    string Per_email;
    unsigned int Per_salary;
};
//------------------------------------------------------------------------------
class CPersonalAgenda
{
  public:
	CPersonalAgenda( ) {}
//------------------------------------------------------------------------------
	~CPersonalAgenda( ) {} 
//------------------------------------------------------------------------------
	vector<PERSON_name> V_ByName;
  vector<PERSON_email> V_ByEmail;
//------------------------------------------------------------------------------
	bool Add( const string & name, const string & surname, const string & email, const unsigned int salary )
	{
    vector<PERSON_name>::iterator it_Name;
    PERSON_name new_to_Name( name , surname , email , salary );
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , new_to_Name );
    if( V_ByName.size() != 0 )
    {
      if( (it_Name+1) == upper_bound( V_ByName.begin() , V_ByName.end() , new_to_Name ) )
        return false;
     }

    vector<PERSON_email>::iterator it_Email;
    PERSON_email new_to_Email( name , surname , email , salary );
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , new_to_Email );
    if( V_ByEmail.size() != 0 )
    {
      if( (it_Email+1) == upper_bound( V_ByEmail.begin() , V_ByEmail.end() , new_to_Email ) )
         return false;
    }

	V_ByName.insert( it_Name , new_to_Name );
    V_ByEmail.insert( it_Email , new_to_Email );
		return true;
	}
//------------------------------------------------------------------------------
	bool Del( const string & name, const string & surname )
	{
    vector<PERSON_name>::iterator it_Name;
    vector<PERSON_email>::iterator it_Email;

    PERSON_name find_me(name,surname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , find_me );
    if( it_Name == upper_bound( V_ByName.begin() , V_ByName.end() , find_me ) )
      return false;
    
    PERSON_email in_emails(it_Name->Per_email);
    V_ByName.erase(it_Name);
    
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , in_emails );
    V_ByEmail.erase(it_Email);
    
    return true;
	}
//------------------------------------------------------------------------------
	bool Del ( const string & email )
	{
    vector<PERSON_name>::iterator it_Name;
    vector<PERSON_email>::iterator it_Email;
    
    PERSON_email find_me(email);
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , find_me );
    if( it_Email == upper_bound( V_ByEmail.begin() , V_ByEmail.end() , find_me ) )
      return false;
    
    PERSON_name in_names(it_Email->Per_name , it_Email->Per_surname);
    V_ByEmail.erase(it_Email);
    
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , in_names );
    V_ByName.erase(it_Name);
    
    return true;
	}
//------------------------------------------------------------------------------
  bool ChangeName ( const string & email, const string & newName, const string & newSurname )
  {
    vector<PERSON_name>::iterator it_Name;//kontrola, zda jde dane jmeno pridat
    PERSON_name new_name_check(newName,newSurname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , new_name_check );
    if( it_Name->Per_name == newName && it_Name->Per_surname == newSurname )
      return false;

    vector<PERSON_email>::iterator it_Email;
    PERSON_email is_email_there(email);
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , is_email_there );
    if( it_Email == upper_bound( V_ByEmail.begin() , V_ByEmail.end() , is_email_there ) )
      return false;

    PERSON_name to_Delete( it_Email->Per_name , it_Email->Per_surname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , to_Delete );
    V_ByName.erase(it_Name);
    
    it_Email->Per_name = newName;
    it_Email->Per_surname = newSurname;

    PERSON_name new_name(newName,newSurname,email,it_Name->Per_salary);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , new_name );
    V_ByName.insert(it_Name , new_name);

    return true;
  }
//------------------------------------------------------------------------------
  bool ChangeEmail ( const string & name, const string & surname, const string & newEmail )
  {
    vector<PERSON_email>::iterator it_Email; // jde ten novy mail?
    PERSON_email is_email_there(newEmail);
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , is_email_there );
    if( it_Email->Per_email == newEmail )
      return false;

    vector<PERSON_name>::iterator it_Name;
    PERSON_name find_him(name,surname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , find_him );
    if( it_Name == upper_bound( V_ByName.begin() , V_ByName.end() , find_him ) )
      return false;

    PERSON_email new_email(name,surname,newEmail,it_Name->Per_salary);
    PERSON_email delete_him(it_Name->Per_email);

    it_Name->Per_email = newEmail;

    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , delete_him);
    V_ByEmail.erase(it_Email);

    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , new_email);
    V_ByEmail.insert(it_Email , new_email);
    
    return true;
  }
//------------------------------------------------------------------------------
  bool SetSalary ( const string & name, const string & surname, unsigned int salary )
  {
    vector<PERSON_name>::iterator it_Name;
    PERSON_name obj_to_change(name,surname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , obj_to_change );
    if( it_Name == upper_bound( V_ByName.begin() , V_ByName.end() , obj_to_change ) )
      return false;
    it_Name->Per_salary = salary;

    vector<PERSON_email>::iterator it_Email;
    PERSON_email change_in_email( it_Name->Per_email );
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , change_in_email );
    it_Email->Per_salary = salary;

    return true;
  }
//------------------------------------------------------------------------------
  bool SetSalary ( const string & email, unsigned int salary )
  {
    vector<PERSON_email>::iterator it_Email;
    PERSON_email obj_to_change(email);
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , obj_to_change );
    if( it_Email == upper_bound( V_ByEmail.begin() , V_ByEmail.end() , obj_to_change ) )
      return false;
    it_Email->Per_salary = salary;

    vector<PERSON_name>::iterator it_Name;
    PERSON_name change_in_name( it_Email->Per_name , it_Email->Per_surname );
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , change_in_name );
    it_Name->Per_salary = salary;

    return true;
  }
//------------------------------------------------------------------------------
    unsigned int  GetSalary ( const string & name, const string & surname ) const
    {
    vector<PERSON_name>::const_iterator it_Name;
    PERSON_name x(name,surname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , x );
    if( V_ByName.size() == 0 || it_Name == upper_bound( V_ByName.begin() , V_ByName.end() , x ) )
      return 0;

    return it_Name->Per_salary;
    }
//------------------------------------------------------------------------------
  unsigned int GetSalary ( const string & email ) const
  {
    vector<PERSON_email>::const_iterator it_Email;
    PERSON_email x(email);
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , x );
    if( V_ByEmail.size() == 0 ||  it_Email == upper_bound( V_ByEmail.begin() , V_ByEmail.end() , x) )
      return 0;

    return it_Email->Per_salary;
  }
//------------------------------------------------------------------------------
  bool GetRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const
  {
    vector<PERSON_name>::const_iterator it_Name;
    PERSON_name x(name,surname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , x );
    if( it_Name == upper_bound( V_ByName.begin() , V_ByName.end() , x ) )
      return false;

    unsigned int lower = 0 , sum = 0;
    const unsigned int value = it_Name->Per_salary;
    for( unsigned int i = 0; i < V_ByName.size() ; i++ )
    {
      if( V_ByName[i].Per_salary < value )
        lower++;
      else if( V_ByName[i].Per_salary == value )
        sum++;
    }
    rankMin = lower;
    rankMax = lower + sum -1 ;
    return true;
  }
//------------------------------------------------------------------------------
  bool GetRank ( const string & email, int & rankMin, int & rankMax ) const
  {
    vector<PERSON_email>::const_iterator it_Email;
    PERSON_email x(email);
    it_Email = lower_bound( V_ByEmail.begin() , V_ByEmail.end() , x );
    if( it_Email == upper_bound( V_ByEmail.begin() , V_ByEmail.end() , x ) )
      return false;

    unsigned int lower = 0 , sum = 0;
    const unsigned int value = it_Email->Per_salary;
    for( unsigned int i = 0; i < V_ByEmail.size() ; i++ )
    {
      if( V_ByEmail[i].Per_salary < value )
        lower++;
      else if( V_ByEmail[i].Per_salary == value )
        sum++;
    }
    rankMin = lower;
    rankMax = lower + sum -1 ;
    return true;
  }
//------------------------------------------------------------------------------
	bool GetFirst ( string & outName, string & outSurname ) const
	{
		if( V_ByName.size() == 0 )
			return false;
		outName = V_ByName[0].Per_name;
		outSurname = V_ByName[0].Per_surname;
		return true;
	}
//------------------------------------------------------------------------------
  bool GetNext ( const string & name, const string & surname, string & outName, string & outSurname ) const
  {
    vector<PERSON_name>::const_iterator it_Name;
    PERSON_name x( name , surname);
    it_Name = lower_bound( V_ByName.begin() , V_ByName.end() , x );
    if( name != it_Name->Per_name || surname != it_Name->Per_surname || (it_Name+1) == V_ByName.end() )
      return false;
    
    outName = (it_Name+1)->Per_name;
    outSurname = (it_Name+1)->Per_surname;
    return true;
  }
//------------------------------------------------------------------------------
	void print_by_name()//moje
	{
		cout << "Vypis vsech prvku serazenych podle jmen:" << endl;
		for( unsigned int i = 0 ; i < V_ByName.size() ; i++ )
		{
			cout << V_ByName[i].Per_surname << "---" << V_ByName[i].Per_name << endl;
			cout << V_ByName[i].Per_email << "---" << V_ByName[i].Per_salary << endl;
			if( i < (V_ByName.size() - 1) )
				cout << "////////////////////////" << endl;
		}
		cout << "Konec vypisu. Pocet prvku: " << V_ByName.size() << endl;
    }
  void print_by_email()//moje
  {
    cout << "Vypis vsech prvku serazenych podle emailu:" << endl;
    for( unsigned int i = 0 ; i < V_ByEmail.size() ; i++ )
    {
      cout << V_ByEmail[i].Per_email << "---" << V_ByEmail[i].Per_salary << endl;
      cout << V_ByEmail[i].Per_surname << "---" << V_ByEmail[i].Per_name << endl;
      if( i < (V_ByEmail.size() - 1) )
        cout << "////////////////////////" << endl;
    }
    cout << "Konec vypisu. Pocet prvku: " << V_ByEmail.size() << endl;
    }

  private:

};
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
 int lo, hi;

	CPersonalAgenda b1;
 //  b1.print_by_name();
  //cout << "-------------" << endl;
  // b1.print_by_email();
	assert ( b1 . Add ( "John", "Smith", "john", 30000 ) );
	assert ( b1 . Add ( "John", "Miller", "johnm", 35000 ) );
	assert ( b1 . Add ( "Peter", "Smith", "peter", 23000 ) );
	assert ( b1 . GetFirst ( outName, outSurname )
				&& outName == "John"
				&& outSurname == "Miller" );
	assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . GetNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . GetNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . SetSalary ( "john", 32000 ) );
  assert ( b1 . GetSalary ( "john" ) ==  32000 );
  assert ( b1 . GetSalary ( "John", "Smith" ) ==  32000 );
  assert ( b1 . GetRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . GetRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . GetRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . SetSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . GetSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . GetSalary ( "john" ) ==  35000 );
  assert ( b1 . GetRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . GetRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . GetRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . ChangeName ( "peter", "James", "Bond" ) );
  assert ( b1 . GetSalary ( "peter" ) ==  23000 );
  assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . GetSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . GetFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . GetNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . GetNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . ChangeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . GetSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . GetSalary ( "james" ) ==  23000 );
  assert ( b1 . GetSalary ( "peter" ) ==  0 );
  assert ( b1 . Del ( "james" ) );
  assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . Del ( "John", "Miller" ) );
  assert ( b1 . GetRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . GetFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . GetNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . Del ( "john" ) );
  assert ( ! b1 . GetFirst ( outName, outSurname ) );
  assert ( b1 . Add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . Add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . Add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . GetFirst ( outName, outSurname ) );
  assert ( b2 . Add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . Add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . Add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . Add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . SetSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . SetSalary ( "joe", 90000 ) );
  assert ( b2 . GetSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . GetSalary ( "joe" ) ==  0 );
  assert ( ! b2 . GetRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . GetRank ( "joe", lo, hi ) );
  assert ( ! b2 . ChangeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . ChangeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . Del ( "Joe", "Black" ) );
  assert ( ! b2 . Del ( "joe" ) );
  assert ( ! b2 . ChangeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . ChangeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . Del ( "Peter", "Smith" ) );
  assert ( ! b2 . ChangeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . SetSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . GetSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . GetRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . ChangeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . SetSalary ( "peter", 37000 ) );
  assert ( b2 . GetSalary ( "peter" ) ==  0 );
  assert ( ! b2 . GetRank ( "peter", lo, hi ) );
  assert ( ! b2 . Del ( "Peter", "Smith" ) );
  assert ( ! b2 . Del ( "peter" ) );
  assert ( b2 . Add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . GetSalary ( "peter" ) ==  40000 );

  return 0;
}
#endif /* __PROGTEST__ */
