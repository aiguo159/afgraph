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


/**@file nmcbase.h

  network multicast base class

  Aiguo Fei
  2000, 2001
*/

#ifndef _AFG_NMCBASE_H_
#define _AFG_NMCBASE_H_

#include <vector>
#include <iostream>

#include "afgraph/path.h"

namespace afg
{

/**@addtogroup research
*/

/**@{
*/

/** base class for network multicast.
  Essentially provides a shortest-path "routing" table that can be used
    to build various multicast trees, along with two additional
    characteristics on the shortest-paths.
  1st weight is the underlying unicast routing weight (shortest path);
  2nd weight is an end-to-end measure that can be used for TM routing;
  3rd weight is another end-to-end measure.
  1st one most likely to be assigned link cost,
  2nd/3rd one can be: end-to-end delay, hop-count.

  After declare a CnmcBase, first call init() to initialize
   the distance and predecessor tables, then call check() to
   check if there is any unreachable nodes.
   Call set_mark() to mark any nodes that shouldn't be in a multicast
   group if necessary.
*/
template < class GT,
		 class WT1 = int, class WT2 = double, class WT3 = int >
//class WT1, class WT2, class WT3>
class CnmcBase
{

public:
	const GT &m_gra;
	int m_nSize;

	// path weight 1 (routing weight)
	std::vector<WT1> m_vW1;
	// path weight 2 (additional end-to-end weight, say, delay)
	std::vector<WT2> m_vW2;
	// path weight 3 (additional end-to-end weight, say, hop-count)
	std::vector<WT3> m_vW3;

	// [i*n+j]: previous hop of j on the shortest path from i to j
	std::vector<int> m_vPred;

	// marks to determine if a node can be multicast group member
	std::vector<bool> m_vMarks;

	double m_dMax;
public:

	/**
	@param gra the graph, should be packed before passed.
	*/
	CnmcBase( const GT &gra, double dmax = 10000 ):
		m_gra( gra ), m_nSize( gra.size() ),
		m_vW1( m_nSize *m_nSize, ( WT1 )dmax ),
		m_vW2( m_nSize *m_nSize, ( WT2 )dmax ),
		m_vW3( m_nSize *m_nSize, ( WT3 )dmax ),
		m_vPred( m_nSize *m_nSize, -1 ),
		m_vMarks( m_nSize, true ), m_dMax( dmax )
	{ }

	~CnmcBase()
	{}

	CnmcBase( const CnmcBase<GT, WT1, WT2, WT3 > &rhs ):
		m_gra( rhs.m_gra ), m_nSize( m_nSize ),
		m_vW1( rhs.m_vW1 ), m_vW2( rhs.m_vW2 ), m_vW3( rhs.m_vW3 ),
		m_vPred( rhs.m_vPred ), m_vMarks( rhs.m_vMarks ),
		m_dMax( rhs.m_dMax )
	{ }

	const CnmcBase &operator=( const CnmcBase<GT, WT1, WT2, WT3> &rhs ) {
		if( this != &rhs ) {
			m_gra = rhs.m_gra;
			m_nSize = rhs.m_nSize;
			m_vW1 = rhs.m_vW1;
			m_vW2 = rhs.m_vW2;
			m_vW3 = rhs.m_vW3;
			m_vPred = rhs.m_vPred;
			m_vMarks = rhs.m_vMarks;
			m_dMax = rhs.m_dMax;
		}
		return *this;
	}

	int size( void ) const {
		return m_nSize;
	}

	WT1 w1( int i, int j ) const {
		return m_vW1[i*m_nSize+j];
	}

	WT2 w2( int i, int j ) const {
		return m_vW2[i*m_nSize+j];
	}

	WT3 w3( int i, int j ) const {
		return m_vW3[i*m_nSize+j];
	}

	bool can_be_member( int i ) const {
		if( i < 0 || i >= m_nSize ) {
			return false;
		}
		return m_vMarks[i];
	}

	bool mark( int i ) const {
		if( i < 0 || i >= m_nSize ) {
			return false;
		}
		return m_vMarks[i];
	}

	void set_mark( int i, bool b = false ) {
		if( i < 0 || i > m_nSize ) {
			return ;
		}
		m_vMarks[i] = b;
	}

	bool get_path( int ns, int nt, afg::CPath &lp ) const {
		int n = m_nSize;
		if( nt >= n || nt < 0 || ns < 0 || ns >= n ) {
			return false;
		}
		if( nt == ns ) {
			lp.push_front( ns );
			return true;
		}

		int i = nt, ncount = 0;
		while( i >= 0 && ncount++ < n ) {
			lp.push_front( i );
			i = m_vPred[ns*n+i];
			if( i == ns ) {
				lp.push_front( ns );
				return true;
			}
		}

		return false;
	}

	/** initialization.
	Initialize the inter-member distance(weight) and predecessor tables.
	1st weight is the underlying unicast routing weight (shortest path),
	2nd weight is an end-to-end measure that can be used for TM routing,
	3rd weight is another end-to-end measure.
	*/
	template<class Fun1, class Fun2, class Fun3>
	void init( Fun1 fw1, Fun2 fw2, Fun3 fw3 ) {
		int i, j, k, n = m_nSize;
		//std::cout<<m_nSize<<", "<<m_vW1.size()<<std::endl;

		// run a Floyd-Warshall algorithm
		typename GT::const_e_iterator ite;
		for( i = 0; i < n; ++i ) {
			//cout<<"r1: "<<i<<endl;
			m_vW1[i *n+i] = ( WT1 )0;
			m_vW2[i *n+i] = ( WT2 )0;
			m_vW3[i *n+i] = ( WT3 )0;
			for( ite = m_gra.e_begin( i ); ite != m_gra.e_end( i ); ++ite ) {
				m_vW1[i*n+ite->to()] = fw1( &( ite->edge_d() ) );
				m_vW2[i*n+ite->to()] = fw2( &( ite->edge_d() ) );
				m_vW3[i*n+ite->to()] = fw3( &( ite->edge_d() ) );
				m_vPred[i*n+ite->to()] = i;
				//cout<<i<<"->"<<ite->to()<<": "<<fw1( &(*ite))<<", "<<fw2( &(*ite) )<<", "<<fw3(&(*ite))<<endl;
			}
		}
		for( k = 0; k < n; ++k ) {
			//cerr<<"r2: "<<k<<endl;
			for( i = 0; i < n; ++i )
				for( j = 0; j < n; ++j )
					if( j != i ) {
						if( m_vW1[i*n+k] + m_vW1[k*n+j] < m_vW1[i*n+j] ) {
							//cout<<"update "<<i<<"->"<<j<<": "<<k<<", "
							//<<m_vW1[i*n+k]+m_vW1[k*n+j]<<", "
							//<<m_vW2[i*n+k]+m_vW1[k*n+j]<<", "
							//<<m_vW3[i*n+k]+m_vW1[k*n+j]<<endl;
							m_vW1[i *n+j] = m_vW1[i*n+k] + m_vW1[k*n+j];
							m_vW2[i *n+j] = m_vW2[i*n+k] + m_vW2[k*n+j];
							m_vW3[i *n+j] = m_vW3[i*n+k] + m_vW3[k*n+j];
							m_vPred[i *n+j] = m_vPred[k*n+j];
						}
					}
		}

	}

	friend std::ostream &operator<<( std::ostream &os,
									 const CnmcBase<GT, WT1, WT2, WT3>& tmg ) {
		os << tmg.m_gra;
		int n = tmg.m_nSize;
		for( int i = 0; i < n; i++ ) {
			os << i << "(" << tmg.m_vMarks[i] << ")->: ";
			for( int j = 0; j < n; j++ ) {
				if( j != i ) {
					os << j << '(' << tmg.m_vPred[i*n+j] << ", " << tmg.m_vW1[i*n+j]
					   << ", " << tmg.m_vW2[i*n+j]
					   << ", " << tmg.m_vW3[i*n+j] << "), ";
				}
			}
			os << std::endl;
		}
		return os;
	}

	/** check to see if there is any unreachable nodes or
	  inter-node distances that are irregular.
	@return true if no such node(s), false otherwise
	*/
	bool check( void ) const {
		int n = m_nSize, i, j, ntotal = 0;
		for( i = 0; i < n; i++ ) {
			//os<<i<<"->: ";
			for( j = 0; j < n; j++ )
				if( j != i ) {
					if( m_vPred[i*n+j] < 0 ||
						m_vW2[i *n+j] >= m_dMax ||
						m_vW3[i *n+j] >= m_dMax ||
						m_vW2[i*n+j] < 0 || m_vW3[i*n+j] < 0 ) {
						++ntotal;
						//cout<<i<<"->"<<j<<": "<<m_vW1[i*n+j]<<", "<<m_vW2[i*n+j]
						//    <<", "<<m_vW3[i*n+j]<<endl;
					}
				}
		}

		if( ntotal > 0 ) {
			return false;
		}
		return true;
	}
}; // end of class CnmcBase

/**@}*/

} // end of namespace

#endif
