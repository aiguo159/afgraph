/*@copyright

Copyright (c) 2000-2012 Aiguo Fei
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

/**
@file util_tl.hpp

collection of utility template classes

Aiguo Fei

March 1999, December 2000
*/

#ifndef _AFL_UTIL_TL_H_
#define _AFL_UTIL_TL_H_

#include <iostream>
#include <functional>

//#include <math.h>

//#include <cassert>
//#include <assert.h>


namespace afl
{

/**@defgroup misc_utils Miscellaneous utilities
*/

/**@{*/

/** class used to get input of one type from an input stream and
    convert it to another type.
*/
template<class T1, class T2>
struct in_convt2t {
	explicit in_convt2t( T2 &t )
		: t2( t )
	{}

	T2 &t2;

	/*std::istream& operator>>( std::istream&is ){
	  is>>t1;
	  t2=(T2)t1;
	  return is;
	  }*/

	//private:
	//T1 t1;

	// friend std::istream& operator>>( std::istream& is, in_convt2t<T1, T2>& i);
};

template<class T1, class T2>
std::istream &operator>>( std::istream &is, in_convt2t<T1, T2> i )
{
	T1 t1;
	is >> t1;
	i.t2 = ( T2 )t1;
	return is;
}

/// return the smaller one of two elements.
template<class T>
inline const T &tmin( const T &a, const T &b )
{
	return ( a < b ) ? a : b;
}

/// return the larger one of two elements.
template<class T>
inline const T &tmax( const T &a, const T &b )
{
	return ( a < b ) ? b : a;
}

/// return the smaller one of two elements, given comparison function object.
template<class T, class Cmp>
inline const T &tmin( const T &a, const T &b, Cmp cmp )
{
	return ( cmp( a, b ) ) ? a : b;
}

/// return the larger one of two elements, given comparison function object.
template<class T, class Cmp>
inline const T &tmax( const T &a, const T &b, Cmp cmp )
{
	return ( cmp( b, a ) ) ? b : a;
}


// The following function objects are useful sometimes.

/// Operator( ) always returns 1 for any reference passed.
template<class T>
struct runit: public std::unary_function<T, int> {
	int operator()( const T & ) const {
		return 1;
	}
};

/// Operator( ) always returns 1 for any pointer passed.
template<class T>
struct runit_p: public std::unary_function<T *, int> {
	int operator()( const T * ) const {
		return 1;
	}
};

/// Operator ( ) returns the value of a reference.
template<class T>
struct ref2value: public std::unary_function<T, T> {
	T operator()( const T &x ) const {
		return x;
	}
};

/// Operator ( ) returns a const reference of a reference.
template<class T>
struct ref2ref: public std::unary_function<T, const T &> {
	const T &operator()( const T &x ) const {
		return x;
	}
};

/// Operator ( ) returns the value of a pointer,
template<class T>
struct pointer2value: public std::unary_function<T *, T> {
	T operator()( const T *pt ) const {
		//assert( pt );
		return *pt;
	}
};

/**@}*/


/**@defgroup namedp Named Pair
@ingroup misc_utils
*/

/**@{*/

/** pair with a name and a key.
    Equal and not_equal (==, !=) comparisons are based on name only,
    while order operations (<, >) operations are based on key only.
*/
template<class NameT, class KeyT>
struct named_pair {

	/// name type
	typedef NameT name_type;
	/// key type
	typedef KeyT key_type;

	/** default constructor.
	    Both NameT and KeyT must have default constructor available to use this one.
	*/
	named_pair(): name( NameT() ), key( KeyT() )
	{}

	/// constructor
	named_pair( const NameT &_V1, const KeyT &_V2 )
		: name( _V1 ), key( _V2 )
	{}

	/// copy constructor
	template<class U, class V>
	named_pair( const named_pair<U, V> &p )
		: name( p.name ), key( p.key )
	{}

	/// data member: name, used for == and != comparisons.
	NameT name;
	/// data member: key, used for order comparisons (<, >)
	KeyT key;
};

/// compare two named pairs, equal if their names are the same.
template<class NameT, class KeyT>
inline
bool operator==( const named_pair<NameT, KeyT>& lhs,
				 const named_pair<NameT, KeyT>& rhs )
{
	return ( lhs.name == rhs.name );
}

/// compare two named pairs, not equal if their names are different.
template<class NameT, class KeyT>
inline
bool operator!=( const named_pair<NameT, KeyT>& lhs,
				 const named_pair<NameT, KeyT>& rhs )
{
	return ( !( lhs == rhs ) );
}

/// compare a named pair with a name
template<class NameT, class KeyT>
inline
bool operator==( const named_pair<NameT, KeyT>& lhs,
				 const NameT &n )
{
	return ( lhs.name == n );
}

/// compare a named pair with a name
template<class NameT, class KeyT>
inline
bool operator!=( const named_pair<NameT, KeyT>& lhs,
				 const NameT &n )
{
	return ( !( lhs.name == n ) );
}

/// compare two named pairs by their keys.
template<class NameT, class KeyT>
inline
bool operator<( const named_pair<NameT, KeyT>& lhs,
				const named_pair<NameT, KeyT>& rhs )
{
	return ( lhs.key < rhs.key );
}

/// compare two named pairs by their keys.
template<class NameT, class KeyT>
inline
bool operator>( const named_pair<NameT, KeyT>& lhs,
				const named_pair<NameT, KeyT>& rhs )
{
	return ( rhs < lhs );
}

/// compare two named pairs by their keys.
template<class NameT, class KeyT>
inline
bool operator<=( const named_pair<NameT, KeyT>& lhs,
				 const named_pair<NameT, KeyT>& rhs )
{
	return ( !( rhs < lhs ) );
}

/// compare two named pairs by their keys.
template<class NameT, class KeyT>
inline
bool operator>=( const named_pair<NameT, KeyT>& lhs,
				 const named_pair<NameT, KeyT>& rhs )
{
	return ( !( lhs < rhs ) );
}

/// Output two values within parenthese, separated by ",".
template<class NameT, class KeyT>
inline
std::ostream &operator<<( std::ostream &os,
						  const named_pair<NameT, KeyT> &np )
{
	if( os ) {
		os << "(" << np.name << ", " << np.key << ")";
	}
	return os;
}

/// produce a named_pair from two values
template<class NameT, class KeyT>
inline
named_pair<NameT, KeyT> make_npair( const NameT &lhs,
									const KeyT &rhs )
{
	return ( named_pair<NameT, KeyT>( lhs, rhs ) );
}

/**@}*/

} // end of namespace afl

#endif

