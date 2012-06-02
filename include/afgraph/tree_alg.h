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


/**@file tree_alg.h

  tree algorithms

  Aiguo Fei, UCLA 2000

*/

#ifndef _AFG_TREE_ALG_H_
#define _AFG_TREE_ALG_H_

#include <utility>
#include <vector>
#include <set>
#include <stack>

#include "shortest_path.h"
#include "tree_util.h"

namespace afg
{

/**@defgroup sptree shortest-path tree algorithms
@ingroup galg
*/

/**@{*/


/** build a single-source shortest-path tree.
The tree is "full-blown", having all the vertices and all vertex
  and edge information from the original graph.
@param gra the graph
@param ns index of source node
@param tree result tree
@param f_weight function to retrieve the weight(distance) of an edge,
  an edge pointer will be passed.
@param w_infty a weight that is large enough to be considered infinity.
@return true is successful, false if any exception happens when it
  calls dijkstra() to compute the shortest paths from the source.
*/
template< class GraphT, class TreeT, class Fun >
bool sptree_all( const GraphT &gra, int ns, TreeT &tree,
				 Fun f_weight, typename Fun::result_type w_infty )
{
	std::vector<int> vpred( gra.range() );
	typedef typename Fun::result_type REST;
	std::vector<REST> vdist( gra.range() );
	try {
		dijkstra<GraphT >( gra, ns, f_weight, w_infty,
						   vpred, vdist );
	} catch( ... ) {
		return false;
	}
	return pred2tree< GraphT, TreeT >( gra, vpred, tree );
}

/** build a single-source shortest-path tree for a subset of nodes.
Nodes are given by index. It is apparent that only nodes from the
 given set can be tree leaf nodes.
The tree is "full-blown", having all the vertices and all vertex
 and edge information from the original graph.
@see sptree_all
*/
template< class GraphT, class TreeT, class Fun >
bool sptree( const GraphT &gra, int ns, TreeT &tree,
			 const std::set
			 <int>& smem,
			 Fun f_weight, typename Fun::result_type w_infty )
{
	std::vector<int> vpred( gra.range() );
	typedef typename Fun::result_type REST;
	std::vector<REST> vdist( gra.range() );
	try {
		dijkstra<GraphT>( gra, ns, f_weight, w_infty,
						  vpred, vdist );
	} catch( ... ) {
		return false;
	}

	if( pred2tree< GraphT, TreeT >( gra, vpred, tree ) ) {
		prune_tree( tree, smem );
		return true;
	}
	return false;
}

/** build a "simple" single-source shortest-path tree.
The tree has integer vertecies (their indices in the original graph)
  and a single weight edge.
@see sptree_all pred2tree_s
*/
template< class GraphT, class TreeT, class Fun >
bool sptree_all_s( const GraphT &gra, int ns, TreeT &tree,
				   Fun f_weight, typename Fun::result_type w_infty )
{
	std::vector<int> vpred( gra.range() );
	typedef typename Fun::result_type REST;
	std::vector<REST> vdist( gra.range() );
	try {
		dijkstra<GraphT >( gra, ns, f_weight, w_infty,
						   vpred, vdist );
	} catch( ... ) {
		return false;
	}
	return pred2tree_s< TreeT >( vpred, vdist, tree );
}

/** build a "simple" single-source shortest-path tree
    for a subset of nodes.
The tree has integer vertecies (their indices in the original graph)
  and a single weight edge.
@see sptree pred2tree_s
*/
template< class GraphT, class TreeT, class Fun >
bool sptree_s( const GraphT &gra, int ns, TreeT &tree,
			   const std::set
			   <int>& smem,
			   Fun f_weight, typename Fun::result_type w_infty )
{
	std::vector<int> vpred( gra.range() );
	typedef typename Fun::result_type REST;
	std::vector<REST> vdist( gra.range() );
	try {
		dijkstra<GraphT >( gra, ns, f_weight, w_infty,
						   vpred, vdist );
	} catch( ... ) {
		return false;
	}

	if( pred2tree_s< TreeT >( vpred, vdist, tree ) ) {
		prune_tree( tree, smem );
		return true;
	}
	return false;
}

/**@}*/

/**@defgroup spantree spanning tree algorithms
@ingroup galg
*/

/**@{*/

/** extend a tree to cover more nodes by greedy strategy.
The tree is a "full-blown" one.
@param tree the tree to be extended.
@param gra the original graph.
@param snodes set of nodes that to be covered (index)
@param vpred all pair shortest path predcessors (n*n)
@param vdist all pari shortest path distances (n*n)
*/
template<class GT, class TT, class EDT>
bool extend_greedy( TT &tree, const GT &gra,
					const std::set
					<int>& snodes,
					const std::vector<int>& vpred,
					const std::vector<EDT>& vdist,
					EDT d_infty )
{
	int n = gra.range();
	tree.pack();
	tree.reserve( gra.range() );

	if( vpred.size() != n * n || vdist.size() != n * n ) {
		return false;
	}

	if( tree.size() < 1 ) {
		return false;    // empty tree, can't be extended
	}

	std::set
	<int> sleft = snodes;
	std::set
	<int>::const_iterator it;
	for( it = snodes.begin(); it != snodes.end(); ++it )
		if( tree.find_index( gra[*it].vertex_d() ) > 0 ) {
			sleft.erase( *it );    // this node is already covered
		}

	std::set
	<int>::iterator it2;
	int i;
	while( !sleft.empty() ) {
		// find a node with minimum distance from a tree node
		int nfrom = -1, nto = -1;
		EDT dmin = d_infty;
		for( i = 0; i < tree.size(); ++i ) {
			// index of tree node i in the original graph
			int oldi = gra.find_index( tree[i].vertex_d() );
			for( it2 = sleft.begin(); it2 != sleft.end(); ++it2 ) {
				if( vdist[oldi*n+( *it2 )] < dmin ) {
					nfrom = oldi;
					nto = *it2;
					dmin = vdist[oldi*n+( *it2 )];
				}
			}
		}
		// add that node and nodes on the connecting path to the tree
		if( nfrom >= 0 && nto >= 0 ) {
			sleft.erase( nto );
			tree.insert_v( gra[nto].vertex_d() );
			int n1 = -1;
			i = 0;
			while( nto != nfrom && ++i < n ) {
				n1 = vpred[nfrom*n+nto];
				//if( tree.find_index( gra[n1].vertex_d() )<0 ){
				if( n1 != nfrom ) {
					// this node is not in the tree yet
					// should be true until n1==nfrom
					tree.insert_v( gra[n1].vertex_d() );
				}
				tree.insert_e( gra[n1].vertex_d(), gra[nto].vertex_d(),
							   *gra.get_edge_byi( n1, nto ) );
				nto = n1;
			}
			if( i >= n ) {
				return false;
			}
			// i is used to control number of edges that can be added,
			// can't be more than n
		} else {
			return false;
		}
	}

	return true;
}

/** extend a tree to cover more nodes by greedy strategy.
The tree is a "simple" tree: vertex data type is int
  and edge has single weight (from vdist).
  Vertex data is index from the original graph given in snodes.
@param snodes set of nodes that to be covered (index)
@param vpred all pair shortest path predcessors (n*n)
@param vdist all pari shortest path distances (n*n)
*/
template<class TT, class EDT>
bool extend_greedy_s( TT &tree, const std::set
					  <int>& snodes, int n,
					  const std::vector<int>& vpred,
					  const std::vector<EDT>& vdist,
					  EDT d_infty )
{
	if(( int )vpred.size() != n * n || ( int )vdist.size() != n * n ) {
		return false;
	}

	if( tree.size() < 1 ) {
		return false;    // empty tree, can't be extended
	}

	std::set
	<int> sleft = snodes;
	std::set
	<int>::const_iterator it;
	for( it = snodes.begin(); it != snodes.end(); ++it )
		if( tree.find_index( *it ) > 0 ) {
			sleft.erase( *it );    // this node is already covered
		}

	std::set
	<int>::iterator it2;
	int i;
	while( !sleft.empty() ) {
		int nfrom = -1, nto = -1;
		typename TT::EDT dmin = d_infty;
		for( i = 0; i < tree.range(); ++i ) {
			if( tree.is_in_use( i ) ) {
				for( it2 = sleft.begin(); it2 != sleft.end(); ++it2 ) {
					if( vdist[tree[i].vertex_d()*n + ( *it2 )] < dmin ) {
						nfrom = tree[i].vertex_d();
						nto = *it2;
						dmin = vdist[tree[i].vertex_d() * n + ( *it2 )];
					}
				}
			}
		}
		if( nfrom >= 0 && nto >= 0 ) {
			sleft.erase( nto );
			int n1 = -1;
			i = 0;
			while( nto != nfrom && ++i < n ) {
				n1 = vpred[nfrom*n+nto];
				if( tree.find_index( n1 ) < 0 ) {
					tree.insert_v( n1 );
				}
				tree.insert_v( nto );
				tree.insert_e( n1, nto, vdist[nfrom*n+nto] - vdist[nfrom*n+n1] );
				nto = n1;
			}
			if( i >= n ) {
				return false;
			}
			// i is used to control number of edges that can be added,
			// can't be more than n
		} else {
			return false;
		}
	}

	return true;
}

/**@}*/

/**@defgroup ttrav tree transversal algorithms
@ingroup galg
*/

/**@{*/


/** Depth-first traversal of a tree, const version.
*/
template<class TreeT>
struct tree_dfs_c {

	typedef typename TreeT::EDT EDT;

protected:

	const TreeT &tree;

	// stack to keep track of edge (source, destination) nodes
	std::stack<std::pair<int, int> > m_sdfsNodes;
	// stack to keep track of edges (pointer to data)
	std::stack<const EDT * > m_sdfsEdges;

public:

	/// constructor
	tree_dfs_c( TreeT &t ): tree( t )
	{ }

	/** call this first before starting a depth-first of node traversal.
	  It actually traverses the subtree rooted at node ns.
	  @param ns index of the starting node.
	*/
	bool dfs_init( int ns ) {
		if( !tree.is_valid( ns ) ) {
			return false;
		}

		while( !m_sdfsNodes.empty() ) {
			m_sdfsNodes.pop();
			m_sdfsEdges.pop();
		}

		for( typename TreeT::const_e_iterator ite = tree.e_begin( ns );
			 ite != tree.e_end( ns ); ++ite ) {
			//std::cout<<"pushing: "<<ns<<"->"<<*ite<<std::endl;
			m_sdfsNodes.push( std::pair<int, int>( ns, ite->to() ) );
			m_sdfsEdges.push( &( ite->edge_d() ) );
		}

		return true;
	}

	/** get the next edge/node of depth-first traversal.
	  @param nsrc reference to hold source node of the edge being visited.
	  @param ndest reference to hold destination node of
	    the edge being visited.
	  @return a pointer to the data of the edge being visited.
	*/
	//typedef EDT* PEDT;
	const EDT *dfs_next( int &nsrc, int &ndest ) {
		if( m_sdfsEdges.empty() ) {
			return NULL;
		}

		// source node of edge being visited
		nsrc = m_sdfsNodes.top().first;
		ndest = m_sdfsNodes.top().second;
		// edge being visited
		const EDT *pe = m_sdfsEdges.top();

		//cout<<"  pop: "<<nsrc<<"->"<<ndest<<": "<<*pe<<endl;

		m_sdfsNodes.pop();
		m_sdfsEdges.pop();

		// destination node of edge being visited
		// push all edges from that node to the stack to visit next
		for( typename TreeT::const_e_iterator ite = tree.e_begin( ndest );
			 ite != tree.e_end( ndest ); ++ite ) {
			m_sdfsNodes.push( std::pair<int, int>( ndest, ite->to() ) );
			m_sdfsEdges.push( &( ite->edge_d() ) );
		}

		return pe;
	}// end of dfs_next

}
; // end of tree_dfs_c


/** Depth-first traversal of a tree, non-const version.
*/
template<class TreeT>
struct tree_dfs {

	typedef typename TreeT::EDT EDT;

protected:

	TreeT &tree;

	// stack to keep track of edge (source, destination) nodes
	std::stack<std::pair<int, int> > m_sdfsNodes;
	// stack to keep track of edges (pointer to data)
	std::stack<EDT * > m_sdfsEdges;

public:

	/// constructor
	tree_dfs( TreeT &t ): tree( t )
	{ }

	/** call this first before starting a depth-first of node traversal.
	  It actually traverses the subtree rooted at node ns.
	  @param ns index of the starting node.
	*/
	bool dfs_init( int ns ) {
		if( !tree.is_valid( ns ) ) {
			return false;
		}

		while( !m_sdfsNodes.empty() ) {
			m_sdfsNodes.pop();
			m_sdfsEdges.pop();
		}

		for( typename TreeT::e_iterator ite = tree.e_begin( ns );
			 ite != tree.e_end( ns ); ++ite ) {
			//std::cout<<"pushing: "<<ns<<"->"<<*ite<<std::endl;
			m_sdfsNodes.push( std::pair<int, int>( ns, ite->to() ) );
			m_sdfsEdges.push( &( ite->edge_d() ) );
		}

		return true;
	}

	/** get the next edge/node of depth-first traversal.
	  @param nsrc reference to hold source node of the edge being visited.
	  @param ndest reference to hold destination node of
	    the edge being visited.
	  @return a pointer to the data of the edge being visited.
	*/
	//typedef EDT* PEDT;
	EDT *dfs_next( int &nsrc, int &ndest ) {
		if( m_sdfsEdges.empty() ) {
			return NULL;
		}

		// source node of edge being visited
		nsrc = m_sdfsNodes.top().first;
		ndest = m_sdfsNodes.top().second;
		// edge being visited
		EDT *pe = m_sdfsEdges.top();

		//cout<<"  pop: "<<nsrc<<"->"<<ndest<<": "<<*pe<<endl;

		m_sdfsNodes.pop();
		m_sdfsEdges.pop();

		// destination node of edge being visited
		// push all edges from that node to the stack to visit next
		for( typename TreeT::e_iterator ite = tree.e_begin( ndest );
			 ite != tree.e_end( ndest ); ++ite ) {
			m_sdfsNodes.push( std::pair<int, int>( ndest, ite->to() ) );
			m_sdfsEdges.push( &( ite->edge_d() ) );
		}

		return pe;
	}// end of dfs_next

}
; // end of tree_dfs

/**@}*/

} // end of namespace afg

#endif
