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


/**@file shortest_path.h

  shortest-path algorithms:
    dijkstra's shortest path algorithm

  Aiguo Fei

  1999, 2000, 2001
*/

#ifndef _AFG_SHORTEST_PATH_H_
#define _AFG_SHORTEST_PATH_H_

#include <vector>
//#include <iostream>

#include "afl/util_tl.hpp"
#include "afl/sorted_list.hpp"
#include "path.h"
#include "gexception.h"

namespace afg
{


/**@defgroup gspalg shortest-path algorithms
@ingroup galg
*/

/**@{*/

/** Dijkstra algorithm to compute single source shortest paths.
    GraphT: type of the graph.
    @param graph graph on which to run the algorithm, no "negative" weighted edge.
    Graph is not required to be "packed".
    @param nsource source node, should be between 0 and graph.szie()-1
    @param f_weight function object to get weight of an edge
      (passed by pointer), e.g.: pointer2value<int>( ), runit_p<..>( ).
    @param w_infty an "upper bound" value of weight which should be larger
      than the path length of any valid path; used to initialize distance
      to all nodes.
    @param pred a vector to store predecessor information,
      must have size()>=graph.range().
    @param dist a vector to store path length information (from nsource
      to a node), must have size()>=graph.range();
      if there is no path from nsource to node i, then dist[i]=w_infty.
    @exception unknown_error
*/
template< class GraphT, class Fun >
bool dijkstra( const GraphT &graph, int nsource, Fun f_weight,
			   typename Fun::result_type w_infty,
			   std::vector<int>& pred,
			   std::vector<typename Fun::result_type>& dist )
{
	if( !graph.is_valid( nsource ) ) {
		return false;    // not a valid source node
	}

	if(( int )pred.size() < graph.range( ) ) {
		pred.resize( graph.range() );
	}
	if(( int )dist.size() < graph.range() ) {
		dist.resize( graph.range() );
	}

	//cout<<"infinity: "<<w_infty<<endl;
	int i;
	for( i = 0; i < graph.range(); ++i ) {
		pred[i] = -1;
		dist[i] = w_infty;
	}

	typedef typename Fun::result_type WeightT;
	typedef afl::named_pair<int, WeightT> NP;

	afl::sorted_list< NP > lPq;
	dist[nsource] = ( WeightT )0;
	lPq.push( NP( nsource, ( WeightT )0 ) );

	int nvertex;
	WeightT w;
	typename GraphT::const_e_iterator ite;
	try {
		while( !lPq.empty() ) {
			nvertex = ( lPq.front() ).name;
			lPq.pop_front();

			for( ite = graph.e_begin( nvertex ); ite != graph.e_end( nvertex ); ++ite ) {
				w = dist[nvertex] + f_weight( &(( *ite ).edge_d() ) );
				if( w < dist[( *ite ).to()] ) {
					lPq.push( NP(( *ite ).to(), w ) );
					pred[( *ite ).to()] = nvertex;
					dist[( *ite ).to()] = w;
				}
			}
		}
	} catch( ... ) {
		// just catch any error and thow unknown_error exception
		std::string s = std::string( "unknown error in dijkstra( ), possible cause: " )
						+ std::string( "some graph edge points to out of range index" );
		throw afl::unknown_except<std::string>( s );
	}

	return true;
}

/** run Dijkstra's algorithm for a single destination.
    A wrapper for the above more general dijkstra().
    @param graph on which to find the path, no "negative" weighted edge.
    Graph is not required to be "packed".
    @param nsource source nodes
    @param ndst destination node
    @param f_weight edge weight function, pass pointer
    @param w_infty an upper bound value of weight which should be larger
      than the path length of any valid path
    @param lp result path
    @return path length, w_infty if no path found.
    @exception unknown_error, from dijkstra( ) function call.
*/
template< class GraphT, class Fun >
typename Fun::result_type dijkstra_t( const GraphT &graph, int nsource,
									  int ndst, Fun f_weight,
									  typename Fun::result_type w_infty,
									  CPath &lp )
{
	std::vector<int> vpred( graph.range() );
	typedef typename Fun::result_type RT;
	std::vector<RT> vdist( graph.range() );

	if( !dijkstra<GraphT, Fun>( graph, nsource, f_weight, w_infty,
								vpred, vdist ) ) {
		return w_infty;
	}

	if( pred2path( vpred, nsource, ndst, lp ) ) {
		return vdist[ndst];
	} else {
		return w_infty;
	}
}

/** Floyd-Warshall algorithm to compute all pair shortest paths.
@param graph graph on which to run the algorithm, no "negative" weighted edge.
@param pred[i*n+j]: predcessor of j on the shortest path from i to j
@param dist[i*n+j]: distance of shortest path from i to j
*/
template< class GraphT, class Fun >
bool floyd_warshall_allsp( const GraphT &graph, Fun f_weight,
						   typename Fun::result_type w_infty,
						   std::vector<int>& pred,
						   std::vector<typename Fun::result_type>& dist )
{
	int n = graph.range( );

	if(( int )pred.size() < n * n ) {
		pred.resize( n * n );
	}
	if(( int )dist.size() < n * n ) {
		dist.resize( n * n );
	}

	//cout<<"infinity: "<<w_infty<<endl;
	int i;
	for( i = 0; i < n * n; ++i ) {
		pred[i] = -1;
		dist[i] = w_infty;
	}

	typedef typename Fun::result_type WeightT;

	typename GraphT::const_e_iterator ite;
	for( i = 0; i < n; ++i ) {
		dist[i *n+i] = ( WeightT )0;
		for( ite = graph.e_begin( i ); ite != graph.e_end( i ); ++ite ) {
			if( ite->to() >= n ) {
				return false;
			}
			pred[i*n+ite->to()] = i;
			dist[i*n+ite->to()] = f_weight( &(( *ite ).edge_d() ) );
		}
	}
	int j, k;
	for( k = 0; k < n; ++k ) {
		//cerr<<"k :"<<k<<endl;}
		for( i = 0; i < n; ++i ) {
			for( j = 0; j < n; ++j )
				if( dist[i*n+j] > dist[i*n+k] + dist[k*n+j] ) {
					dist[i *n+j] = dist[i*n+k] + dist[k*n+j];
					pred[i *n+j] = pred[k*n+j];
				}
		}
	}

	return true;
}

/**@}*/

} // end of namespace afg


#endif
