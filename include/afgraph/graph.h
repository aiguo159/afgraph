/*@copyright

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


/**@file graph.h

  Aiguo Fei

    1999-2001

    2001/02/19: bug fix, CGraph::pack()

*/

#ifndef _AFG_GRAPH_H_
#define _AFG_GRAPH_H_

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <exception>

#include "afl/util_tl.hpp"
#include "afl/sorted_list.hpp"
#include "graph_intf.h"

namespace afg
{

/**@defgroup mygraph An Implementation of Graph classes
@ingroup graphd
*/

/**@{*/

/** edge
*/
template<class EdgeDT>
class CiEdge: public IEdge<EdgeDT>
{
protected:
	/// data member, destination node (index)
	int m_nTo;

	/// data member, edge data
	EdgeDT m_EdgeD;

public:

	/// type of the edge data
	typedef EdgeDT EDT;

	/// constructor
	explicit CiEdge( int nto = -1, const EdgeDT &edg = EdgeDT() )
		: m_nTo( nto ), m_EdgeD( edg )
	{ }

	/// return destination node (index)
	int to( void ) const {
		return m_nTo;
	};

	/// return reference of destination node (index)
	int &to( void ) {
		return m_nTo;
	}

	/// return reference of edge data
	EdgeDT &edge_d( void ) {
		return m_EdgeD;
	}

	/// return reference of edge data, const version
	const EdgeDT &edge_d( void ) const {
		return m_EdgeD;
	}

};

/**@name stream input/output operators */
//@{
/** operator to input CiEdge.
  Read an integer as index then a character (':') then edge_data
*/
template<class EdgeDT>
inline std::istream &operator>>( std::istream &is, CiEdge<EdgeDT>& edge )
{
	char c;
	is >> edge.to() >> c >> edge.m_EdgeD;
	return is;
}
//@}

/* should make some members protected and make CGraph as friend;
  however, template friend is annoying with C++ (have to put all the template
  names in declarion), so just use public.
*/
/** internal vertex in our graph representation.
    @author Aiguo Fei
    @version 0.8a, revision November 2000
*/
template<class VertexDT, class EdgeDT>
class CiVertex: public IVertex < VertexDT, EdgeDT,
	typename std::list<CiEdge<EdgeDT> >::iterator,
	typename std::list<CiEdge<EdgeDT> >::const_iterator >
{
public:

	/// edge type
	typedef CiEdge<EdgeDT> iET;

	/// vertex type, this class itself
	typedef CiVertex<VertexDT, EdgeDT> iVT;

	/// edge data type
	typedef EdgeDT EDT;
	/// vertex data type
	typedef VertexDT VDT;

	/** iterator type to access edges.
	*/
	typedef typename std::list<CiEdge<EdgeDT> >::iterator iterator;

	/** const iterator type to access edges (read-only).
	*/
	typedef typename std::list<CiEdge<EdgeDT> >::const_iterator const_iterator;

protected:
	/// data member, vertex data
	VertexDT m_VertexD;
	/// data member, a list of edges.
	std::list< CiEdge<EdgeDT> > m_lEdges;

private:
	// notation convenience, vertex base type
	typedef IVertex < VertexDT, CiEdge<EdgeDT>,
			typename std::list<CiEdge<EdgeDT> >::iterator,
			typename std::list<CiEdge<EdgeDT> >::const_iterator > VBT;

public:
	/// constructor.
	explicit CiVertex( const VertexDT &v = VertexDT() )
		: m_VertexD( v ), m_lEdges()
	{ }

	CiVertex( const CiVertex<VertexDT, EdgeDT>& rhs )
		: m_VertexD( rhs.m_VertexD ), m_lEdges( rhs.m_lEdges )
	{ }

	/// assignment operator
	const CiVertex<VertexDT, EdgeDT>& operator=(
		const CiVertex<VertexDT, EdgeDT>& rhs ) {
		if( this != &rhs ) {
			m_VertexD = rhs.m_VertexD;
			m_lEdges = rhs.m_lEdges;
		}
		return *this;
	}

	/// return a reference of the vertex data
	virtual VertexDT &vertex_d( void ) {
		return m_VertexD;
	}
	/// return a reference of the vertex data, const version
	virtual const VertexDT &vertex_d( void ) const {
		return m_VertexD;
	}

	// implementation of interface
	/// return iterator pointing to the first edge
	virtual iterator begin( void ) {
		return m_lEdges.begin();
	}
	/// return iterator pointing to the end (not pointting to any endge).
	virtual iterator end( void ) {
		return m_lEdges.end();
	}
	/// return iterator pointing to the first edge
	virtual const_iterator begin( void ) const {
		return m_lEdges.begin();
	}
	/// return iterator pointing to the end (not pointting to any endge).
	virtual const_iterator end( void ) const {
		return m_lEdges.end();
	}

	/** get an edge of this vertex
	    @param nto destination node (index)
	    @return a pointer to the edge data, NULL if edge doesn't exist
	*/
	virtual EdgeDT *get_edge( int nto ) {
		if( m_lEdges.empty() ) {
			return NULL;
		}
		iterator il = m_lEdges.begin();
		while( il != m_lEdges.end() ) {
			if( nto == ( *il ).to( ) ) {
				return &(( *il ).edge_d( ) );
			}
			++il;
		}
		return NULL;
	}

	/** get an edge of this vertex, const version
	    @param nto destination node (index)
	    @return a pointer to the edge data, NULL if edge doesn't exist
	*/
	virtual const EdgeDT *get_edge( int nto ) const {
		if( m_lEdges.empty() ) {
			return NULL;
		}
		const_iterator il = m_lEdges.begin();
		while( il != m_lEdges.end() ) {
			if( nto == ( *il ).to( ) ) {
				return &(( *il ).edge_d( ) );
			}
			++il;
		}
		return NULL;
	}

	/** get the out degree of this vertex.
	    @return out degree
	*/
	virtual int out_degree( void ) const {
		return m_lEdges.size();
	}

	// edge insertion/removal routines, used by CGraph

	/** insert an edge for this vertex.
	  If to the given destination node already exists,
	  old edge data will be replaced.
	  @param nto destination node (index)
	  @param edge edge data
	*/
	void insert_edge( int nto, EdgeDT edge ) {
		typename std::list<iET>::iterator it =
			std::find_if( m_lEdges.begin(), m_lEdges.end(),
						  std::bind2nd( equal_n<EDT>(), nto ) );
		if( it == m_lEdges.end() ) {
			m_lEdges.push_back( CiEdge<EdgeDT>( nto, edge ) );
		} else {
			m_lEdges.erase( it );
			m_lEdges.push_back( CiEdge<EdgeDT>( nto, edge ) );
		}
	}

	/** remove an edge for this vertex.
	    Has no effect if the given edge doesn't exist.
	  @param nto destination node (index)
	*/
	void remove_edge( int nto ) {
		typename std::list<iET>::iterator it = m_lEdges.begin();
		while( it != m_lEdges.end() ) {
			if(( *it ).to( ) == nto ) {
				m_lEdges.erase( it );
				break;
			}
			++it;
		}
	}

	/** remove all edges for this vertex.
	*/
	void remove_all( void ) {
		m_lEdges.clear();
	}

};

/** The graph class.
  A graph is represented as an adjacency list, naturally a directed graph.
  To store an un-directed graph, each edge will be kept as two copies.
  A vertex has "vertex data" of type VertexDT, which could be simple type
  (e.g., int, string) or a compound type (e.g., including x,y coordinates).
  An edge has "edge data" of type EdgeDT, which could be "simple" data
  type (int, double representing weight), or compound (two double number,
  or a weight plust a string as a "label"). See edge.h

  In many comments, the words of vertex and node are used interchangeablly.

  Internally vertices (adjacency list) are stored in a vector,
  for fast access. Many algorithms will use [ index ] to access vertices
  (with their edges) by index.

  VertexDT: data type of vertex data, must have default constructor defined;
  a default value of VertexDT type must be provided for the constructor,
  which should be some value a "normal" vertex never has.
  EdgeDT: data type of edge data.
  f_eqv: a function object which compares two vertex data to see they
  are equal; by default, std::equal_to is used.

  @author Aiguo Fei
  @version 0.5a, November/December 2000
*/
template < class VertexDT, class EdgeDT, class f_eqv = std::equal_to<VertexDT> >
class CGraph: public IGraph < VertexDT, EdgeDT,
	f_eqv,
	typename std::vector<CiVertex<VertexDT, EdgeDT> >::iterator,
	typename std::vector<CiVertex<VertexDT, EdgeDT> >::const_iterator,
	CiVertex<VertexDT, EdgeDT> >
{

public:

	/**@name type definitions.
	*/
	//@{

	/// edge data type
	typedef EdgeDT EDT;

	/// vertex data type
	typedef VertexDT VDT;

	/// edge type
	typedef CiEdge<EdgeDT> iET;

	typedef CiVertex<VertexDT, EdgeDT> iVT;

	/// graph type, this class itself
	typedef CGraph<VertexDT, EdgeDT, f_eqv> GT;

	/// type compare function object to determine if two vertex data are equal
	typedef f_eqv COMPT;


	/** iterator type to access vertices.
	*/
	typedef typename std::vector<CiVertex<VertexDT, EdgeDT> >::iterator iterator;

	/** iterator type to access vertices.
	    Only read access is allowed.
	*/
	typedef typename std::vector<CiVertex<VertexDT, EdgeDT> >::const_iterator
	const_iterator;

	/// edge iterator
	typedef typename iVT::iterator e_iterator;

	/// edge iterator, const
	typedef typename iVT::const_iterator const_e_iterator;

	typedef IGraph < VertexDT, EdgeDT, f_eqv,
			typename std::vector<CiVertex<VertexDT, EdgeDT> >::iterator,
			typename std::vector<CiVertex<VertexDT, EdgeDT> >::const_iterator,
			CiVertex<VertexDT, EdgeDT> > igraph_base_type;

	//@}

protected:

	/**@name data members */
	//@{
	/// data member, vertices
	std::vector< CiVertex<VertexDT, EdgeDT> > m_Vertices;

	/// default vertex value
	VertexDT m_vDefault;

	/// size of the graph, which is the number of vertices
	int m_nSize;

	/// growth percentage when vertex vector is full
	double m_dGrow;

	/// list of unused indices in m_Vertices
	afl::sorted_list<int> m_iUnused;
	//@}

public:
	/** constructor
	@param v a vertex data value that won't be taken by any vertex,
	  internally used by CGraph to fill unused vertex table entries.
	@param dg growth percentage to grow a graph when graph needs to grow
	due to insertion of new vertices.
	@param eqv predicate to determine if two vertices are equivalent (having
	the same key or id); default std::equal_to is used.
	*/
	CGraph( int size = 1, const VertexDT &v = VertexDT(), double dg = 0.25,
			const f_eqv &eqv = f_eqv() )
		: IGraph < VertexDT, EdgeDT, f_eqv,
		  typename std::vector<CiVertex<VertexDT, EdgeDT> >::iterator,
		  typename std::vector<CiVertex<VertexDT, EdgeDT> >::const_iterator,
		  CiVertex<VertexDT, EdgeDT> >
		  ( size, v, dg, eqv ),
		  m_Vertices( size, CiVertex<VertexDT, EdgeDT>( v ) ),
		  m_vDefault( v ), m_nSize( 0 ), m_dGrow( dg ) {
		// initially vertex table is empty and all indices are unused
		for( int i = size - 1; i >= 0; --i ) {
			m_iUnused.push( i );
		}
	}

	~CGraph()
	{}

	CGraph( const CGraph<VertexDT, EdgeDT, f_eqv> &rhs )
		: IGraph < VertexDT, EdgeDT,
		  f_eqv,
		  typename std::vector<CiVertex<VertexDT, EdgeDT> >::iterator,
		  typename std::vector<CiVertex<VertexDT, EdgeDT> >::const_iterator,
		  CiVertex<VertexDT, EdgeDT> >
		  ( rhs.size(), rhs.m_vDefault, rhs.m_dGrow, rhs.m_eqv ),
		  m_Vertices( rhs.m_Vertices ), m_vDefault( rhs.m_vDefault ),
		  m_nSize( rhs.m_nSize ), m_dGrow( rhs.m_dGrow ),
		  m_iUnused( rhs.m_iUnused )
	{ }

	const CGraph<VertexDT, EdgeDT, f_eqv>& operator=
	( const CGraph<VertexDT, EdgeDT, f_eqv>& rhs ) {
		if( this != &rhs ) {
			igraph_base_type::m_eqv = rhs.m_eqv;
			m_Vertices = rhs.m_Vertices;
			m_vDefault = rhs.m_vDefault;
			m_nSize = rhs.m_nSize;
			m_dGrow = rhs.m_dGrow;
			m_iUnused = rhs.m_iUnused;
		}
		return *this;
	}

	/// copy all vertices from a graph
	virtual void copy_vertices( const GT &gra ) {
		m_Vertices.resize( gra.range() );
		for( int i = 0; i < gra.range(); ++i ) {
			m_Vertices[i] = GT::iVT( gra[i].vertex_d() );
		}
		m_vDefault = gra.m_vDefault;
		m_nSize = gra.m_nSize;
		m_dGrow = gra.m_dGrow;
		m_iUnused = gra.m_iUnused;
	}

	/**@name element access and helpers*/
	//@{

	/// return the default vertex value
	const VDT &v_default( void ) const {
		return m_vDefault;
	}

	/// return iterator pointing to the first vertex
	virtual iterator begin( void ) {
		return m_Vertices.begin();
	}

	/// return iterator pointing to the end (not pointing to any vertex).
	virtual iterator end( void ) {
		return m_Vertices.end();
	}

	/// return iterator pointing to the first vertex
	virtual const_iterator begin( void ) const {
		return m_Vertices.begin();
	}

	/// return iterator pointing to the end (not pointing to any vertex).
	virtual const_iterator end( void ) const {
		return m_Vertices.end();
	}

	/** return vertex at position i
	    @exception std::out_of_range.
	*/
	virtual iVT &at( int i ) {
		if( !igraph_base_type::is_in_range( i ) ) {
			throw std::out_of_range( std::string( "at( ) of CGraph" ) );
		}
		return m_Vertices[i];
	}

	/** return vertex at position i
	*/
	virtual iVT &operator[]( int i ) {
		return m_Vertices[i];
	}

	/** return vertex at position i, const version.
	    @exception std::out_of_range.
	*/
	virtual const iVT &at( int i ) const {
		if( !igraph_base_type::is_in_range( i ) ) {
			throw std::out_of_range( std::string( "at( ) of CGraph" ) );
		}
		return m_Vertices[i];
	}

	/** return vertex at position i, const version.
	*/
	virtual const iVT &operator[]( int i ) const {
		return m_Vertices[i];
	}

	/** Get edge data (from u to v).
	   @return a pointer to that edge (data), NULL if not found.
	*/
	EDT *get_edge( const VDT &u, const VDT &v ) {
		int nfrom = find_index( u );
		if( nfrom >= 0 ) {
			int nto = find_index( v );
			if( nto >= 0 ) {
				return m_Vertices[nfrom].get_edge( nto );
			}
		}
		return NULL;
	}

	/** get edge data by index.
	    @return a pointer to that edge (data), NULL if not found.
	*/
	EDT *get_edge_byi( int nfrom, int nto ) {
		if( !igraph_base_type::is_in_range( nfrom )
			|| !igraph_base_type::is_in_range( nto ) ) {
			return NULL;
		}
		return m_Vertices[nfrom].get_edge( nto );
	}

	/** Get edge data (from u to v), const version.
	   @return a pointer to that edge (data), NULL if not found.
	*/
	const EDT *get_edge( const VDT &u, const VDT &v ) const {
		int nfrom = find_index( u );
		if( nfrom >= 0 ) {
			int nto = find_index( v );
			if( nto >= 0 ) {
				return m_Vertices[nfrom].get_edge( nto );
			}
		}
		return NULL;
	}

	/** get edge data by index.
	    @return a pointer to that edge (data), NULL if not found.
	*/
	const EDT *get_edge_byi( int nfrom, int nto ) const {
		if( !igraph_base_type::is_in_range( nfrom )
			|| !igraph_base_type::is_in_range( nto ) ) {
			return NULL;
		}
		return m_Vertices[nfrom].get_edge( nto );
	}

	/// find the index of vertex v
	virtual int find_index( const VDT &v ) const {
		size_t i = 0;
		while( i < m_Vertices.size() && !m_eqv( v, m_Vertices[i].vertex_d() ) ) {
			++i;
		}
		if( i < m_Vertices.size() ) {
			return ( int )i;
		} else {
			return -1;
		}
	}

	/// return graph size: number of vertices in the graph
	virtual int size( void ) const {
		return m_nSize;
	}

	/** return current upper bound on vertex index range.
	    Vertex index will be within [0, range()-1 ],
	    will always have range()>=size()
	*/
	virtual int range( void ) const {
		return m_Vertices.size();
	}

	/// return if i (range checked) is an index used for a vertex.
	virtual bool is_in_use( int i ) const {
		if( std::find( m_iUnused.begin(), m_iUnused.end(), i )
			!= m_iUnused.end() ) {
			return false;    // index i is still available, so it is not in use
		}
		return true;
	}

	//@}

	/** reserve a certain size for the graph.
	  Has no effect if n is less than or equal to range( );
	  otherwise capacity will be increased and all iteroators may be
	  invalidated, but an index should still pointing to the same vertex.
	*/
	virtual void reserve( int n ) {
		int olds = m_Vertices.size();
		if( n > olds ) {
			m_Vertices.resize( n, iVT( m_vDefault ) );
			for( int i = n - 1; i >= olds; --i ) {
				m_iUnused.push( i );
			}
		}
	}

	/** rearrange vertices to make them stored consecutively.
	  After pack( ), all iterators and indices may be invalidated.
	  All vertices will be stored within [0, size()-1 ].
	  @param ns new capacity; if ns<size( ), size() will be used.
	  @return number of index changes that have been made.
	*/
	virtual int pack( int ns = 0 ) {
		if( ns < m_nSize ) {
			ns = m_nSize;
		}
		//std::cout<<"size: "<<m_nSize<<", "<<m_Vertices.size()<<", "<<m_iUnused.size()<<std::endl;

		using std::pair;
		afl::sorted_list< afl::named_pair<int, int> > slind;
		int i, j, next, ii;
		// pack vertices into unused vertex table spots
		if( !m_iUnused.empty() ) {
			i = m_iUnused.front(); // first unused spot
			j = i + 1;
			m_iUnused.pop_front();
			//std::cout<<"first: "<<i<<std::endl;
			while( j < ( int )m_Vertices.size() && !m_iUnused.empty() ) {
				next = m_iUnused.front();
				m_iUnused.pop_front();
				while( next == j && !m_iUnused.empty() ) {
					// find a consecutive block to move
					++j;
					next = m_iUnused.front();
					m_iUnused.pop_front();
				}
				//std::cout<<"block: "<<i<<", "<<j<<", "<<next<<std::endl;
				for( ; j < next; ++j ) {
					// move this block
					// old index j is mapped to new index i, note: always i<j
					//std::cout<<j<<"->"<<i<<std::endl;
					m_Vertices[i] = m_Vertices[j];
					slind.push( afl::named_pair<int, int>( i, j ) );
					++i;
				}
				j = next + 1;
			} // end of while( j<... )
			// last block
			//std::cout<<"block: "<<i<<", "<<j<<", "<<m_Vertices.size()<<std::endl;
			for( ; j < ( int )m_Vertices.size(); ++j ) {
				// old index j is mapped to new index i, note: always i<j
				//std::cout<<j<<"->"<<i<<std::endl;
				m_Vertices[i] = m_Vertices[j];
				slind.push( afl::named_pair<int, int>( i, j ) );
				++i;
			}
		}// end of if
		// update edges' destination indices for all vertices affected,
		// the mapping of j->i has been sorted from small to large according to j;
		// because always i<j, the mapping update won't mess up
		// ?note: can't use iterator m_Vertices[ii].begin()
		afl::named_pair<int, int> pin;
		typename iVT::iterator it;
		int nretn = 0;
		while( !slind.empty() ) {
			pin = slind.front();
			slind.pop_front();
			for( ii = 0; ii < m_nSize; ++ii ) {
				for( it = m_Vertices[ii].begin();
					 it != m_Vertices[ii].end(); ++it ) {
					if(( *it ).to( ) == pin.key ) {
						( *it ).to( ) = pin.name;
						//std::cout<<pin.key<<"->"<<pin.name<<std::endl;
					}
				}
			}
			++nretn;
		}
		for( ii = m_nSize; ii < afl::tmin( ns, ( int )m_Vertices.size() ); ++ii ) {
			//std::cout<<"set default: "<<ii<<std::endl;
			m_Vertices[ii] = iVT( m_vDefault );
		}
		m_Vertices.resize( ns, iVT( m_vDefault ) );
		m_iUnused.clear( );
		for( ii = range() - 1; ii >= m_nSize; --ii ) {
			m_iUnused.push( ii );    // unused indices
		}

		return nretn;
	}

	/**@name insertion/deletion */
	//@{

	/** insert a vertex of value v.
	    Quick version that doesn't search for existing vertex of the same
	    value -- use this if it is known that the new vertex doesn't exist.
	    @return index of the vertex in vertex table,
	    -1 (an invalid index) to indicate failure.
	*/
	virtual int insert_v_qik( const VDT &v ) {
		// there is still unused index
		if( !m_iUnused.empty() ) {
			int i = m_iUnused.front();
			m_Vertices[i] = iVT( v );
			m_iUnused.pop_front();
			++m_nSize;
			return i;
		} else {
			// vertex table is full, grow it first
			grow();
			return insert_v_qik( v );
		}
	}

	/** insert a vertex of value v.
	    Vertex data is updated if vertex (v) already exists.
	    @return index of the vertex in vertex table,
	    -1 (an invalid index) to indicate failure.
	*/
	virtual int insert_v( const VDT &v ) {
		int vi = find_index( v );
		if( vi >= 0 ) {
			// vertex found
			m_Vertices[vi].set( v );
			return vi;
		}
		// vertex not found
		return insert_v_qik( v );
	}

	/** insert a vertex of value v at position i.
	    This allows a graph user to control vertex of each position.
	    Existing vertex at position i (if exist) will be replaced by v,
	    any existing edge will be lost.
	    However, any other edge pointing to this vertex will still be there.
	    @return true if insertion succeeds; false if fails
	      (table is smaller than i and fails to grow).
	*/
	virtual bool insert_v_ati( const VDT &v, int i ) {
		if( i < 0 ) {
			return false;
		}

		// grow the graph until is large enough for position i
		if( i >= ( int )m_Vertices.size() ) {
			grow( i + 1 );
		}

		m_Vertices[i].set( v );

		// remove i from unused index list if it is in list
		afl::sorted_list<int>::iterator it = std::find( m_iUnused.begin(),
											 m_iUnused.end(), i );
		if( it != m_iUnused.end() ) {
			m_iUnused.erase( it );
			++m_nSize; // new vertex at position i
		}
		return true;
	}

	/** remove a vertex of value v.
	    Has no effect if vertex not found; otherwise this vertex and
	    all related edges (from/to it) will be removed as well.
	*/
	virtual void remove_v( const VDT &v ) {
		int vi = find_index( v );
		if( vi >= 0 ) {
			remove_v_byi( vi );
		}
	}

	/** remove a vertex of index vi.
	    Has no effect if vertex not found; otherwise this vertex and
	    all related edges (from/to it) will be removed as well.
	*/
	virtual void remove_v_byi( int vi ) {
		if( igraph_base_type::is_valid( vi ) ) {
			// valid index
			for( int i = 0; i < ( int )m_Vertices.size(); ++i ) {
				m_Vertices[i].remove_edge( vi );
			}
			m_Vertices[vi].remove_all();
			m_Vertices[vi].set( m_vDefault );
			--m_nSize;
			m_iUnused.push( vi );
		}
	}

	/** insert an edge for vertex u to v.
	  Insertion fails if any of the vertex is not found.
	  If edge already exists then edge data will be replaced by e.
	  @return true if succeeds, false otherwise.
	*/
	virtual bool insert_e( const VDT &u, const VDT &v, const EDT &e ) {
		// find the indices of both nodes, then insert the corresponding edge,
		// if both nodes are found
		int i = find_index( u );
		if( i >= 0 ) {
			int j = find_index( v );
			if( j >= 0 ) {
				m_Vertices[i].insert_edge( j, e );
				return true;
			}
		}
		return false;
	}

	/** insert both (u->v) and (v->u).
	    The same edge data is used for both edges.
	    Equivalent to "insert_e(u,v,e); insert_e(v,u,e);".
	    @see insert_e( )
	*/
	virtual bool insert_2e( const VDT &u, const VDT &v, const EDT &e ) {
		int i = find_index( u );
		if( i >= 0 ) {
			int j = find_index( v );
			if( j >= 0 ) {
				m_Vertices[i].insert_edge( j, e );
				m_Vertices[j].insert_edge( i, e );
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
		if( !igraph_base_type::is_valid( i )
			|| !igraph_base_type::is_valid( j ) ) {
			return false;
		}

		m_Vertices[i].insert_edge( j, e );
		return true;
	}

	/** insert both (i->j) and (j->i).
	    The same edge data is used for both edges.
	    Equivalent to "insert_e(i,j,e); insert_e(j,i,e);".
	    @see insert_e_byi( )
	*/
	virtual bool insert_2e_byi( int i, int j, const EDT &e ) {
		if( !igraph_base_type::is_valid( i )
			|| !igraph_base_type::is_valid( j ) ) {
			return false;
		}

		m_Vertices[i].insert_edge( j, e );
		m_Vertices[j].insert_edge( i, e );
		return true;
	}

	/** remove edge (u->v).
	    Has no effect if edge not found.
	*/
	virtual void remove_e( const VDT &u, const VDT &v ) {
		// find the indices of both nodes, then remove the corresponding edge,
		// if found
		int nfrom = find_index( u );
		if( nfrom >= 0 ) {
			int nto = find_index( v );
			remove_e_byi( nfrom, nto );
		}
	}

	/** remove both (u->v) and (v->u).
	    Equivalent to "remove_e(u,v); remove_e(v,u);".
	    Has no effect if edge not found. */
	virtual void remove_2e( const VDT &u, const VDT &v ) {
		int n1 = find_index( u );
		if( n1 >= 0 ) {
			int n2 = find_index( v );
			if( n2 >= 0 ) {
				m_Vertices[n1].remove_edge( n2 );
				m_Vertices[n2].remove_edge( n1 );
			}
		}
	}

	/** remove all edges of vertex v.
	    Has no effect if vertex not found. */
	virtual void remove_alle( const VDT &u ) {
		int nfrom = find_index( u );
		remove_alle_byi( nfrom );
	}

	/** remove an edge by index.
	    Has no effect if edge not found. */
	virtual void remove_e_byi( int nfrom, int nto ) {
		if( igraph_base_type::is_in_range( nfrom )
			&& igraph_base_type::is_in_range( nto ) ) {
			m_Vertices[nfrom].remove_edge( nto );
		}
	}

	/** remove both edges by index.
	    Has no effect if edge not found. */
	virtual void remove_2e_byi( int n1, int n2 ) {
		if( igraph_base_type::is_in_range( n1 ) && igraph_base_type::is_in_range( n2 ) ) {
			m_Vertices[n1].remove_edge( n2 );
			m_Vertices[n2].remove_edge( n1 );
		}
	}

	/// remove all edges of vertex of index nfrom
	virtual void remove_alle_byi( int nfrom ) {
		if( igraph_base_type::is_in_range( nfrom ) ) {
			m_Vertices[nfrom].remove_all();
		}
	}

	/// remove all edges of all vertices
	virtual void remove_all_edges( void ) {
		for( int i = 0; i < ( int )m_Vertices.size(); ++i ) {
			m_Vertices[i].remove_all();
		}
		;
	}

	/// clear all nodes and all their edges
	virtual void clear( void ) {
		m_nSize = 0;
		m_iUnused.clear();
		for( int i = ( int )m_Vertices.size() - 1; i >= 0; --i ) {
			m_Vertices[i] = iVT( m_vDefault );
			m_iUnused.push( i );
		}
	}
	//@} // end of insertion/deletion

protected:
	// grow the graph size by a certain percentage
	virtual int grow( int ns = 0 ) {
		int nold = m_Vertices.size();
		int n = ( int )( nold * ( 1 + m_dGrow ) );
		n = ( n > nold ) ? n : nold + 1;
		n = ( n > ns ) ? n : ns;

		m_Vertices.resize( n, iVT( m_vDefault ) );
		for( int i = n - 1; i >= nold; --i ) {
			m_iUnused.push( i );
		}

		return n;
	}

};
// end of CGraph

/// type of the graph provided by this file
#define graph_type afg::CGraph

#define _GRAPHDEFINED

/**@}*/

}// end of namespace afg

#endif
