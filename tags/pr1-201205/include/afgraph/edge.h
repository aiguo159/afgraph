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


/**@file edge.h

*/


#ifndef _AFG_EDGE_H_
#define _AFG_EDGE_H_

#include <functional>
#include <iostream>

namespace afg
{

/**@defgroup gedges Different Edges
@ingroup graphd
*/
/**@{
*/
/** edge with nothing that we care about.*/
struct Ce_nCare {
	//int nx;
	Ce_nCare( void )
	{ }

	template<class T>
	Ce_nCare( const T & )
	{ }
};

/** edge with two weights
*/
template<class WType1, class WType2>
struct CEdgeW2 {
public:

	/// constructor, both weights need to have default constructors.
	CEdgeW2( WType1 w1 = WType1(), WType2 w2 = WType2() ):
		m_Weight1( w1 ), m_Weight2( w2 )
	{ }

	/// data member, first weight
	WType1 m_Weight1;
	/// data member, second weight
	WType2 m_Weight2;

	/// type of the 1st weight
	typedef WType1 WT1;
	/// type of the 2nd weight
	typedef WType2 WT2;

	/// return the 1st weight
	WType1 weight1( void ) const {
		return m_Weight1;
	}

	/// return the 2nd weight
	WType2 weight2( void ) const {
		return m_Weight2;
	}

}; // end of CEdgeW2

/** edge with three weights
*/
template<class WType1, class WType2, class WType3 >
struct CEdgeW3: public CEdgeW2<WType1, WType2> {

public:
	/// data member, the 3rd weight
	WType3 m_Weight3;

	/// constructor, all weights need to have default constructors.
	CEdgeW3( WType1 w1 = WType1(), WType2 w2 = WType2(), WType3 w3 = WType3() ):
		CEdgeW2<WType1, WType2>( w1, w2 ), m_Weight3( w3 )
	{ }

	/// type of the 3rd weight
	typedef WType3 WT3;

	/// return the 3rd weight
	WType3 weight3( void ) const {
		return m_Weight3;
	}
};

/**@name stream input/output opertors */
//@{

inline std::ostream &operator<<( std::ostream &os, const Ce_nCare &edge )
{
	return os;
}

/** conventional stream output operator for CEdgeW2.
    weights are separated by space.
*/
template<class WType1, class WType2>
inline std::ostream &operator<<( std::ostream &os,
								 const CEdgeW2<WType1, WType2>& edge )
{
	os << edge.m_Weight1 << " " << edge.m_Weight2;
	return os;
}

/** conventional stream input operator for CEdgeW2.
    Read two weights separated by space,
    may not work with some weight type.
*/
template<class WType1, class WType2>
inline std::istream &operator>>( std::istream &is,
								 CEdgeW2<WType1, WType2>& edge )
{
	is >> edge.m_Weight1 >> edge.m_Weight2;
	return is;
}

/** conventional stream output operator for CEdgeW3.
     weights are separated by space.
*/
template<class WType1, class WType2, class WType3 >
inline std::ostream &operator<<( std::ostream &os,
								 const CEdgeW3<WType1, WType2, WType3> &edge )
{
	os << edge.m_Weight1 << ' ' << edge.m_Weight2 << ' ' << edge.m_Weight3;
	return os;
}

/** conventional stream input operator for CEdgeW3.
    Read three weights separated by space,
    may not work with some weight type.
*/
template<class WType1, class WType2, class WType3 >
inline std::istream &operator>>( std::istream &is,
								 CEdgeW3<WType1, WType2, WType3> &edge )
{
	char c;
	is >> edge.m_Weight1 >> edge.m_Weight2 >> edge.m_Weight3;
	return is;
}

//@}

/**@}*/

}// end of namespace afg

#endif
