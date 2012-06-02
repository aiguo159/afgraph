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


/* t_graph_intf.cpp

  test edge and vertex interfaces defined in graph_intf.h

  Aiguo Fei

  1999, 2000
*/

#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>

#include "afl/util_tl.hpp"
#include "afgraph/graph_intf.h"
#include "afgraph/edge.h"


//using namespace std;
using namespace afg;

template<class EdgeDT>
struct CiEdge: public IEdge<EdgeDT> {
public:
	/// type of the edge data
	typedef EdgeDT EDT;

	/// data member, destination node (index)
	int m_nTo;
	/// data member, edge data
	EdgeDT m_EdgeD;

	/// constructor
	explicit CiEdge( int nto = -1, const EdgeDT &edg = EdgeDT() )
		: m_nTo( nto ), m_EdgeD( edg ) { }

	/// return destination node (index)
	int to( void ) const {
		return m_nTo;
	};

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
	/// output IEdge to a stream in format of "edge_index: edge_data"
	virtual std::ostream &output( std::ostream &os ) const {
		os << "to " << to( ) << ": " << edge_d( );
		return os;
	}
};

template<class VertexDT, class EdgeDT>
class CVertex: public IVertex < VertexDT, EdgeDT,
	typename std::list<CiEdge<EdgeDT> >::iterator,
	typename std::list<CiEdge<EdgeDT> >::const_iterator >
{
public:
	VertexDT m_VertexD;
	std::list<CiEdge<EdgeDT> > m_lEdges;

	typedef CiEdge<EdgeDT> iET;
	typedef CVertex<VertexDT, EdgeDT> iVT;

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

public:

	/// constructor.
	explicit CVertex( const VertexDT v ) { m_VertexD = v; }

	/// return a reference of the vertex data
	virtual VertexDT &vertex_d( void ) {
		return m_VertexD;
	}
	/// return a reference of the vertex data, const version
	virtual const VertexDT &vertex_d( void ) const {
		return m_VertexD;
	}

	virtual iterator begin( void ) {
		return m_lEdges.begin();
	}
	virtual iterator end( void ) {
		return m_lEdges.end();
	}

	virtual const_iterator begin( void ) const {
		return m_lEdges.begin();
	}
	virtual const_iterator end( void ) const {
		return m_lEdges.end();
	}

	virtual int out_degree( void ) const {
		return m_lEdges.size();
	}

	void insert_edge( int i, const EDT &ed ) {
		m_lEdges.push_back( CiEdge<EDT>( i, ed ) );
	}

	virtual EdgeDT *get_edge( int nto ) {
		if( m_lEdges.empty() ) {
			return NULL;
		}
		typename std::list<iET>::iterator il = m_lEdges.begin();
		while( il != m_lEdges.end() ) {
			if( nto == ( *il ).m_nTo ) {
				return &(( *il ).m_EdgeD );
			}
			++il;
		}
		return NULL;
	}

	virtual const EdgeDT *get_edge( int nto ) const {
		if( m_lEdges.empty() ) {
			return NULL;
		}
		typename std::list<iET>::const_iterator il = m_lEdges.begin();
		while( il != m_lEdges.end() ) {
			if( nto == ( *il ).m_nTo ) {
				return &(( *il ).m_EdgeD );
			}
			++il;
		}
		return NULL;
	}
};

void tv( const CVertex<int, int>& v, int i )
{
	const int *pn = v.get_edge( i );
	if( pn ) {
		std::cout << *pn << std::endl;
	}
}

using std::cout;
using std::endl;

int main()
{
	CiEdge<int> edge( 1, 4 );
	cout << edge << std::endl;
	cout << "to " << edge.to() << ", " << "data: " << edge.edge_d() << endl;
	edge.edge_d() = 104;
	cout << "now: " << edge << endl;
	CiEdge<int> edge1( edge );
	CiEdge<int> edge2 = edge1;
	CiEdge<int> edge3( 2, 5 );
	CiEdge<int> edge4( 1, 6 );
	//afg::CiEdge<int> edge11=5; // should get compile error if uncomment this one
	std::cout << edge1 << std::endl;
	std::cout << edge2 << std::endl;
	cout << ( edge1 == edge2 ) << ", " << ( edge1 == edge3 ) << ", " << ( edge1 == edge4 ) << endl;
	equal_i<int> eqi;
	equal_n<int> eqn;
	cout << eqi( edge1, edge2 ) << ", " << eqn( edge1, 1 ) << ", " << eqn( edge1, 2 ) << endl;

	CVertex<int, int> v0( 0 );
	v0.insert_edge( 1, 5 );
	v0.insert_edge( 2, 11 );
	v0.output( cout );
	cout << endl << v0 << endl;
	CVertex<int, int>::iterator it = v0.find_edge( 2 );
	if( it != v0.end() ) {
		cout << "to " << it->to() << endl;
		( *it ).to() = 5;
		cout << "after change: " << *it << endl;
		//(*it).to()=2;
	} else { cout << "not found\n"; }
	it = v0.find_edge( 1 );
	if( it != v0.end() ) {
		cout << *it << endl;
	} else { cout << "not found\n"; }
	it = v0.find_edge( 3 );
	if( it != v0.end() ) {
		cout << *it << endl;
	} else { cout << "not found\n"; }

	int *pn;
	pn = v0.get_edge( 1 );
	if( pn ) {
		cout << *pn << endl;
		*pn = *pn + 8;
		it = v0.find_edge( 1 );
		if( it != v0.end() ) {
			cout << *it << endl;
		} else { cout << "not found\n"; }
	}

	tv( v0, 1 );
	tv( v0, 2 );


	CVertex<int, double> vert0( 0 );
	vert0.insert_edge( 1, 3.5 );
	vert0.insert_edge( 2, 4.3 );
	std::cout << "vertex: \n" << vert0 << std::endl;

	CEdgeW2<int, double> e1( 12, 3.4 );
	CEdgeW2<int, double> e2( 13, 2.5 );
	std::cout << e1 << std::endl;
	std::cout << e2 << std::endl;

	CVertex<int, CEdgeW2<int, double> > vert2( 0 );
	vert2.insert_edge( 1, e1 );
	vert2.insert_edge( 2, e2 );
	std::cout << "vertex: \n" << vert2 << std::endl;

	return 0;
}
