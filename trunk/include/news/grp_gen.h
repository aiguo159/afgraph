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


/**@file grp_gen.h

  generate groups
*/

#ifndef _NEWS_GRP_GEN_H_
#define _NEWS_GRP_GEN_H_

#include <list>
#include <set>

#include "afl/rand.hpp"

namespace news
{

/**@defgroup grpgen utilities to generate groups for multicast simulation
@ingroup news
*/
/**@{*/

/** generate a sequence of node addition/deletion actions for
  a multicast group.
BT: a class that provides at least two interfaces "int size()" and
  "bool can_be_member(int i)". The 1st one returns the number of nodes
  in a graph, and the 2nd determines if a node of index i can be
  a multicast group member. For example, CnmcBase<...> (research/nmcbase.h)
  or something derived from it.
Reference (paper):

@param base an instance of BT that provides size() function and
  can_be_member(int i) function that determines if node i can be a multicast
  group member.
@param alpha percentage of nodes that a group can have in average;
  i.e., at steady state, average number of group members(including source
  node ns) is base.size()*alpha.
@param ns source node of a group, it will never be deleted from the group.
@param nt number of actions to generate after an initial sequence of
  additions that brings the number of members to the average number.
@param lact a list of actions; for every pair in the list, the 1st integer
  number represents the action, 1: add, -1: delete; the 2nd integer
  (>=0, <base.size() ) is the index of the node for the action.
  First a sequence of additions will be generated until the number of group
  members reaches the average, then addition/deletion
@param sdest set of destination nodes in the group (not including ns);
  initially it is the set of current destination nodes in the group;
  when this procedure returns, it is the set of destination nodes that
  are in the group after the list of actions.
*/
template<class BT>
void rand_seq_gen( const BT &base, double alpha, int ns, int nt,
				   std::list<std::pair<int, int> >& lact,
				   std::set<int>& sdest )
{
	int n = base.size();
	int n2 = n * alpha;
	int ntmp;
	while( (int)sdest.size() < n2 - 1 ) {
		ntmp = ( n - 1 ) * afl::rand::gen();
		if( ntmp != ns && sdest.find( ntmp ) == sdest.end()
			&& base.can_be_member( ntmp ) ) {
			sdest.insert( ntmp );
			lact.push_back( std::pair<int, int>( 1, ntmp ) );
		}
	}

	double dtmp, dpck;
	int i, j;
	std::set
	<int>::iterator isn;
	for( i = 0; i < nt; ++i ) {
		dpck = alpha * ( n - 1 - sdest.size() ) / ( alpha * ( n - 1 - sdest.size() )
				+ ( 1 - alpha ) * ( 1 + sdest.size() ) );
		dtmp = afl::rand::gen();
		if( dtmp < dpck ) {
			// add a node
			while( true ) {
				ntmp = ( n - 1 ) * afl::rand::gen();
				if( ntmp != ns && sdest.find( ntmp ) == sdest.end()
					&& base.can_be_member( ntmp ) ) {
					sdest.insert( ntmp );
					lact.push_back( std::pair<int, int>( 1, ntmp ) );
					break;
				}
			}
		} else {
			// randomly pick a node to delete
			if( sdest.size() < 1 ) {
				--i;
			} else {
				isn = sdest.begin();
				ntmp = ( sdest.size() - 1 ) * afl::rand::gen();
				for( j = 0; j < ntmp; ++j ) {
					++isn;
				}
				lact.push_back( std::pair<int, int>( -1, *isn ) );
				sdest.erase( isn );
			}
		}
	}
}

/** generate a constant group size.
*/
struct CConstGrpSz {
	CConstGrpSz( int nsize )
		: m_nSize( nsize )
	{}

	int operator()( void ) const {
		return m_nSize;
	}

	int gen_size( void ) const {
		return m_nSize;
	}

	int m_nSize;

};

/** generate uniformly distributed group size, from nmin to nmax.
*/
struct CUniformGrpSz {
	CUniformGrpSz( int nmax, int nmin = 2 )
		: m_nmin( nmin ), m_nmax( nmax )
	{}

	int operator()( void ) const {
		//return (int)afl::rand::gen( m_nmin, m_nmax+1.0 );
		return gen_size();
	}

	int gen_size( void ) const {
		int n = ( int )afl::rand::gen( m_nmin, m_nmax + 1.0 );

		// unlikely, may never depending on random number generator
		// just be safe, "bullet-proof"
		if( n == m_nmax + 1 ) {
			n = m_nmax;
		}

		return ( int )afl::rand::gen( m_nmin, m_nmax + 1.0 );
	}

	int m_nmin, m_nmax;
};

/** interface of group generator.
*/
struct CGrpGenerator {
	/** generate a group.
	@param ns source node of generated group
	@param smem member nodes (indices) of generated group
	@return number of nodes in group, 0 means failed.
	*/
	virtual int gen_grp( int &ns, std::set
						 <int>& smem ) = 0;

	/** operator () -- so this class can be used as a function object.
	@see gen_grp
	*/
	int operator()( int &ns, std::set
					<int>& smem ) {
		return gen_grp( ns, smem );
	}
};

/** generate a random group of size generated by a group size generator.
GrpSzG: group size generator.
A group is generated by randomly picking up certain number (size) of
  nodes of indices between 0 and the upper bound.
  The size is generated by the group size generator.
*/
template<class GrpSzG>
class CRandGrpX: public CGrpGenerator
{
public:
	/** constructor.
	@param nmax upper bound of node index allowed in the group,
	  i.e, any node in the group is going to have an index from 0 to nmax
	  (inclusive).
	@param gsg group size generator
	*/
	CRandGrpX( int nmax, const GrpSzG &gsg )
		: m_nMax( nmax ), m_grpszgen( gsg )
	{}

	/** generate a group.
	@param ns source node of generated group
	@param smem member nodes (indices) of generated group
	@return number of nodes in group, 0 means failed.
	*/
	int gen_grp( int &ns, std::set
				 <int>& smem ) {
		int n = m_grpszgen();
		if( n <= 0 || n > m_nMax + 1 ) { // illegal group size
			return 0;
		}

		while( smem.size() < n ) {
			int nx = afl::rand::gen( 0, m_nMax + 1 );
			if( nx == m_nMax + 1 ) {
				// unlikely, just be safe
				nx = m_nMax;
			}
			smem.insert( nx );
		}

		// the following will randomly pick one node as source
		int nx = afl::rand::gen( 0, smem.size() );

		// unlikely, may never happen depending on random number generator,
		// just be safe
		if( nx == smem.size() ) {
			nx = smem.size() - 1;
		}

		std::set
		<int>::iterator it = smem.begin();
		for( int i = 0; i < nx; ++i ) {
			++it;
		}
		ns = *it;

		return smem.size();
	}

private:
	int m_nMax;
	GrpSzG m_grpszgen; // group size generator
};

/** generate random groups.
A group is generated by randomly picking up nodes of indices between 0
  and a given upper bound according to certain probability.
  Group size is enforced to be larger than a minimum.
*/
class CRandGrpG: public CGrpGenerator
{
public:
	/**
	@param dp probability according to which to pick up nodes
	@param nmax upper bound on node index allowed, inclusive
	@param nmin minimum group size
	*/
	CRandGrpG( double dp, int nmax, int nmin = 2 )
		: m_dp( dp ),  m_nMax( nmax ), m_nMin( nmin )
	{}

	/** generate a group.
	@param ns source node of generated group
	@param smem member nodes (indices) of generated group
	@return number of nodes in group, 0 means failed.
	*/
	int gen_grp( int &ns, std::set
				 <int>& smem ) {
		double p;
		int ntrial = 0;
		while( (int)smem.size() < m_nMin && ntrial++ < m_nMax ) {
			smem.clear();
			for( int i = 0; i < m_nMax + 1; ++i ) {
				p = afl::rand::gen( 1.0 );
				if( p < m_dp ) {
					smem.insert( i );
				}
			}
		}
		if( (int)smem.size() < m_nMin ) {
			return 0;
		}

		// the following will randomly pick one node as source
		int nx = ( int )afl::rand::gen( 0, smem.size() );

		// unlikely, may never happen depending on random number generator,
		// just be safe
		if( nx == (int)smem.size() ) {
			nx = smem.size() - 1;
		}

		std::set<int>::iterator it = smem.begin();
		for( int i = 0; i < nx; ++i ) {
			++it;
		}
		ns = *it;

		return smem.size();
	}

private:
	double m_dp;
	int m_nMax; // upper bound on node index
	int m_nMin; // minimum number of nodes in a group
};

/** generate random groups.
A group is generated by randomly picking up nodes of indices from
  a given graph according a certain probability.
Group size is enforced to be larger than a minimum.
An index is checked to make sure it is a valid one in the graph before
  it is picked-- this makes it possible to generate
  groups for a graph having "holes" in its index range; for graph
  without such "holes", use CRandGrpG instead (specifying group size
  as the upper bound on index).
*/
template<class GraphT>
class CRandGrpG2: public CGrpGenerator
{
public:
	CRandGrpG2( const GraphT &gra, double dp, int n1 = 2 )
		: m_gra( gra ), m_dp( dp ),  m_nMin( n1 )
	{}

	/** generate a group.
	@param ns source node of generated group
	@param smem member nodes (indices) of generated group
	@return number of nodes in group, 0 means failed.
	*/
	int gen_grp( int &ns, std::set
				 <int>& smem ) {
		double p;
		int ntrial = 0;
		while( smem.size() < m_nMin && ntrial++ < m_gra.range() ) {
			smem.clear();
			for( int i = 0; i < m_gra.range(); ++i )
				if( m_gra.is_in_use( i ) ) {
					p = afl::rand::gen( 1.0 );
					if( p < m_dp ) {
						smem.insert( i );
					}
				}
		}
		if( smem.size() < m_nMin ) {
			return 0;
		}

		// the following will randomly pick one node as source
		int nx = afl::rand::gen( 0, smem.size() );

		// unlikely, may never happen depending on random number generator,
		// just be safe
		if( nx == smem.size() ) {
			nx = smem.size() - 1;
		}

		std::set<int>::iterator it = smem.begin();
		for( int i = 0; i < nx; ++i ) {
			++it;
		}
		ns = *it;

		return smem.size();
	}

private:
	const GraphT &m_gra;
	double m_dp;
	int m_nMin; // minimum number of nodes in a group
};

/** generate group from a "node-weighted" graph.
 In this graph, each vertex has a weight within [0,1]
 and provides a weight() function (as idwVertex) to access that weight.
 GraphT: graph type.
 GrpSzG: group size generator.
 */
template<class GraphT, class GrpSzG>
class CWeightedGrpG: public CGrpGenerator
{
public:
	CWeightedGrpG( const GraphT &gra, const GrpSzG &gsg )
		: m_gra( gra ), m_grpszgen( gsg )
	{}

	int gen_grp( int &ns, std::set
				 <int>& smem ) {
		// first generate a group size
		int n = m_grpszgen();

		double p;
		int nx;
		while( smem.size() < n ) {
			// randomly pick up a non-member node
			nx = ( int )afl::rand::gen( 0, m_gra.range() );
			if( m_gra.is_valid( nx ) &&
				std::find( smem.begin(), smem.end(), nx ) == smem.end() ) {
				// determine if this node should be added
				p = afl::rand::gen( 1.0 );
				if( p < m_gra[nx].vertex_d().weight() ) {
					smem.insert( nx );
				}
			}
		}

		// the following will randomly pick one node as source
		nx = ( int )afl::rand::gen( 0, smem.size() );

		// unlikely, may never depends on random number generator, just be safe
		if( nx == smem.size() ) {
			nx = smem.size() - 1;
		}

		std::set<int>::iterator it = smem.begin();
		for( int i = 0; i < nx; ++i ) {
			++it;
		}
		ns = *it;

		return smem.size();
	}

private:
	const GraphT &m_gra;
	GrpSzG m_grpszgen; // group size generator
};

/**@}*/

} // end of namespace news

#endif
