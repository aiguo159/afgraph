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


/**@file graph_gen.h

  Graph generators.

  Aiguo Fei
    UCLA, 1999, 2000, 2001
*/

#ifndef _AFG_GRAPH_GEN_H_
#define _AFG_GRAPH_GEN_H_

#include <utility>
#include <vector>

#include <math.h>

#include "afl/rand.hpp"

#include "gtypes.h"

/* Before including this file, make sure the header file, in which graph_type
    (required by gtypes.h) is defined, is included first.
    For example, graph.h.
*/

namespace afg
{

/**@defgroup gutil Graph Utilities
graph generators, convertors, input/export utilities
*/

/**@defgroup graphgen graph generators
@ingroup gutil
*/

/**@{*/


/** generate bi-directional mesh of n column, m rows.
    Graph generated has vertex data type of xyVertex<int> (integer x/y coordinates)
      and edge data type of int (every edge has weight 1).
    @param grf result graph. grf should be empty before passed to this function;
      grf.clear( ) will make it emtpy. A newly created graph is empty as well.
*/
void grid_graph_gen( T_xynGraph &grf, int n, int m )
{
	if( n <= 0 || m <= 0 ) {
		return;
	}

	grf.reserve( n * m );

	int i, j;
	// insert all vertices
	for( i = 0; i < n; ++i )
		for( j = 0; j < m; ++j ) {
			grf.insert_v( xyVertex<int>( i, j ) );
		}

	i = 0;
	for( j = 0; j < m - 1; ++j ) {
		grf.insert_2e( xyVertex<int>( i, j ), xyVertex<int>( i, j + 1 ), 1 );
	}
	j = 0;
	for( i = 1; i < n; ++i ) {
		grf.insert_2e( xyVertex<int>( i - 1, j ), xyVertex<int>( i, j ), 1 );
	}

	for( i = 1; i < n; ++i )
		for( j = 1; j < m; ++j ) {
			grf.insert_2e( xyVertex<int>( i - 1, j ), xyVertex<int>( i, j ), 1 );
			grf.insert_2e( xyVertex<int>( i, j ), xyVertex<int>( i, j - 1 ), 1 );
		}
}


/** a graph generator based on B.M. Waxman's method.
    Generate a connected directed graph where if there is an edge (i->j)
    then there is an edge (j->i).
    Graph generated has vertex data type of xyVertex<int>
      (integer x/y coordinates) and edge data type of double,
      which is the geometric distance between two nodes.
    @param dgrf result graph, each vertex has integer x,y coordinates,
      each edge has a double weight which is geometric distance between nodes.
      dgrf should be empty before passed to this function;
      dgrf.clear( ) will make it emtpy. A newly created graph is empty as well.
    @param ngrid grid size, graph nodes are picked from an ngrid*ngrid area
    @param n number of nodes
    @param ntry number of trials; if no connected graph can be generated
      after n trials, then gives up.
    @return number of links in graph generated;
      since every link is bidirectional, # of edges=2*(# of links).
*/
int bmw_graph_gen( T_xydGraph &dgrf,
				   int ngrid, int n, int ntry = 10,
				   double lamda = 0.3, double rou = 0.3 )
{
	std::vector<std::pair<int, int> > vpos( n );
	double dtmp, ddist;
	double dmaxdist = 1.414 * ngrid;
	double dInfty = n * dmaxdist;
	int nlinks = 0;
	bool bconnected;

	while( ntry > 0 ) {
		std::vector<bool> vmarks( ngrid * ngrid, false ) ;
		int ncount = 0, i, j;
		nlinks = 0;
		// randomly choose a number of grid points as nodes
		while( ncount < n ) {
			i = ( int )afl::rand::gen( ngrid );
			j = ( int )afl::rand::gen( ngrid );
			if( !vmarks[i*ngrid+j] ) {
				vmarks[i *ngrid+j] = true;
				vpos[ncount++] = std::pair<int, int>( i, j );
			}
		}

		// choose a link between tow nodes according to a certain probability
		std::vector<double> vadjdist( n * n, dInfty );
		for( i = 0; i < n; ++i ) {
			vadjdist[i *n+i] = 0.0;
			for( j = i + 1; j < n; ++j ) {
				int d1 = vpos[i].first - vpos[j].first;
				int d2 = vpos[i].second - vpos[j].second;
				ddist = sqrt(( double )( d1 * d1 + d2 * d2 ) );
				dtmp = lamda * exp( -ddist / ( rou * dmaxdist ) );
				if( afl::rand::gen( 1.0 ) < dtmp ) {
					vadjdist[i *n+j] = ddist;
					vadjdist[j *n+i] = ddist;
					++nlinks;
					//cout<<i<<", "<<j<<": "<<vadjdist[i*n+j]<<endl;
				}
			}// for j
		}// for i

		// find distances between all pairs
		std::vector<double> vdist( vadjdist );
		// vdist[j*n+i] is distance between (i,j)
		for( i = 0; i < n; ++i ) {
			for( j = 0; j < n; ++j )
				for( int k = 0; k < n; ++k )
					if( vdist[i*n+j] + vdist[k*n+i] < vdist[k*n+j] ) {
						vdist[k *n+j] = vdist[i*n+j] + vdist[k*n+i];
					}
		}

		// see if graph is connected
		bconnected = true;
		i = 0;
		while( i < n && bconnected ) {
			j = i + 1;
			while( j < n ) {
				//cout<<i<<", "<<j<<": "<<vdist[j*n+i]<<endl;
				if( vdist[j *n+i] >= dInfty ) {
					bconnected = false;
					break;
				}
				++j;
			}
			++i;
		}

		// get the graph from adj matrix, edge weight is the geometric distance
		if( bconnected ) {
			dgrf.reserve( n );
			for( i = 0; i < n; ++i ) {
				dgrf.insert_v( xyVertex<int>( vpos[i].first, vpos[i].second ) );
			}
			for( i = 0; i < n; ++i ) {
				for( j = i + 1; j < n; ++j ) {
					if( vadjdist[i*n+j] < dInfty ) {
						dgrf.insert_2e( xyVertex<int>( vpos[i].first, vpos[i].second ),
										xyVertex<int>( vpos[j].first, vpos[j].second ),
										vadjdist[j*n+i] );
					}
				}
			}
			return nlinks;
		}

		ntry--;
	}

	return -1;
};

/**@}*/

} // end of namespace afg

#endif



