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


/**@file tree_util.h

  utility routines for tree

  Aiguo Fei, UCLA

    1999, 2000
*/

#ifndef _AFG_TREE_UTIL_H_
#define _AFG_TREE_UTIL_H_

#include <set>

namespace afg
{

using std::set
;
using std::vector;

/**@defgroup trutils tree related utilities
@ingroup galg
*/

/**@{*/

/** prune a tree such that a vertex can be a leaf node
  only if it is in in_set .
*/
template<class TreeT>
void prune_tree( TreeT &tree, const set
				 <int>& in_set )
{
	int nprune = 1, nparent;
	while( nprune ) {
		nprune = 0;
		for( int i = 0; i < tree.range(); ++i ) {
			if( tree.is_in_use( i ) &&
				tree.is_leaf( i ) && ( in_set.find( i ) == in_set.end() ) ) {
				if(( nparent = tree.parent_byi( i ) ) >= 0 ) {
					tree.remove_e_byi( nparent, i );
					tree.remove_v_byi( i );
					++nprune;
				}
			}
		}
	}
}

/** get a "full-blown" tree from a predecessor array and the
  corresponding graph.
The tree has all the vertices and all the vertex and edge information
  from the original graph.
*/
template<class GT, class TT>
bool pred2tree( const GT &gra, int n, const int *pred,
				TT &tree )
{
	tree.copy_vertices( gra );
	const typename GT::EDT *pedg;
	for( int i = 0; i < n; ++i ) {
		if( pred[i] >= 0 ) {
			if( pedg = gra.get_edge_byi( pred[i], i ) ) {
				tree.insert_e_byi( pred[i], i, *pedg );
			} else {
				return false;    // this should never happen
			}
		}
	}
	return true;
}

/** get a "full-blown" tree from a predecessor vector and the
  corresponding graph.
The tree has all the vertices and all the vertex and edge information
  from the original graph.
*/
template<class GT, class TT>
bool pred2tree( const GT &gra, const vector<int>& vpred,
				TT &tree )
{
	tree.copy_vertices( gra );
	const typename GT::EDT *pedg;
	for( int i = 0; i < ( int )vpred.size(); ++i ) {
		if( vpred[i] >= 0 ) {
			if(( pedg = gra.get_edge_byi( vpred[i], i ) ) ) {
				tree.insert_e_byi( vpred[i], i, *pedg );
			} else {
				return false;    // this should never happen
			}
		}
	}
	return true;
}

/** get a "simple" tree from a predecessor vector and the
  corresponding distance vector.
The tree has integer vertecies (their indecies in the original graph)
  and edges of a single weight (distance from distance vector),
  or edges of type that can initialize from the distance type.
Note: if the original graph from which vpred and vdist are generated
  using Dijkstra and the graph is not packed, then resulting tree may
  have "extra" node(s).
*/
template<class TT, class EDT>
bool pred2tree_s( const vector<int>& vpred,
				  const vector<EDT>& vdist, TT &tree )
{
	if( vpred.size() != vdist.size() ) {
		return false;
	}

	// new edge data type
	typedef typename TT::EDT NEDT;

	int i;
	tree.reserve( vpred.size() );
	// insert all vertices
	for( i = 0; i < ( int )vpred.size(); ++i ) {
		tree.insert_v_ati( i, i );
	}

	for( i = 0; i < ( int )vpred.size(); ++i ) {
		if( vpred[i] >= 0 ) {
			if( vpred[i] < ( int )vpred.size() )
				tree.insert_e_byi( vpred[i], i,
								   ( NEDT )( vdist[i] - vdist[vpred[i]] ) );
			else {
				return false;    // should never happen
			}
		}
	}
	return true;
}


/** get a "full-blown" tree from an all-pair-shortest-pathpredecessor
  vector and the corresponding graph.
The tree has all the vertices and all the vertex and edge information
  from the original graph.
@param vpred[i*n+j]: predcessor of j on the shortest path from i to j
@param ns source node
*/
template<class GT, class TT>
bool all_pred2tree( const GT &gra, const vector<int>& vpred,
					int ns, TT &tree )
{
	int n = gra.range();
	tree.copy_vertices( gra );

	const typename GT::EDT *pedg;
	for( int i = 0; i < n; ++i ) {
		if( vpred[ns *n+i] >= 0 ) {
			if(( pedg = gra.get_edge_byi( vpred[ns*n+i], i ) ) ) {
				tree.insert_e_byi( vpred[ns*n+i], i, *pedg );
			} else {
				return false;    // this should never happen
			}
		}
	}
	return true;
}

/** get a "simple" tree from an all-shortest-path predecessor vector
  and the corresponding distance vector.
The tree has an integer vertex (its index in the original graph)
  and a single weight edge (distance from distance vector).
Note: if the original graph from which vpred and vdist are generated
is not packed, then resulting tree may have "extra" node(s).
@param vpred[i*n+j]: predcessor of j on the shortest path from i to j
@param vdist[i*n+j]: distance of shortest path from i to j
*/
template<class TT, class EDT>
bool all_pred2tree_s( const vector<int>& vpred,
					  const vector<EDT>& vdist,
					  int n, int ns, TT &tree )
{
	if( vpred.size() != n * n || vdist.size() != n * n ) {
		return false;
	}

	// new edge data type
	typedef typename TT::EDT NEDT;

	int i;
	tree.reserve( n );
	// insert all vertices
	for( i = 0; i < n; ++i ) {
		tree.insert_v_ati( i, i );
	}

	for( i = 0; i < n; ++i ) {
		if( vpred[ns *n+i] >= 0 ) {
			if( vpred[ns*n+i] < n ) {
				//cout<<"insert: "<<vpred[ns*n+i]<<"->"<<i<<": "
				//  <<vdist[ns*n+i]-vdist[ns*n+vpred[ns*n+i]]
				//  <<" ("<<vdist[ns*n+i]<<", "<<vdist[ns*n+vpred[ns*n+i]]<<")\n";
				tree.insert_e_byi( vpred[ns*n+i], i,
								   ( NEDT )( vdist[ns*n+i] - vdist[ns*n+vpred[ns*n+i]] ) );
			} else {
				return false;    // should never happen
			}
		}
	}
	return true;
}

/**@}*/
}
#endif
