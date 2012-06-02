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


/**@file kthsp.h

  the kth shortest path algorithm

  Aiguo Fei, UCLA 2000
*/

#ifndef _AFG_KTHSP_H_
#define _AFG_KTHSP_H_

//#include <iostream>

#include "path.h"
#include "shortest_path.h"

namespace afg
{

/**@addtogroup gspalg
*/

/**@{
*/
/** the kth shortest-path algorithm.
    This class is used to compute the shortest path, the 2nd shortest path,
      the 3rd shortest path, until the kth shortest path,
      from a given source to all destinations.

    Algorithm description: see "Graphs and Algorithms"
      by Michel Gondran and Michel Minoux, pp.63, John Wiley & Sons, 1984

    How to use: after declare an instance, call get_first( )
    to compute the shortest path and it will do some initialization
    at the same time; now the 1st shorest path is available for retrival
    by calling get_path( , 1,  ).
    To get the ith shortest path, have to call get_next( ) (i-1) times
    (after calling get_first( ) ).
    If want to go beyond the initial k, call increase_k( ) to specify a new k value.
*/
template<class GraphT, class Fun >
class CkthSP
{
public:
	/**
	@param thegra graph on which to run the algorithm;
	  a reference to that graph is kept internally, reference graph
	  can't change before running to the ith path wanted (i<=k).
	@param source source node
	@param k desired kth
	@param fw function object used to get edge weight, pass pointer
	@param winfty an "upper bound" value of weight which should be larger
		than the path length of any valid path; used to initialize distance
		to all nodes.
	*/
	CkthSP( const GraphT &thegra, int source, int k, Fun fw,
			typename Fun::result_type winfty ):
		m_gra( thegra ), m_nSource( source ), m_nK( k ),
		f_Weight( fw ), m_wInfty( winfty ),
		m_kCurrent( 0 ), m_nSize( thegra.range() ),
		m_vDist( thegra.range()*( k + 1 ), winfty ),
		m_vp( thegra.range()*thegra.range(), -1 ),
		m_vPred( thegra.range()*( k + 1 ), -1 ),
		m_vPath( thegra.range()*( k + 1 ) )

	{ }

	~CkthSP()
	{}

	///
	typedef typename Fun::result_type  WT;

private:
	const GraphT &m_gra;
	int m_nSource;
	int m_nK;
	Fun f_Weight;
	WT m_wInfty;

private:
	int m_kCurrent;
	int m_nSize;
	std::vector<WT> m_vDist;
	std::vector<int> m_vp;
	std::vector<int> m_vPred;
	std::vector<CPath> m_vPath;

public:

	/** get the first shortest path.
	    This should be the first function to call after constructor.
	*/
	void get_first( ) {
		m_kCurrent = 1;
		int k = m_kCurrent;
		std::vector<WT> vd( m_nSize, m_wInfty );
		std::vector<int> vpred( m_nSize, -1 );
		dijkstra<GraphT, Fun>( m_gra, m_nSource, f_Weight, m_wInfty, vpred, vd );
		m_vDist[m_nSource] = ( WT )0;
		int i, j;
		for( i = 0; i < m_nSize; i++ )
			for( j = 0; j < m_nSize; j++ ) {
				m_vp[m_nSize *i+j] = 0;
			}

		for( j = 0; j < m_nSize; j++ ) {
			m_vDist[m_nSize *k+j] = vd[j];
			if( vpred[j] >= 0 ) {
				m_vp[m_nSize *vpred[j] + j] = 1;
				m_vPred[m_nSize *k+j] = vpred[j];
			}
		}

		for( j = 0; j < m_nSize; j++ ) {
			pred2path( vpred, m_nSource, j, m_vPath[m_nSize*k+j] );
		}
	}

	/// call this one to compute the next shortest path (from ith to (i+1)th ).
	void get_next( ) {
		m_kCurrent++;
		if( m_kCurrent > m_nK ) {
			m_kCurrent--;
			return ;
		}

		int i, j, k = m_kCurrent;
		const typename GraphT::EDT *pedg;

		//cerr<<"k="<<k<<endl;

		for( j = 0; j < m_nSize; j++ ) {
			//bool isupdated=false;
			for( i = 0; i < m_nSize; i++ ) {
				if( i != j ) {
					if(( pedg = m_gra.get_edge( i, j ) ) ) {
						int k2 = m_vp[m_nSize*i+j] + 1;

						//cerr<<j<<": "<<m_vDist[ m_nSize*k+j]<<", i="<<i<<", k2="<<k2<<", "
						//<<m_vDist[ m_nSize*k2 +i ]<<", w="<<f_Weight( pedg)<<endl;

						if( m_vDist[ m_nSize*k+j]
							> m_vDist[ m_nSize*k2 +i ] + f_Weight( pedg ) ) {
							m_vDist[ m_nSize *k+j] = m_vDist[ m_nSize*k2 +i ] + f_Weight( pedg ) ;
							m_vPath[ m_nSize *k+j] = m_vPath[m_nSize*k2+i];
							m_vPath[ m_nSize*k+j].push_back( j );
							m_vPred[ m_nSize *k+j] = i;
							//isupdated=true;

							//cerr<<"update to "<<i<<", "<<m_vDist[ m_nSize*k+j]<<endl;

						}
					}

				}
			}// for (i)
		}// for( j)

		// update m_vp
		for( j = 0; j < m_nSize; j++ )
			if( m_vPred[m_nSize *k+j] >= 0 ) {
				m_vp[ m_nSize*m_vPred[m_nSize*k+j] + j]++;
			}
	}

	/** return the ith shortest path to node nt.
		get_next( ) should have been already called (nk-1) times.
	@param nt destination node
	@param nk the nkth shortest-path to nt
	@param lp the path retrieved
	@return the total "weight" (distance) of the path
	*/
	WT get_path( int nt, int nk, CPath &lp ) {
		if( nt < 0 || nt >= m_nSize || nk < 1 || nk > m_nK || nk > m_kCurrent ) {
			lp.clear();
			return m_wInfty;
		}

		lp = m_vPath[ m_nSize*nk+nt];
		return m_vDist[ m_nSize*nk+nt];
	}

	/// increase k value to newk, no effect if newk<=old k
	bool increase_k( int newk ) {
		if( newk <= m_nK ) {
			return true;
		}

		m_vDist.resize( m_nSize*( newk + 1 ), m_wInfty );
		if(( int )m_vDist.size() < m_nSize*( newk + 1 ) ) {
			return false;    // failed to allocate additional memory
		}

		m_vPath.resize( m_nSize*( newk + 1 ) );
		if(( int )m_vPath.size() < m_nSize*( newk + 1 ) ) {
			return false;
		}

		m_vPred.resize( m_nSize*( newk + 1 ), -1 );
		if(( int )m_vPred.size() < m_nSize*( newk + 1 ) ) {
			return false;
		}

		m_nK = newk;
		return true;
	}

};

/**@}*/

} // end of namespace afg

#endif
