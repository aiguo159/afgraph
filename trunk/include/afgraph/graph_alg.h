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


/**@file graph_alg.h

  some graph algorithms

  Aiguo Fei

  1999-2001

*/

#ifndef _GRAPH_ALG_H_
#define _GRAPH_ALG_H_

#include <vector>

namespace afg
{

/**@defgroup galg Graph Algorithms
*/

/**@defgroup ggenalg general graph algorithms
@ingroup galg
*/

/**@{*/


/** Determine if a graph is connected.
  GT: a graph type provides interface defined in graph_intf.h.
  @param grf the graph, doesn't need to be "packed" beforehand.
  @return true if grf is connected, false otherwise
*/
template<class GT>
bool is_connected( const GT &grf )
{
	// find distances between all pairs
	int n = grf.range();
	//std::cout<<"n: "<<n<<std::endl;

	std::vector<int> vdist( n * n, n * n );
	// vdist[j*n+i] is distance between (i,j)
	// first, if there is edge (i,j) then d(i,j)=1;
	typename GT::const_e_iterator eit;
	int i, j, k;
	for( i = 0; i < n; ++i ) {
		vdist[i *n+i] = 0;
		for( eit = grf.e_begin( i );
			 eit != grf.e_end( i ); ++eit ) {
			vdist[( *eit ).to( )*n+i] = 1;
		}
	}
	// run an all-pair shortest-path algorithm
	for( i = 0; i < n; ++i ) {
		for( j = 0; j < n; ++j )
			for( k = 0; k < n; ++k )
				if( vdist[i*n+j] + vdist[k*n+i] < vdist[k*n+j] ) {
					vdist[k *n+j] = vdist[i*n+j] + vdist[k*n+i];
				}
	}

	// see if graph is connected by looking at distance between every pair
	// note: unused indices have to be excluded
	for( i = 0; i < n; ++i ) {
		for( j = 0; j < n; ++j ) {
			if( j != i ) {
				//std::cout<<i<<", "<<j<<": "<<vdist[j*n+i]<<std::endl;
				if( vdist[j *n+i] >= n * n && grf.is_in_use( i ) && grf.is_in_use( j ) ) {
					return false;
				}
			}
		}
	}
	//cout<<endl;
	return true;
}

/** Compute the total cost of all edges of the graph.
@param g the given graph
@param f_w function object that returns the cost of an edge
*/
template< class GraphT, class Fun >
typename Fun::result_type graph_cost( const GraphT &g, Fun f_w )
{
	typename Fun::result_type wt = ( typename Fun::result_type )0;
	typename GraphT::const_e_iterator ie;
	for( int i = 0; i < g.range(); ++i ) {
		for( ie = g[i].begin(); ie != g[i].end(); ++ie ) {
			wt = wt + f_w( &(( *ie ).edge_d() ) );
		}
	}
	return wt;
}

/**@}*/

} // end of namespace afg

#endif // end of _GRA_ALG_H_
