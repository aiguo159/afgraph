/*@copyright

Copyright (c) 2001 Aiguo Fei, University of California, Los Angeles
Copyright (c) 2012 Aiguo Fei
All rights reserved.

Permission to use and re-distribute this code in source and binary forms
with or without modifications is therefore granted provided that
the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Information regarding where to obtain the original source code must be
      included with the redistrbution.

*NO WARRANTY*
THIS CODE OR PROGRAM IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND CONTRIBUTOR(S)
"AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE. THE COPYRIGHT HOLDER(S) AND CONTRIBUTOR(S) MUST NOT BE
HELD RESPONSIBLE FOR ANY DAMAGE OR UNDESIRED EFFECT CAUSED DIRECTLY OR
INDIRECTLY BY USE, MISUSE OR ABUSE OF THIS CODE OR PROGRAM.

*/


/**@file vertex.h

  pre-defined vertex types

*/

#ifndef _AFG_VERTEX_H_
#define _AFG_VERTEX_H_

#include <string>
#include <iostream>

namespace afg
{

/**@defgroup gvertex Different Vertices
@ingroup graphd
*/

/**@{*/

/** vertex with an ID and some other data.
The other data doesn't matter in comparison operations.
*/
template<class IDT, class DT>
struct iddVertex {
	IDT id;
	DT data;

	explicit iddVertex( const IDT &i = IDT(), const DT &d = DT() )
		: id( i ), data( d )
	{}
}
;

template<class IDT, class DT>
inline bool operator==( const iddVertex<IDT, DT>& lhs,
						const iddVertex<IDT, DT>& rhs )
{
	return ( lhs.id == rhs.id );
}

template<class IDT, class DT>
inline bool operator<( const iddVertex<IDT, DT>& lhs,
					   const iddVertex<IDT, DT>& rhs )
{
	return ( lhs.id < rhs.id );
}

template<class IDT, class DT>
inline bool operator!=( const iddVertex<IDT, DT>& lhs,
						const iddVertex<IDT, DT>& rhs )
{
	return !( lhs == rhs );
}

template<class IDT, class DT>
inline std::ostream &operator<<( std::ostream &os,
								 const iddVertex<IDT, DT>& rhs )
{
	os << rhs.id << ' ' << rhs.data;
	return os;
}

template<class IDT, class DT>
inline std::istream &operator>>( std::istream &is,
								 iddVertex<IDT, DT>& rhs )
{
	is >> rhs.id >> rhs.data;
	return is;
}

/** vertex with an id and a weight of type double
*/
template<class IDT>
struct idwVertex: public iddVertex<IDT, double> {

	explicit idwVertex( const IDT &i = IDT(), double d = 0.0 )
		: iddVertex<IDT, double>( i, d )
	{}

	double &weight( void ) {
		return iddVertex<IDT, double>::data;
	}

	double weight( void ) const {
		return iddVertex<IDT, double>::data;
	}
};

/** vertex with x,y coordinates only.
T is the type of the coordinate, normally integer or double.
*/
template<class T>
struct xyVertex {
	/// data member, x coordinate
	T m_x;
	/// data member, y coordinate
	T m_y;

	xyVertex( T tx = ( T ) - 1, T ty = ( T ) - 1 ): m_x( tx ), m_y( ty )
	{ }

	T &x( void ) {
		return m_x;
	}

	T x( void ) const {
		return m_x;
	}

	T &y( void ) {
		return m_y;
	}

	T y( void ) const {
		return m_y;
	}
};

template<class T>
inline bool operator ==( const xyVertex<T>& lhs, const xyVertex<T>& rhs )
{
	return ( lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y );
}

template<class T>
inline bool operator !=( const xyVertex<T>& lhs, const xyVertex<T>& rhs )
{
	return !( lhs == rhs );
}

template<class T>
std::ostream &operator<<( std::ostream &os, const xyVertex<T>& v )
{
	os << '(' << v.m_x << ", " << v.m_y << ')';
	return os;
}

template<class T>
std::istream &operator>>( std::istream &is, xyVertex<T>& v )
{
	char c;
	is >> c >> v.m_x >> c >> v.m_y >> c;
	return is;
}

/** vertex with an id and x,y coordinates.
IDT: type the id, T: coordinate type.
*/
template<class IDT, class T>
struct ixyVertex: public xyVertex<T> {
	IDT m_id;

	explicit ixyVertex( IDT i = IDT(), T tx = ( T ) - 1, T ty = ( T ) - 1 )
		: xyVertex<T> ( tx, ty ), m_id( i )
	{ }

	IDT &id( void ) {
		return m_id;
	}

	IDT id( void ) const {
		return m_id;
	}

};

template<class IDT, class T>
inline bool operator ==( const ixyVertex<IDT, T>& lhs,
						 const ixyVertex<IDT, T>& rhs )
{
	return ( lhs.m_id == rhs.m_id && lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y );
}

template<class IDT, class T>
inline bool operator !=( const ixyVertex<IDT, T>& lhs,
						 const ixyVertex<IDT, T>& rhs )
{
	return !( lhs == rhs );
}

template<class IDT, class T>
std::ostream &operator<<( std::ostream &os,
						  const ixyVertex<IDT, T>& v )
{
	os << '(' << v.m_id << ": " << v.m_x << ", " << v.m_y << ')';
	return os;
}

/** vertex for transit-stub graph.
*/
#if 0
struct tsVertex {
	/// node id
	int id;
	/// data member, x coordinate
	int x;
	/// data member, y coordinate
	int y;
	std::string sLabel;

	tsVertex( int i = -1, int nx = -1, int ny = -1,
			  std::string l = std::string( "S" ) )
		: id( i ), x( nx ), y( ny ), sLabel( l ) { }

	/** type of the node.
	  @return node type, 'T': tranist node, 'S': stub node
	*/
	char type( void ) const {
		return sLabel.c_str()[0];
	}
};

inline bool operator ==( const tsVertex &v1, const tsVertex &v2 )
{
	return ( v1.id == v2.id && v1.x == v2.x && v1.y == v2.y );
}

inline bool operator !=( const tsVertex &v1, const tsVertex &v2 )
{
	return !( v1 == v2 );
}
#endif

struct tsVertex: public ixyVertex<int, int> {

	std::string sLabel;

	tsVertex( int i = -1, int nx = -1, int ny = -1,
			  std::string l = std::string( "S" ) )
		: ixyVertex<int, int>( i, nx, ny ), sLabel( l )
	{ }

	/** type of the node.
	  @return node type, 'T': tranist node, 'S': stub node
	*/
	char type( void ) const {
		return sLabel.c_str()[0];
	}
};

inline std::istream &operator>>( std::istream &is, tsVertex &v )
{
	is >> v.m_id >> v.sLabel >> v.x() >> v.y();
	return is;
}

inline std::ostream &operator<<( std::ostream &os, const tsVertex &v )
{
	os << v.m_id << ' ' << v.sLabel << ' ' << v.x() << ' ' << v.y();
	return os;
}

/**@} */

}

#endif
