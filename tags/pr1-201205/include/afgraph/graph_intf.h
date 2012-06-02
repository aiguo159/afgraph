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


/**@file graph_intf.h

  defintions of interface classes IEdge, IVertex and IGraph

  see also graph.h

  todo: design documentation,
    decision on vertex iterator (allow invalid vertex or not)
    ?add explicit un-directed graph support (by conditional compiling?)

  Aiguo Fei

  1999, 2000, 2001
*/

#ifndef _AFG_GRAPH_INTF_H_
#define _AFG_GRAPH_INTF_H_

#include <iostream>
#include <stdexcept>
#include <functional>
#include <algorithm>

/**@defgroup graphd Graph Data Structures
	Data structures to support graph operations and algorithms.
*/

/** A Framework for Graph (network) data structures and algorithms.
*/
namespace afg
{

/**@defgroup gintf Interface Classes
@ingroup graphd
*/
/**@{*/

/** Edge interface.
An edge can return an index of the destination node and edge data
    of type EdgeDT.
*/
template<class EdgeDT>
class IEdge
{
public:
	/// type of the edge data
	typedef EdgeDT EDT;

	virtual ~IEdge()
	{}

	/// return destination node (index)
	virtual int to( void ) const = 0;

	/// return reference of destination node (index)
	virtual int &to( void ) = 0;

	/// return reference of edge data
	virtual EdgeDT &edge_d( void ) = 0;

	/// return reference of edge data, const version
	virtual const EdgeDT &edge_d( void ) const = 0;

	/// set data of the edge
	virtual void set
	( const EdgeDT &e ) {
		edge_d() = e;
	}

	/// output IEdge to a stream in format of "edge_index: edge_data"
	virtual std::ostream &output( std::ostream &os ) const {
		os << to( ) << ": " << edge_d( );
		return os;
	}
};

/// equal operator for IEdge
template<class EdgeDT>
inline bool operator==( const IEdge<EdgeDT>& lhs, const IEdge<EdgeDT>& rhs )
{
	return ( lhs.to( ) == rhs.to( ) && lhs.edge_d( ) == rhs.edge_d( ) );
}

/// not_equal operator for IEdge
template<class EdgeDT>
inline bool operator!=( const IEdge<EdgeDT>& lhs, const IEdge<EdgeDT>& rhs )
{
	return !( lhs.to( ) == rhs.to( ) );
}

/// less operator for IEdge
template<class EdgeDT>
inline bool operator<( const IEdge<EdgeDT>& lhs, const IEdge<EdgeDT>& rhs )
{
	return ( lhs.to( ) < rhs.to( ) );
}

/// larger operator for IEdge
template<class EdgeDT>
inline bool operator>( const IEdge<EdgeDT>& lhs, const IEdge<EdgeDT>& rhs )
{
	return ( lhs.to( ) > rhs.to( ) );
}

/// conventional output operator for IEdge
template<class EdgeDT>
inline std::ostream &operator<<( std::ostream &os,
								 const IEdge<EdgeDT>& edge )
{
	return ( &edge )->output( os );
}

// helper function objects

/// predicate to test if the indices of two edges are the same.
template<class EDT>
struct equal_i: std::binary_function<IEdge<EDT>, IEdge<EDT>, bool> {
	///
	bool operator()( const IEdge<EDT>& lhs, const IEdge<EDT>& rhs ) const {
		return ( lhs.to( ) == rhs.to( ) );
	}
};

/// predicate to test if the index of an edge is equal to an integer.
template<class EDT>
struct equal_n: std::binary_function<IEdge<EDT>, int, bool> {
	///
	bool operator()( const IEdge<EDT>& lhs, int n ) const {
		return ( lhs.to( ) == n );
	}
};


/** a struct that has nothing.
*/
struct Cvoid
	{}
;

/** Interface of internal vertex in a graph.
    VertexDT is the type of data contained in a vertex, simple data type
    (int, char) or compound type.
    EdgeT is an edge type which provides an IEdge interface.
    Requirements of a vertex type:
    (1)provides all interfaces defined below;
    (2)provides iterator-based access to edges; assume
      "iterator_type it;", then *it should return an EdgeT;
    (3)VertexDT has to be uniquely identifiable.
*/
template < class VertexDT, class EdgeDT,
		 class iterator_type = Cvoid,
		 class const_iterator_type = Cvoid >
class IVertex
{

public:

	/// edge data type
	typedef EdgeDT EDT;

	/// edge type, providing the interface defined above
	typedef IEdge<EdgeDT> iET;

	/// vertex data type
	typedef VertexDT VDT;

	/// vertex type, this class itself
	typedef IVertex<VDT, EDT, iterator_type, const_iterator_type> iVT;

	/** iterator type to access edges.
	*/
	typedef iterator_type iterator;

	/** const iterator type to access edges (read-only).
	*/
	typedef const_iterator_type const_iterator;


	virtual ~IVertex()
	{}

	/// return a reference of the vertex data
	virtual VertexDT &vertex_d( void ) = 0;

	/// return a reference of the vertex data, const version
	virtual const VertexDT &vertex_d( void ) const = 0;

	/// set vertex data
	virtual void set
	( const VertexDT &v ) {
		vertex_d() = v;
	}

	/// return iterator pointing to the first edge
	virtual iterator begin( void ) = 0;

	/// return iterator pointing to the end (not pointting to any endge).
	virtual iterator end( void ) = 0;

	/// return iterator pointing to the first edge, const version
	virtual const_iterator begin( void ) const = 0;

	/// return iterator pointing to the end, const version.
	virtual const_iterator end( void ) const = 0;

	/** find the edge pointing to node of index nto
	    @return iterator pointing to that edge,
	    equal to end( ) if no edge pointing to nto found.
	*/
	virtual iterator find_edge( int nto ) {
		return std::find_if( begin(), end(), std::bind2nd( equal_n<EDT>(), nto ) );
	}

	/** find the edge pointing to node of index nto
	    @return const_iterator pointing to that edge,
	    equal to end( ) if no edge pointing to nto found.
	*/
	virtual const_iterator find_edge( int nto ) const {
		return std::find_if( begin(), end(), std::bind2nd( equal_n<EDT>(), nto ) );
	}

	/** get an edge of this vertex.
	    @param nto destination node (index)
	    @return a pointer to the edge data, NULL if edge doesn't exist
	*/
	virtual EDT *get_edge( int nto ) = 0;

	/** get an edge of this vertex, const version.
	    @param nto destination node (index)
	    @return a pointer to the edge data, NULL if edge doesn't exist
	*/
	virtual const EDT *get_edge( int nto ) const = 0;

	/** get the out degree of this vertex.
	    @return out degree
	*/
	virtual int out_degree( void ) const = 0;

	/// output the vertex to an output stream
	virtual std::ostream &output( std::ostream &os ) const {
		os << vertex_d( ) << "-> ";

		const_iterator it = begin();

		// output first edge first,
		// so "," will not be printed if there is only one edge
		if( it != end() ) {
			os << *it;
		}
		while( ++it != end() ) {
			os << ", " << *it;
		}
		return os;
	}

};

/// output operator for IVertex
template < class VertexDT, class EdgeDT,
		 class iterator_type,
		 class const_iterator_type >
inline std::ostream &operator<<( std::ostream &os,
								 const IVertex < VertexDT, EdgeDT,
								 iterator_type,
								 const_iterator_type > & rhs )
{
	return ( &rhs )->output( os ); // simply use output( ) member function.
}


/** Interface of a graph.
  Requirements of a graph class:
    (1)Provides an interface defined below.
    (2)Provides iterator-based access to vertices, an iterator gets
      a VertexT type which provides an interface defined in IVertex.
    (3)Provides operator [ ] based random access to vertices, which returns
      a VertexT type; the index must be within a [0, range( )-1] range;
      this is to facilitate the implementations of many algorithms.
      However, an index or iterator may point to a "vertex" which is not
      part of the graph (removed or empty position); such a "vertex"
      is isolated without any neighbors, this doesn't affect many algorithms;
      is_valid( int i) can tell if index i points to a valid vertex;
      pack( ) can be called to make all vertices appear in consecutive order
      within [0, size( )-1].
    (4)It guarantees that the same index points to the same
      vertex before a pack( ) function call is made.
      However, edge removal may lead to poisitioning change and my invalidate
      iterator pointing to an edge.
    (5)vertex iterator?
*/

template < class VertexDT, class EdgeDT,
		 class f_eqv = std::equal_to<VertexDT>,
		 class v_iterator_type = Cvoid,
		 class const_v_iterator_type = Cvoid,
		 class CVertex = IVertex<VertexDT, EdgeDT> >
class IGraph
{

protected:
	/** predicate to determine if two vertex data
	    (of type VertexDT) are equivalent (e.g., with same key or id)
	*/
	f_eqv m_eqv;

public:

	/**@name type definitions.
	An implementation should provide all the following type defintions.
	*/
	//@{
	/// edge data type
	typedef EdgeDT EDT;

	/// vertex data type
	typedef VertexDT VDT;

	/// edge type, edge destination along with data
	typedef IEdge<EDT> iET;

	/// vertex type,
	typedef CVertex iVT;

	/// type compare function object to determine if two vertex data are equal
	typedef f_eqv COMPT;

	/// graph type, this class itself
	typedef IGraph < VertexDT, EdgeDT,
			v_iterator_type, const_v_iterator_type,
			f_eqv, CVertex > GT;

	/** iterator type to access vertices.
	*/
	typedef v_iterator_type iterator;

	/** iterator type to access vertices.
	    Only read access is allowed.
	*/
	typedef const_v_iterator_type const_iterator;

	/// edge iterator
	typedef typename iVT::iterator e_iterator;

	/// edge iterator, const
	typedef typename iVT::const_iterator const_e_iterator;

	//@}


	/*
	@param size initial graph size
	@param v default constructor for vertex data
	@param dg default graph size growth factor (i.e., if the graph
	is full, the graph will be increased to be (1+dg)*original_size --
	extra space is reserved for future growth).
	@param eqv functional object that determines if two vertex
	data are equavilent
	*/
	IGraph( int size = 1, const VertexDT &v = VertexDT(), double dg = 0.25,
			const f_eqv &eqv = f_eqv() ): m_eqv( eqv )
	{ }

	virtual ~IGraph()
	{}


	/**@name element access and helpers*/
	//@{

	/// return iterator pointing to the first vertex
	virtual iterator begin( void ) = 0;

	/// return iterator pointing to the end (not pointing to any vertex).
	virtual iterator end( void ) = 0;

	/// return iterator pointing to the first vertex, const version.
	virtual const_iterator begin( void ) const = 0;

	/// return iterator pointing to the end, const version.
	virtual const_iterator end( void ) const = 0;

	/** return vertex at position i
	    @exception std::out_of_range.
	*/
	virtual iVT &at( int i ) = 0;
	/** return vertex at position i.
	    Behavior is undefined if i is out of range,
	    normally results in run-time error.
	*/
	virtual iVT &operator[]( int i ) = 0;

	/** return vertex at position i, const version.
	    @exception std::out_of_range.
	*/
	virtual const iVT &at( int i ) const = 0;

	/** return vertex at position i, const version.
	    Behavior is undefined if i is out of range,
	    normally results in run-time error.
	*/
	virtual const iVT &operator[]( int i ) const = 0;

	/** return the vertex data of at position index.
	    @param nindex position index, should be within [0, size()-1 ].
	    @return a reference to vertex data at position nindex
	    (type VertexDT).
	    @exception std::out_of_range, indirectly through at.
	*/
	virtual VDT &vertex_d( int nindex ) {
		return at( nindex ).vertex_d( );
	}

	/** return the vertex data of at position index, const version.
	    @see vertex_d( ).
	*/
	virtual const VDT &vertex_d( int nindex ) const {
		return at( nindex ).vertex_d( );
	}

	/** return iterator pointing to the first edge of vertex of index i.
	    @exception std::out_of_range, with message "e_begin( ) of IGraph".
	*/
	virtual e_iterator e_begin( int i ) {
		if( !is_in_range( i ) ) {
			throw std::out_of_range( std::string( "e_begin( ) of IGraph" ) );
		}
		return at( i ).begin();
	}

	/** return iterator pointing to the end of edges of vertex of index i.
	    @exception std::out_of_range, with message "e_end( ) of IGraph".
	*/
	virtual e_iterator e_end( int i ) {
		if( !is_in_range( i ) ) {
			throw std::out_of_range( std::string( "e_end( ) of IGraph" ) );
		}
		return at( i ).end();
	}

	/** return const iterator pointing to the first edge of vertex of index i.
	    @exception std::out_of_range, with message "e_begin( ) of IGraph".
	*/
	virtual const_e_iterator e_begin( int i ) const {
		if( !is_in_range( i ) ) {
			throw std::out_of_range( std::string( "e_begin( ) of IGraph" ) );
		}
		return at( i ).begin();
	}

	/** return const iterator pointing to the end of edges of vertex of index i.
	    @exception std::out_of_range, with message "e_end( ) of IGraph".
	*/
	virtual const_e_iterator e_end( int i ) const {
		if( !is_in_range( i ) ) {
			throw std::out_of_range( std::string( "e_end( ) of IGraph" ) );
		}
		return at( i ).end();
	}

	/// find the index of vertex v
	virtual int find_index( const VDT &v ) const = 0;

	/// find vertex equal to v, return position (iterator)
	virtual iterator find_vertex( const VDT &v ) {
		iterator it = begin();
		while( it != end() ) {
			if( m_eqv(( *it ).vertex_d(), v ) ) {
				break;
			}
			++it;
		}
		return it;
	}

	/// find vertex equal to v, return position (iterator), const version
	virtual const_iterator find_vertex( const VDT &v ) const {
		const_iterator it = begin();
		while( it != end() ) {
			if( m_eqv(( *it ).vertex_d(), v ) ) {
				break;
			}
			++it;
		}
		return it;
	}


	/** Get edge data (from u to v).
	   @return a pointer to that edge (data), NULL if not found.
	*/
	virtual EDT *get_edge( const VDT &u, const VDT &v ) = 0;

	/** get edge data by index.
	    @return a pointer to that edge (data), NULL if not found.
	*/
	virtual EDT *get_edge_byi( int nfrom, int nto ) = 0;

	/** Get edge data (from u to v), const version.
	   @return a pointer to that edge (data), NULL if not found.
	*/
	virtual const EDT *get_edge( const VDT &u, const VDT &v ) const = 0;

	/** get edge data by index, const version.
	    @return a pointer to that edge (data), NULL if not found.
	*/
	virtual const EDT *get_edge_byi( int nfrom, int nto ) const = 0;

	/// return graph size: number of vertices in the graph
	virtual int size( void ) const = 0;

	/** return current upper bound on vertex index range.
	    Vertex index will be within [0, range()-1 ],
	    will always have range()>=size()
	*/
	virtual int range( void ) const = 0;

	/// is i within the index access range
	bool is_in_range( int i ) const {
		return !( i < 0 || i >= range() );
	}

	/// return if i (range checked) is an index used for a vertex.
	virtual bool is_in_use( int i ) const = 0;

	/// is i an index pointing to a "valid" vertex
	bool is_valid( int i ) const {
		return ( is_in_range( i ) && is_in_use( i ) );
	}
	//@}

	/** reserve a certain size for the graph.
	  n has to be >=range( ), otherwise it has no effect;
	  Range will will be increased to n, and all iteroators may be invalidated,
	  but an index should still pointing to the same vertex.
	*/
	virtual void reserve( int n ) = 0;

	/** rearrange vertices to make them stored consecutively.
	  After pack( ), all iterators and indices may be invalidated.
	  All vertices will be stored within [0, size() ).
	  @param ns new capacity; if ns<size( ), size() will be used.
	  @return number of index changes that have been made.
	*/
	virtual int pack( int ns = 0 ) = 0;


	/**@name insertion/deletion */
	//@{

	/** insert a vertex of value v.
	    Quick version that doesn't search for existing vertex of the same
	    value -- use this if it is known that the new vertex doesn't exist.
	    @return index of the vertex in vertex table,
	    -1 (an invalid index) to indicate failure.
	*/
	virtual int insert_v_qik( const VDT &v ) = 0;

	/** insert a vertex of value v.
	    Vertex data is updated if vertex (v) already exists.
	    @return index of the vertex in vertex table,
	    -1 (an invalid index) to indicate failure.
	*/
	virtual int insert_v( const VDT &v ) = 0;

	/** insert a vertex of value v at position i.
	    This allows a graph user to control vertex at each position.
	    Vertex data is updated if vertex (v) already exists.
	    @return true if insertion succeeds; false if fails
	      (table is smaller than i and fails to grow).
	*/
	virtual bool insert_v_ati( const VDT &v, int i ) = 0;

	/** ?? set the value of vertex at position i.
	    This allows a graph user to control vertex at each position.
	    Value of existing vertex at position i (if exist) will be replaced by v,
	    any existing edge will be kept.
	    Has no effect if there is no vertex at position i.
	    @return true if replacement succeeds; false if there is no vertex at i.
	*/
	virtual bool set_v_ati( const VDT &v, int i ) {
		if( !is_valid( i ) ) {
			return false;
		}
		at( i ).set( v );
		return true;
	}

	/** remove a vertex of value v.
	    Has no effect if vertex not found; otherwise this vertex and
	    all related edges (from/to it) will be removed as well.
	*/
	virtual void remove_v( const VDT &v ) = 0;

	/** remove a vertex of index vi.
	    Has no effect if vertex not found; otherwise this vertex and
	    all related edges (from/to it) will be removed as well.
	*/
	virtual void remove_v_byi( int vi ) = 0;

	/** insert an edge for vertex u to v.
	  Insertion fails if any of the vertex is not found.
	  If edge already exists then edge data will be replaced by e.
	  @return true if succeeds, false otherwise.
	*/
	virtual bool insert_e( const VDT &u, const VDT &v, const EDT &e ) = 0;

	/** insert both (u->v) and (v->u).
	    The same edge data is used for both edges.
	    Equivalent to "insert_e(u,v,e); insert_e(v,u,e);".
	    @see insert_e( )
	*/
	virtual bool insert_2e( const VDT &u, const VDT &v, const EDT &e ) = 0;

	/** insert an edge (i->j).
	  Insertion fails if any of the two indices is unused
	  (not vertext at that position).
	  @return true if succeeds, otherwise false.
	*/
	virtual bool insert_e_byi( int i, int j, const EDT &e ) = 0;

	/** insert both (i->j) and (j->i).
	    The same edge data is used for both edges.
	    Equivalent to "insert_e(i,j,e); insert_e(j,i,e);".
	    @see insert_e_byi( )
	*/
	virtual bool insert_2e_byi( int i, int j, const EDT &e ) = 0;

	/** remove edge (u->v).
	    Has no effect if edge not found.
	*/
	virtual void remove_e( const VDT &u, const VDT &v ) = 0;

	/** remove both (u->v) and (v->u).
	    Equivalent to "remove_e(u,v); remove_e(v,u);".
	    Has no effect if edge not found.
	*/
	virtual void remove_2e( const VDT &u, const VDT &v ) = 0;

	/** remove all edges of vertex v.
	    Has no effect if vertex not found.
	*/
	virtual void remove_alle( const VDT &u ) = 0;

	/** remove an edge by index.
	    Has no effect if edge not found.
	*/
	virtual void remove_e_byi( int nfrom, int nto ) = 0;

	/** remove both edges by index.
	    Has no effect if edge not found.
	*/
	virtual void remove_2e_byi( int n1, int n2 ) = 0;

	/// remove all edges of vertex of index nfrom
	virtual void remove_alle_byi( int nfrom ) = 0;

	/// remove all edges of all vertices
	virtual void remove_all_edges( void ) = 0;

	/// clear all nodes and all their edges
	virtual void clear( void ) = 0;

	//@} // end of insertion/deletion

	/// output graph to a stream
	virtual std::ostream &output( std::ostream &os ) const {
		for( int i = 0; i < range( ); ++i ) {
			if( is_in_use( i ) ) {
				os << '(' << i << ')' << at( i ) << std::endl;
			}
		}
		return os;
	}

};

/// conventional output operator for IGraph
template < class VertexDT, class EdgeDT,
		 class iterator_type, class const_v_iterator_type,
		 class f_eqv,
		 class CVertex >
inline std::ostream &operator<<( std::ostream &os,
								 const IGraph < VertexDT, EdgeDT,
								 iterator_type, const_v_iterator_type,
								 f_eqv, CVertex > & rhs )
{
	return ( &rhs )->output( os );
}

/**@}*/

}// end of namespace afg

#endif
