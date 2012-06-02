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


/**@file rtree.h

*/

#ifndef _AFG_TREE_H_
#define _AFG_TREE_H_

#include <set>

#include "graph.h"

namespace afg
{

/**@defgroup rtree A Rooted Tree Structure
@ingroup graphd
*/
/**@{*/

/** rooted tree.
    Tree structure that has a specific root.
*/
template < class VDT, class EDT, class f_eqv = std::equal_to<VDT> >
class CrTree: public CGraph<VDT, EDT, f_eqv>
{
protected:
	int m_nRoot;
	std::vector<int> m_Parents;

public:
	/// constructor.
	CrTree( int size = 1, VDT v = VDT(), double dg = 0.25,
			const f_eqv &eqv = f_eqv() )
		: CGraph<VDT, EDT, f_eqv>( size, v, dg, eqv ),
		  m_nRoot( -1 ), m_Parents( size, -1 )
	{ }

	/// tree type, this class itself
	typedef CrTree<VDT, EDT, f_eqv> TT;
	typedef CGraph<VDT, EDT, f_eqv> base_graph_t;

	CrTree( const TT &rhs ): CGraph<VDT, EDT, f_eqv>( rhs ),
		m_Parents( rhs.m_Parents )
	{ }

	const TT &operator=( const TT &rhs ) {
		if( this != &rhs ) {
			base_graph_t::m_eqv = rhs.m_eqv;
			base_graph_t::m_Vertices = rhs.m_Vertices;
			base_graph_t::m_vDefault = rhs.m_vDefault;
			base_graph_t::m_nSize = rhs.m_nSize;
			base_graph_t::m_dGrow = rhs.m_dGrow;
			base_graph_t::m_iUnused = rhs.m_iUnused;
			m_Parents = rhs.m_Parents;
		}
		return *this;
	}

	/// copy all vertices from a graph
	virtual void copy_vertices( const base_graph_t &gra ) {
		base_graph_t::copy_vertices( gra );
		m_Parents.resize( gra.range() );
		for( int i = 0; i < gra.range(); ++i ) {
			m_Parents[i] = -1;
		}
	}

	/// return parent node of node v
	int parent( const VDT &v ) const {
		int i = find_index( v );
		if( i >= 0 ) {
			return m_Parents[i];
		}
		return -1;
	}

	/** return parent node of node of index i
	@exception std::out_of_range, with message "parent node access in tree".
	*/
	int parent_byi( int i ) const throw( std::out_of_range ) {
		if( !base_graph_t::is_valid( i ) ) {
			throw std::out_of_range( std::string( "parent node access in tree" ) );
		}

		return m_Parents[i];
	}

	/// return the index of the root node
	int root( void ) const {
		return m_nRoot;
	}

	/// return the index of the root node, by reference
	int &root( void ) {
		return m_nRoot;
	}

	/// set the root node
	bool set_root( const VDT &v ) {
		int n = find_index( v );
		if( n < 0 ) {
			return false;
		}
		m_nRoot = n;
		m_Parents[m_nRoot] = -1;
		return true;
	}

	/// set the root node by index
	bool set_root_byi( int n ) {
		if( !base_graph_t::is_vallid( n ) ) {
			return false;
		}
		m_nRoot = n;
		m_Parents[m_nRoot] = -1;
		return true;
	}

	/// find the first node which doesn't have a parent
	int first_root( void ) const {
		int i = 0;
		while( i < base_graph_t::range() && m_Parents[i] >= 0 ) {
			++i;
		}
		if( i < base_graph_t::range() ) {
			return i;
		}
		return -1;
	}

	bool is_member( int i ) const {
		if( m_Parents[i] >= 0 || ( *this )[i].out_degree() > 0 ) {
			return true;
		}
		return false;
	}

	bool is_leaf( int i ) const {
		if( m_Parents[i] >= 0 && ( *this )[i].out_degree() == 0 ) {
			return true;
		} else {
			return false;
		}
	}

	virtual std::ostream &output( std::ostream &os ) const {
		for( int i = 0; i < ( int )base_graph_t::m_Vertices.size(); ++i )
			if( base_graph_t::is_in_use( i ) ) {
				os << '(' << m_Parents[i] << ")(" << i << ')'
				   << base_graph_t::at( i ) << std::endl;
			}
		return os;
	}

	/** reserve a certain size for the tree.
	  Has no effect if n is less than or equal to range( );
	  otherwise capacity will be increased and all iteroators may be
	  invalidated, but an index should still pointing to the same vertex.
	*/
	virtual void reserve( int n ) {
		int olds = base_graph_t::range();
		base_graph_t::reserve( n );
		int nn = base_graph_t::range();
		if( nn > olds ) {
			m_Parents.resize( n, -1 );
		}
	}

	/** rearrange vertices to make them stored consecutively.
	  After pack( ), all iterators and indices may be invalidated.
	  All vertices will be stored within [0, size()-1 ].
	  @param ns new range; if ns<size( ), size() will be used.
	  @return number of index changes that have been made.
	*/
	virtual int pack( int ns = 0 ) {
		VDT vroot;
		bool broot = false;
		if( base_graph_t::is_in_range( m_nRoot ) && base_graph_t::is_in_use( m_nRoot ) ) {
			broot = true;
			vroot = base_graph_t::at( m_nRoot ).vertex_d();
		}
		int nch = base_graph_t::pack(); // number of index changes
		//cout<<"before updating parent:\n"<<*this<<endl;
		m_Parents.resize( base_graph_t::range(), -1 );
		if( nch > 0 ) {
			// only need to update parents if some index has been changed
			typename base_graph_t::const_e_iterator it;
			for( int i = 0; i < base_graph_t::size(); ++i )
				for( it = base_graph_t::e_begin( i ); it != base_graph_t::e_end( i ); ++it ) {
					//cout<<it->to()<<", "<<m_Parents[it->to()]<<", "<<i<<endl;
					m_Parents[it->to()] = i;
				}
			if( broot ) {
				m_nRoot = find_index( vroot );
				m_Parents[m_nRoot] = -1;
				//cout<<"root: "<<m_nRoot<<", "<<m_Parents[m_nRoot]<<endl;
			}
		}
		return nch;
	}

	/**@name insertion/deletion */
	//@{

	/** remove a vertex of index vi.
	    Has no effect if vertex not found; otherwise this vertex and
	    all related edges (from/to it) will be removed as well.
	*/
	virtual void remove_v_byi( int vi ) {
		if( base_graph_t::is_valid( vi ) ) {
			// valid index
			typename base_graph_t::const_e_iterator it;
			for( it = base_graph_t::e_begin( vi ); it != base_graph_t::e_end( vi ); ++it ) {
				m_Parents[it->to()] = -1;
			}
			base_graph_t::remove_v_byi( vi );
			m_Parents[vi] = -1;
		}
	}

	/** clean-up the "tree" by removing "isolated" nodes.
	  An "isolated" is a node that is not any node's parent or child;
	    in other words, has no neighbors.
	  @return number of nodes that are removed.
	*/
	virtual int clean( void ) {
		int ncount = 0;
		for( int i = 0; i < base_graph_t::range(); ++i )
			if( i != m_nRoot && m_Parents[i] == -1 ) {
				// a node in this rooted tree is an isolated node
				// if it has no parent and it is not the root;
				// in other words, every node except the root need to have a parent
				base_graph_t::remove_v_byi( i );
				++ncount;
			}
		return ncount;
	}

	/** insert an edge for vertex u to v.
	  Insertion fails if any of the vertex is not found.
	  If edge already exists then edge data will be replaced by e.
	  @return true if succeeds, false otherwise.
	*/
	virtual bool insert_e( const VDT &u, const VDT &v,
						   const EDT &e ) {
		// find the indices of both nodes, then insert the corresponding edge,
		// if both nodes are found
		int i = find_index( u );
		if( i >= 0 ) {
			int j = find_index( v );
			if( j >= 0 ) {
				base_graph_t::m_Vertices[i].insert_edge( j, e );
				m_Parents[j] = i;
				//cout<<"tree insert_e: "<<i<<"->"<<j<<endl;
				return true;
			}
		}
		return false;
	}


	/** insert an edge (i->j).
	  Insertion fails if any of the two indices is unused
	  (not vertext at that position).
	  @return true if succeeds, otherwise false.
	*/
	virtual bool insert_e_byi( int i, int j, const EDT &e ) {
		if( base_graph_t::insert_e_byi( i, j, e ) ) {
			m_Parents[j] = i;
			//cout<<"tree insert_e_byi: "<<i<<j<<endl;
			return true;
		} else {
			return false;
		}
	}

	/** remove an edge by index.
	    Has no effect if edge not found. */
	virtual void remove_e_byi( int nfrom, int nto ) {
		if( base_graph_t::is_in_range( nfrom ) & base_graph_t::is_in_range( nto ) ) {
			base_graph_t::remove_e_byi( nfrom, nto );
			if( m_Parents[nto] == nfrom ) { // always true for a "valid" tree
				m_Parents[nto] = -1;
			}
		}
	}

	/// remove all edges of vertex of index nfrom
	virtual void remove_alle_byi( int nfrom ) {
		if( base_graph_t::is_in_range( nfrom ) ) {
			typename base_graph_t::const_e_iterator it;
			for( it = base_graph_t::e_begin( nfrom ); it != base_graph_t::e_end( nfrom ); ++it ) {
				m_Parents[it->to()] = -1;
			}
			base_graph_t::remove_alle_byi( nfrom );
		}
	}

	/// remove all edges of all vertices
	virtual void remove_all_edges( void ) {
		base_graph_t::remove_all_edges( );
		for( int i = 0; i < ( int )m_Parents.size(); ++i ) {
			m_Parents[i] = -1;
		}
	}

	/// clear all nodes and all their edges
	virtual void clear( void ) {
		base_graph_t::clear();
		for( int i = 0; i < ( int )m_Parents.size(); ++i ) {
			m_Parents[i] = -1;
		}
	}
	//@} // end of insertion/deletion

private: // the following member functions are disabled in tree

	virtual bool insert_2e( const VDT &u, const VDT &v,
							const EDT &e ) {
		return false;
	}
	virtual bool insert_2e_byi( int i, int j, const EDT &e ) {
		return false;
	}
	virtual void remove_2e( const VDT &u, const VDT &v )
	{ }
	virtual void remove_2e_byi( int n1, int n2 )
	{ }

protected:
	// grow the graph size by a certain percentage
	virtual int grow( int ns = 0 ) {
		int n = base_graph_t::grow( ns );
		int nold = m_Parents.size();
		if( n > nold ) {
			m_Parents.resize( n );
			for( int i = nold - 1; i < n; ++i ) {
				m_Parents[i] = -1;
			}
		}
		return n;
	}

public:
	/** get all child nodes of a specific node given by index.
	@param inode index the specific node
	@param sch set to hold indices of the children
	@return number of children found.
	*/
	int get_all_children( int inode, std::set
						  <int>& sch ) {
		int ncount = 0;
		for( int i = 0; i < base_graph_t::range(); ++i )
			if( base_graph_t::is_in_use( i ) && parent_byi( i ) == inode ) {
				sch.insert( i );
				++ncount;
			}
		return ncount;
	}

}; // end of class CTree

/// type of rooted tree provided by rtree.h
#define rtree_type afg::CrTree

#define _RTREEDEFINED

/**@} */
} // end of namespace afg

#endif
