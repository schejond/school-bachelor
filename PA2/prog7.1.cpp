#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
public:
	CRect ( double x, double y, double w, double h )
		: m_X(x) , m_Y(y) , m_W(w) , m_H(h) {}
	
	friend ostream & operator << ( ostream & os, const CRect & x )
	{
		return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
	}
	double m_X;
	double m_Y;
	double m_W;
	double m_H;
};

#endif /* __PROGTEST__ */
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
class Component{
public:
	Component( int id , const CRect & relPos , const CRect & relPos2 )
		: id(id) , koef_position(relPos) , real_pos(relPos2) {}

	virtual ~Component() {}

	void compute_position( const double & window_x , const double & window_y , const double & window_w , const double & window_h )
	{
		double x = window_x + koef_position.m_X * window_w;
		double y = window_y + koef_position.m_Y * window_h;
		double w = window_w * koef_position.m_W;
		double h = window_h * koef_position.m_H;
		real_pos = CRect(x,y,w,h);			
	}

	virtual void print( ostream & out, const int situation = 0 )const = 0;
	virtual Component * Clone()const = 0;

	size_t get_id()const
	{
		return id;
	}

	friend ostream & operator << ( ostream & out , const Component & x );

protected:
	size_t id;
	CRect koef_position;
	CRect real_pos;
};
//-------------------------------------------------------------------------------------------------------------------------
class CButton : public Component{
public:
	CButton ( int id, const CRect & relPos, const string & name )
		: Component(id,relPos,relPos) , name(name) {}

	CButton( const CButton & src )
		: Component(src.id,src.koef_position,src.real_pos) , name(src.name) {}

	void print( ostream & out, const int situation )const
	{
		out << "[" << id << "] ";
		out << "Button \"";
		out << name;
		out << "\" " << real_pos;
		out << endl;
	}

	CButton * Clone()const
	{
		return new CButton(*this);
	}

protected:
	string name;
};
//-------------------------------------------------------------------------------------------------------------------------
class CInput : public Component{
public:
	CInput ( int id, const CRect & relPos, const string & value )
		: Component(id,relPos,relPos) , input_text(value) {}
	
	CInput( const CInput & src )
		: Component(src.id,src.koef_position,src.real_pos) , input_text(src.input_text) {}

	void print( ostream & out, const int situation )const
	{
		out << "[" << id << "] ";
		out << "Input \"";
		out << input_text;
		out << "\" " << real_pos;
		out << endl;
	}

	CInput * Clone()const
	{
		return new CInput(*this);
	}

	void SetValue( const string & x )
	{
		input_text = x;
	} 

	string GetValue()
	{
		return input_text;
	}

private:
	string input_text;
};
//-------------------------------------------------------------------------------------------------------------------------
class CLabel : public Component{
public:
	CLabel ( int id, const CRect & relPos, const string & label )
		: Component(id,relPos,relPos) , label_text(label) {}
	CLabel( const CLabel & src )
		: Component(src.id,src.koef_position,src.real_pos) , label_text(src.label_text) {}

	void print( ostream & out, const int situation )const
	{
		out << "[" << id << "] ";
		out << "Label \"";
		out << label_text;
		out << "\" " << real_pos;
		out << endl;
	}

	CLabel * Clone()const
	{
		return new CLabel(*this);
	}

protected:
	string label_text;
};
//-------------------------------------------------------------------------------------------------------------------------
class CComboBox : public Component{
public:
	CComboBox ( int id, const CRect & relPos )
		: Component(id,relPos,relPos) , selected(0) {}
	CComboBox( const CComboBox & src )
		: Component(src.id,src.koef_position,src.real_pos) , selected(src.selected)
	{
		Combos = src.Combos;
	}
	
	CComboBox & Add( const string & x )
	{
		Combos.push_back(x);
		return *this;
	}

	void print( ostream & out, const int situation )const
	{
		size_t curr_position = 0;
		out << "[" << id << "] ";
		out << "ComboBox " << real_pos;
		for( auto it : Combos)
		{
			out << endl;
			if( !situation )
				out << "   ";
			else if( situation == 21 )
				out << "|  ";
			out << "+-";
			if( curr_position == selected )
				out << ">" << it << "<";
			else
				out << " " << it;

			curr_position++;
		}
		out << endl;
	}

	CComboBox * Clone()const
	{
		return new CComboBox(*this);
	}

	size_t GetSelected()const
	{
		return selected;
	}

	void SetSelected( size_t x )
	{
		selected = x;
	}

private:
	vector<string> Combos;
	size_t selected;
};

class CWindow 
{
public:
	CWindow( const string & title, const CRect & absPos )
		: title(title) , position(absPos) {}

	CWindow( const CWindow & src )
		: title(src.title) , position(src.position)
	{
		for( auto it : src.Data )
		{
			Component * ptr = it->Clone();
			Data.push_back( ptr );
		}
	}

	~CWindow()
	{
		for(auto it : Data)
		{
			delete it;
		}
	}

	CWindow operator = ( const CWindow & x )
	{
		if( &x == this )
			return *this;
		
		title = x.title;
		position = x.position;
		for( auto del : Data )
		{
			delete del;
		}

		Data.clear();
		
		for( auto it : x.Data )
		{
			Component * ptr = it->Clone();
			Data.push_back(ptr);
		}

		return *this;
	}
	
	template <typename T>
	CWindow & Add( const T & x )
	{
		T * ptr = new T(x);
		ptr->compute_position( position.m_X , position.m_Y , position.m_W , position.m_H );
		Data.push_back(ptr);

		return *this;
	}
	Component * Search( size_t id_tofind )
	{
		for(auto it : Data)
		{
			if( it->get_id() == id_tofind )
				return it;
		}
		return nullptr;
	}

	void SetPosition( const CRect & new_position )
	{
		position = new_position;
		for( auto it : Data )
		{
			it->compute_position( position.m_X , position.m_Y , position.m_W , position.m_H );
		}
	}

	friend ostream & operator << ( ostream & out , const CWindow & x );

protected:
	string title;
	CRect position;
	vector<Component*> Data;
};
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
// output operators
ostream & operator << ( ostream & out , const CWindow & x )
{
	out << "Window \"" << x.title << "\" " << x.position << endl;
	auto it = x.Data.begin(); 
	for( ; it != (x.Data.end()-1) ; it++ )
	{
		out << "+- ";
		(*it)->print(out,21);
	}
	out << "+- ";
	(*it)->print(out);
	return out;
}

ostream & operator << ( ostream & out , const Component & x )
{
	x.print(out,66);
	return out;
}
//-------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  
  CWindow a ( "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . Add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . Add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . Add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . Add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . Add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . Add ( "Karate" ) . Add ( "Judo" ) . Add ( "Box" ) . Add ( "Progtest" ) );

  assert ( toString ( a ) ==
    "Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  
  CWindow b = a;
  assert ( toString ( b ) ==
    "Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  assert ( toString ( *b . Search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . GetSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . Search ( 20 ) ) . SetSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . GetValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . Search ( 11 ) ) . SetValue ( "chucknorris@fit.cvut.cz" );
  b . Add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . Add ( "PA2" ) . Add ( "OSY" ) . Add ( "Both" ) );
  assert ( toString ( b ) ==
    "Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . SetPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  return 0;
}
#endif /* __PROGTEST__ */
