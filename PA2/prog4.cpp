#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
class Person;

class MyString{
public:
	MyString();
	MyString( const char * input_text );
	MyString( const MyString & src );
	MyString( MyString && src );
	
	MyString operator = ( const MyString & src );
	MyString operator = ( MyString && src );

	~MyString();

	bool operator == ( const MyString & right )const;
	bool operator < ( const MyString & right )const;

	const char * get_text()const;
private:
	char * text;
	int text_length;

	void reset_after_move();
};
//--------------------------------------------------------------------------
//definice MyString
MyString::MyString()
	: text(nullptr) , text_length(0) {}

MyString::MyString( const char * input_text )
	: text_length(strlen(input_text) + 1)
{
	text = new char[text_length];
	memcpy(text,input_text,text_length);
}

MyString::MyString( const MyString & src )
{
	text_length = src.text_length;
	text = new char[text_length];
	memcpy(text,src.text,text_length);
}

MyString::MyString( MyString && src )
{
	text_length = src.text_length;
	text = src.text;

	src.reset_after_move();
}

MyString MyString::operator = ( const MyString & src )
{
	if( this == &src )
		return *this;

	delete[] text;
	text_length = src.text_length;
	text = new char[text_length];
	memcpy(text , src.text , text_length);

	return *this;
}

MyString MyString::operator = ( MyString && src )
{
	if( this == &src )
		return *this;

	text = src.text;
	text_length = src.text_length;

	src.reset_after_move();
	return *this;
}

MyString::~MyString()
{
	delete[] text;
}

const char * MyString::get_text()const
{
	return text;
}

bool MyString::operator == ( const MyString & right )const
{
	if( text_length != right.text_length )
		return false;	
	return strcmp( text , right.text ) == 0;
}

bool MyString::operator < ( const MyString & right )const
{
	const int minLength = min(text_length,right.text_length);

	for (int i = 0 ; i < minLength ; i++)
	{
		if( text[i] != right.text[i] )
		{
			return text[i] < right.text[i];
		}
	}

	return text_length < right.text_length;
}

void MyString::reset_after_move()
{
	text_length = 0;
	text = nullptr;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//slouzi pro rozhodovani zda nahlizim do inboxu/outboxu
enum class Mailbox{
	inbox,
	outbox
};
//--------------------------------------------------------------------------
class CMail{
public:
	CMail( const char *from, const char *to, const char *body );
	CMail() = default;

	bool operator == ( const CMail &x ) const;

	const MyString & getFrom()const;
	const MyString & getTo()const;

private:
	MyString from;
	MyString to;
	MyString body;
};
//--------------------------------------------------------------------------
//definice CMail
CMail::CMail( const char *from, const char *to, const char *body )
	: from(from) , to(to) , body(body) {}

bool CMail::operator == ( const CMail &x ) const
{
	return from == x.from && to == x.to && body == x.body;
}

const MyString & CMail::getFrom()const
{
	return from;
}
	
const MyString & CMail::getTo()const
{
	return to;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
template <typename T>
class MyVector{
public:
	MyVector();
	MyVector( const MyVector<T> & src );
	MyVector( MyVector<T> && src );

	MyVector operator = ( const MyVector<T> & src );
	MyVector operator = ( MyVector<T> && src );

	~MyVector();	
	
	void push_back( const T & x );
	void push_back( const T && x );
	void insert( const  T && x , const int position );

	int get_count()const;
	bool MyBinary_search( const MyString & name , int & position )const;//tyto 3 metody jsou jen pro vektor typu Person
	void inject_index( const int index , const int person_position , const Mailbox whichMailBox );
	const MyVector<int> & copy_box( const int person_position , const Mailbox whichMailBox )const;
	
	friend class CMailIterator;

private:
	T *Data;
	int length;
	int count;

	void resize();
	void reset_after_move();
};
//--------------------------------------------------------------------------
//definice MyVector
template <typename T>
MyVector<T>::MyVector()
{
	length = 21;
	Data = new T[length];
	count = 0;
}

template <typename T>
MyVector<T>::MyVector( const MyVector<T> & src )
	: length(src.length) , count(src.count)
{
	Data = new T[length];
	for (int i = 0; i < count ; ++i)
	{
		Data[i] = src.Data[i];
	}
}

template <typename T>
MyVector<T>::MyVector(MyVector<T> && src)
	: Data(src.start) , length(src.length) , count(src.cnt)
{
	src.reset_after_move();
}

template <typename T>
MyVector<T> MyVector<T>::operator = ( const MyVector<T> & src )
{
	if( this == &src )
		return *this;
	delete[] Data;
	length = src.length;
	count = src.count;
	Data = new T[length];
	for (int i = 0; i < count ; ++i)
	{
		Data[i] = src.Data[i];
	}
	return *this;
}

template <typename T>
MyVector<T> MyVector<T>::operator = ( MyVector<T> && src )
{
	if( this == &src )
		return *this;
	
	delete[] Data;	
	length = src.length;
	count = src.count;
	Data = src.Data;

	src.reset_after_move();
	return *this;
}

template <typename T>
MyVector<T>::~MyVector()
{
	delete[] Data;
}

template <typename T>
void MyVector<T>::push_back( const T & x )
{
	if( count == length )
		resize();
	Data[count++] = x;
}

template <typename T>
void MyVector<T>::push_back( const T && x )
{
	if( count == length )
		resize();
	Data[count++] = move(x);
}

template <typename T>
void MyVector<T>::insert( const  T && x , const int position )
{
	if( (count+1) == length )
		resize();

	for( int i = count ; i > position ; i-- )
	{
		Data[i] = move(Data[i-1]);
	}

	Data[position] = move(x);
	count++;
}

template<typename T>
int MyVector<T>::get_count()const
{
	return count;
}

template<typename T>
bool MyVector<T>::MyBinary_search( const MyString & src_name , int & position )const
{
	int from = 0;
	int to = count-1;
	int mid = (to-from)/2;

	if( count == 0  )
	{
		position = 0;
		return false;
	}

	while( from <= to )
	{
		mid = (to+from)/2 ;
		if( src_name < (Data[mid]).name  )
		{
			to = mid - 1;

		}
		else if( (Data[mid]).name == src_name )
		{
			position = mid;
			return true;
		}
		else
			from = mid + 1;
	}

	if( (Data[mid]).name < src_name )
		position = mid + 1;
	else
		position = mid;

	return false;
}

template<typename T>
void MyVector<T>::inject_index( const int index , const int person_position , const Mailbox whichMailBox )
{
	if( whichMailBox == Mailbox:: outbox )
		Data[person_position].add_to_outbox(index);
	else
		Data[person_position].add_to_inbox(index);	
}

template<typename T>
const MyVector<int> & MyVector<T>::copy_box( const int person_position , const Mailbox whichMailBox )const
{
	if( whichMailBox == Mailbox::outbox )
		return Data[person_position].Outbox;
	else
		return Data[person_position].Inbox;
}

template <typename T>
void MyVector<T>::resize()
{
	length = max( 20 , length*2 );
	T *tmp = new T[length];
	for( int i = 0 ; i < count ; i++)
	{
		tmp[i] = move(Data[i]);
	}
	delete[] Data;
	Data = tmp;
}

template <typename T>
void MyVector<T>::reset_after_move()
{
	Data = nullptr;
	length = 0;
	count = 0;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
class Person{
public:
	Person() = default;
	Person( const char *name );
	
	void add_to_inbox( const int index );
	void add_to_outbox( const int index );
	
	friend class MyVector<Person>;

private:
	MyString name;
	MyVector<int> Inbox;
	MyVector<int> Outbox;
};
//--------------------------------------------------------------------------
//definice Person
Person::Person( const char *name )
	: name(name) {}

void Person::add_to_inbox( const int index )
{
	Inbox.push_back(index);
}

void Person::add_to_outbox( const int index )
{
	Outbox.push_back(index);
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
class CMailIterator {
public:
	CMailIterator( const MyVector<int> & src , const MyVector<CMail> & email_src );
	CMailIterator( const CMailIterator & src);
	CMailIterator();

	operator bool () const;
	bool operator ! () const;
	const CMail & operator * () const;
	CMailIterator & operator ++ ();

private:
	int size;
	int position;
	MyVector<CMail> emails;
};
//--------------------------------------------------------------------------
//definice CMailIterator
CMailIterator::CMailIterator( const MyVector<int> & src , const MyVector<CMail> & email_src )
	: size(src.get_count()) , position(0)
{
	for( int i = 0 ; i < size ; i++ )
	{
		emails.push_back(email_src.Data[src.Data[i]]);
	}
}

CMailIterator::CMailIterator( const CMailIterator & src )
	: size(src.size) , position(src.position) , emails(src.emails) {}

CMailIterator::CMailIterator()
	: size(0) , position(0) {}

CMailIterator::operator bool () const
{
	return position < size;
}

bool CMailIterator::operator ! () const
{
	return !operator bool();
}

const CMail & CMailIterator::operator * () const
{
	return emails.Data[position];
}

CMailIterator & CMailIterator::operator ++ ()
{
	position++;
	return *this;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
class CMailServer 
{
public:
	CMailServer () = default;

	void SendMail ( const CMail & m );
	CMailIterator Outbox ( const char * email ) const;
	CMailIterator Inbox ( const char * email ) const;

private:
	MyVector<CMail> Emails_Array;
    MyVector<Person> Persons_Array;

	CMailIterator mailbox_Wrapper( const char * email , const Mailbox whichMailBox )const;
};
//--------------------------------------------------------------------------

void Send_Find_Inject( const Mailbox whichMailBox , MyVector<Person> & Data , int  & person_position , const CMail & src , const int email_position)
{
	if( whichMailBox == Mailbox::outbox && !Data.MyBinary_search( src.getFrom(),person_position) )
	{
		Data.insert( Person( src.getFrom().get_text() ) , person_position);
	}
	if( whichMailBox == Mailbox::inbox && !Data.MyBinary_search( src.getTo(),person_position) )
	{
		Data.insert( Person( src.getTo().get_text() ) , person_position);
	}

	Data.inject_index( email_position,person_position,whichMailBox );
}
//definice CMailServer
void CMailServer::SendMail( const CMail & src )
{
	Emails_Array.push_back(src);
	int email_position = Emails_Array.get_count() - 1;
	int person_position = 0;

	Send_Find_Inject( Mailbox::outbox,Persons_Array,person_position,src,email_position );
	Send_Find_Inject( Mailbox::inbox,Persons_Array,person_position,src,email_position );
}

CMailIterator CMailServer::Outbox ( const char * email ) const
{
	return mailbox_Wrapper(email,Mailbox::outbox);
}

CMailIterator CMailServer::Inbox ( const char * email ) const
{
	return mailbox_Wrapper(email,Mailbox::inbox);
}

CMailIterator CMailServer::mailbox_Wrapper( const char * email , const Mailbox whichMailBox )const
{
	int person_position = 0;

	MyString copy(email);
	if( Persons_Array.MyBinary_search(copy,person_position) )
		return CMailIterator( Persons_Array.copy_box(person_position,whichMailBox),Emails_Array );

	return CMailIterator();
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
#ifndef __PROGTEST__
int main ( void )
{
  char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );

  CMailServer s0;
  s0 . SendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . SendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . SendMail ( CMail ( from, to, body ) );
  s0 . SendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . SendMail ( CMail ( "peter", "alice", "PR bullshit" ) );

  CMailIterator i0 = s0 . Inbox ( "alice" );
  assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );

  assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ! ++i0 );

  CMailIterator i1 = s0 . Inbox ( "john" );
  assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  assert ( ! ++i1 );

  CMailIterator i2 = s0 . Outbox ( "john" );
  assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ! ++i2 );

  CMailIterator i3 = s0 . Outbox ( "thomas" );
  assert ( ! i3 );

  CMailIterator i4 = s0 . Outbox ( "steve" );
  assert ( ! i4 );

  CMailIterator i5 = s0 . Outbox ( "thomas" );
  s0 . SendMail ( CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! i5 );

  CMailIterator i6 = s0 . Outbox ( "thomas" );
  assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! ++i6 );

  CMailIterator i7 = s0 . Inbox ( "alice" );
  s0 . SendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ! ++i7 );

  CMailIterator i8 = s0 . Inbox ( "alice" );
  assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ! ++i8 );

  CMailServer s1 ( s0 );
  s0 . SendMail ( CMail ( "joe", "alice", "delivery details" ) );
  s1 . SendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  CMailIterator i9 = s0 . Inbox ( "alice" );
  assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( ! ++i9 );

  CMailIterator i10 = s1 . Inbox ( "alice" );
  assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  assert ( ! ++i10 );

  CMailServer s2;
  s2 . SendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  CMailIterator i11 = s2 . Inbox ( "alice" );
  assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  assert ( ! ++i11 );

  s2 = s0;
  s0 . SendMail ( CMail ( "steve", "alice", "newsletter" ) );
  s2 . SendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  CMailIterator i12 = s0 . Inbox ( "alice" );
  assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  assert ( ! ++i12 );

  CMailIterator i13 = s2 . Inbox ( "alice" );
  assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  assert ( ! ++i13 );

  return 0;
}
#endif /* __PROGTEST__ */
