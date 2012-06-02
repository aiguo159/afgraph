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


/**@file path.h

  helper class and functions for path manipulation.

*/

#ifndef _AFG_PATH_H_
#define _AFG_PATH_H_

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

#include <sstream>

#include "gexception.h"

namespace afg
{

/**@defgroup ghelper Helper Classes and Utilities
@ingroup graphd
*/

/**@{*/

/** helper class for path manipulation.
    A path is list of integers representing indices in a graph.
    @see IGraph
*/
class CPath: public std::list<int>
{
public:
	CPath(): std::list<int>()
	{}

	CPath( int i ): std::list<int>() {
		push_back( i );
	}

	/** construct a path from nsource to ndest from a predecessor vector.
	    @param nsource source node
	    @param ndest destination node
	    @param vpred predecessor index vector. vpred[i] is the node that comes
	      just before i on the shortest path from nsource to ndest.
	      vpred should be generated from some shortest-path algorithm
	      such as Dijkstras's.
	*/
	CPath( int nsource, int ndest, const std::vector<int>& vpred ) {
		if( vpred.size() < 2 ) {
			return;
		}
		int n = vpred.size();
		if( ndest >= n || ndest < 0 || nsource < 0 || nsource >= n ) {
			return;
		}
		int ntmp = ndest, ncount = 0;
		while( ntmp != nsource && ntmp >= 0 && ntmp < n && ncount++ < n ) {
			push_front( ntmp );
			ntmp = vpred[ntmp];
		}
		// while can be in an infinite loop if vpred is deliberately constructed.
		// use a counter to break that: any path can never be more than n hops
		// (path with loop can't be constructed from a predecessor index vector).

		if( ntmp != nsource ) {
			clear();
		}
		// nsource can't be reversely reached from ndest, set path empty

		push_front( nsource );
	}

	/// get the source node of the path
	int source( void ) const {
		return front();
	}
	/// get the destination node of the path
	int dest( void ) const {
		return back();
	}

	/** insert i before n
	    @return ture if n found, false otherwise
	*/
	bool insert_before( int i, int n ) {
		std::list<int>::iterator it = std::find( begin(), end(), n );
		if( it != end() ) {
			insert( it, i );
			return true;
		}
		return false;
	}

	/** insert i after n
	    @return ture if n found, false otherwise
	*/
	bool insert_after( int i, int n ) {
		std::list<int>::iterator it = std::find( begin(), end(), n );
		if( it != end() ) {
			++it;
			insert( it, i );
			return true;
		}
		return false;
	}

	/// output path as a list of vertex (value) connected by a given connector.
	template<class GraphT, class Fun>
	std::ostream& output( std::ostream &os, GraphT &gra, Fun fv,
		const std::string &connector=" -> " )
	{
		const_iterator it = begin();
		if( it != end() ){
			os << fv( gra[ *it ] ); ++it;
			while( it != end() ){
				os << connector << fv( gra[ *it ] ); ++it;
			}
		}
		return os;
	}

};

/**@name path related utility functions */

//@{
/// output operator to output path as a list of indices separated by space.
inline std::ostream &operator <<( std::ostream &os, CPath &lp )
{
	std::copy( lp.begin(), lp.end(), std::ostream_iterator<int>( os, " " ) );
	return os;
}

/** get path from a predcessor index array.
    @param n number of nodes
    @param pnpred predcessor array, at least of size n.
    If array is smaller than that, out of range error may happen.
    pnpred[i] is the node (index) closest to i on the shortest path
    from source node ns to i.
    @param ns source node
    @param nt destination node
    @param lp result path
    @return true if path is successfully constructed, false otherwise.
*/
bool pred2path( int n, int *pnpred, int ns, int nt, CPath &lp )
{
	lp.clear();
	if( nt >= n || nt < 0 || ns < 0 || ns >= n ) {
		return false;
	}
	if( nt == ns ) {
		lp.push_front( ns );
		return true;
	}

	// while can be in an infinite loop if vpred is deliberately constructed.
	// use a counter to break that: any path can never be more than n hops
	// (path with loop can't be constructed from a predecessor index vector).
	int i = nt, ncount = 0;
	while( i >= 0 && i < n && ncount++ < n ) {
		lp.push_front( i );
		i = pnpred[i];
		if( i == ns ) {
			lp.push_front( ns );
			return true;
		}
	}

	lp.clear();
	return false;
}

/** get path from a predcessor vector.
    @param vpred predcessor vector.
    vpred[i] is the node (index) closest to i on the shortest path
    from source node ns to i.
    @param ns source node
    @param nt destination node
    @param lp result path
    @return true if path is successfully constructed, false otherwise.
*/
bool pred2path( const std::vector<int>& vpred, int ns, int nt, CPath &lp )
{
	lp.clear();
	int n = vpred.size();
	if( nt >= n || nt < 0 || ns < 0 || ns >= n ) {
		return false;
	}
	if( nt == ns ) {
		lp.push_front( ns );
		return true;
	}

	int i = nt, ncount = 0;
	while( i >= 0 && i < n && ncount++ < n ) {
		lp.push_front( i );
		i = vpred[i];
		if( i == ns ) {
			lp.push_front( ns );
			return true;
		}
	}

	lp.clear();
	return false;
}

/** get path from a n*n predcessor vector.
    @param vpred predcessor vector, size at least n*n.
    vpred[i*n+j] is the node (index) closest to j on the shortest path
    from source node i to j.
    @see pred2path
*/
bool allpred2path( int n, const std::vector<int>& vpred, int ns,
				   int nt, CPath &lp )
{
	lp.clear();
	if( nt >= n || nt < 0 || ns < 0 || ns >= n ) {
		return false;
	}
	if( nt == ns ) {
		lp.push_front( ns );
		return true;
	}

	int i = nt, ncount = 0;
	while( i >= 0 && i < n && ncount++ < n ) {
		lp.push_front( i );
		i = vpred[ns*n+i];
		if( i == ns ) {
			lp.push_front( ns );
			return true;
		}
	}

	lp.clear();
	return false;
}

/** get path length.
    GraphT is the type of graph passed.
    @param gra graph to work on, to map the edges specified in lp.
    @param fw function object to get the length or "weight" of an edge.
      It takes a GraphT::EdgeDT* and returns a weight.
    @exception invalid_path, with message
      "in path_length( ): edge from->to not found".
*/
template<class GraphT, class Fun>
typename Fun::result_type path_length( const GraphT &gra,
									    const CPath &lp, Fun fw )
{
	typedef typename Fun::result_type WeightT;
	//typedef typename GraphT::EDT EDT;

	if( lp.empty() ) {
		return ( WeightT )0;
	}

	WeightT w = ( WeightT )0;
	typename CPath::const_iterator ilp = lp.begin();
	int n1 = *ilp;
	int n2 = n1;
	const typename GraphT::EDT *pedg;
	++ilp;
	while( ilp != lp.end() ) {
		n1 = n2;
		n2 = *ilp;
		if(( pedg = gra.get_edge( n1, n2 ) ) ) {
			w = w + fw( pedg );
		} else {
			// edge not found, throw exception
			std::ostringstream ost;
			ost << "in path_length( ): edge " << n1 << "->" << n2 << " not found";
			throw invalid_path( ost.str() );
		}
		++ilp;
	}
	return w;
}

//@}

/**@}*/

} // end of namespace afg

#endif
