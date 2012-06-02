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


/**@file graph_convert.h

  Graph convertors.

  Aiguo Fei

  UCLA, 1999-2001
*/

#ifndef _AFG_GRAPH_CONVERT_H_
#define _AFG_GRAPH_CONVERT_H_

#include <utility>
#include <map>

#include <math.h>

#include "afl/rand.hpp"

namespace afg
{

/**@defgroup gconvert graph convertors
@ingroup gutil
*/

/**@{*/

/** interface of vertex converter.
  Convert a vertex from type VDT1 to VDT2.
*/
template<class VDT1, class VDT2>
struct IvConverter: public std::unary_function<VDT1, VDT2> {

	/** type of the original vertex.
	  New vertex type is result_type as defined in unary_function.
	*/
	typedef VDT1 original_type;

	virtual VDT2 convert( const VDT1 &v1 ) const = 0;

	VDT2 operator()( const VDT1 &v1 ) const {
		return convert( v1 );
	}
};


/** interface of vertex converter with index.
  Convert a vertex from type VDT1 to VDT2, given index in the original
    graph.
*/
template<class VDT1, class VDT2>
struct IviConverter: public std::binary_function<VDT1, int, VDT2> {

	/** type of the original vertex.
	  New vertex type (VDT2) is result_type as defined in binary_function.
	*/
	typedef VDT1 original_type;

	virtual VDT2 convert( const VDT1 &v1, int i ) const = 0;

	VDT2 operator()( const VDT1 &v1, int i ) const {
		return convert( v1, i );
	}
};

/** interface of edge converter.
  Convert an edge from type EDT1 to EDT2.
*/
template<class EDT1, class EDT2>
struct IeConverter: public std::unary_function<EDT1, EDT2> {

	typedef EDT1 original_type;
	typedef EDT2 result_type;

	virtual EDT2 convert( const EDT1 &e1 ) const = 0;

	/** () operator.
	@param e1 original edge
	@return converted edge of type EDT2
	*/
	EDT2 operator()( const EDT1 &e1 ) const {
		return convert( e1 );
	}
};

/** interface of edge converter with indices.
  Convert an edge from type EDT1 to EDT2, indices of source/destination
    nodes are passed.
  This can also be used a default converter which just do an explicit
    type conversion.
*/
template<class EDT1, class EDT2>
struct IeiConverter {

	typedef EDT1 original_type;
	typedef EDT2 result_type;

	virtual EDT2 convert( const EDT1 &e1, int i, int j ) {
		return ( EDT2 )e1;
	}

	/** () operator.
	@param e1 original edge
	@param i originating node
	@param j destination node
	@return converted edge
	*/
	EDT2 operator()( const EDT1 &e1, int i, int j ) {
		return convert( e1, i, j );
	}
};

/** vertex converter that just does an explicit type conversion.
*/
template<class VDT1, class VDT2>
struct vConverter: public IvConverter<VDT1, VDT2> {
	VDT2 convert( const VDT1 &v1 ) const {
		return ( VDT2 )v1;
	}
};

/** vertex converter that just makes a copy.
*/
template<class VDT>
struct vCopier: public vConverter<VDT, VDT> {
	VDT convert( const VDT &v1 ) const {
		return v1;
	}
};

/** vertex converter with index, just does an explicit type conversion.
*/
template<class VDT1, class VDT2>
struct viConverter: IviConverter<VDT1, VDT2> {
	VDT2 convert( const VDT1 &v1, int ) const {
		return ( VDT2 )v1;
	}
};

/** vertex converter with index, just makes a copy.
*/
template<class VDT>
struct viCopier: public viConverter<VDT, VDT> {
	VDT convert( const VDT &v1, int ) const {
		return v1;
	}
};

/** edge converter that just does an explicit type conversion.
*/
template<class EDT1, class EDT2>
struct eConverter: IeConverter<EDT1, EDT2> {
	EDT2 convert( const EDT1 &e1 ) {
		return ( EDT2 )e1;
	}
};

/** edge converter that just makes a copy.
*/
template<class EDT>
struct eCopier: eConverter<EDT, EDT> {
	EDT convert( const EDT &e1 ) {
		return e1;
	}
};

/** edge converter with indices, just do an explicit type conversion.
*/
template<class EDT1, class EDT2>
struct eiConverter: public IeiConverter<EDT1, EDT2> {
	EDT2 convert( const EDT1 &e1, int, int ) {
		return ( EDT2 )e1;
	}
};

/** edge converter with indices, that just makes a copy.
*/
template<class EDT>
struct eiCopier: eiConverter<EDT, EDT> {
	EDT convert( const EDT &e1, int, int ) {
		return e1;
	}
};

/** convert a graph of tyep GT1 to type GT2.
@param g1 original graph
@param g2 converted graph, should be empty before being passed.
@param f_vc a vertex converter with index, providing interface IviConverter
@param f_ec an edge converter with indices, providing interface IeiConverter
*/
template < class GT1, class GT2,
		 class Vconvert, class Econvert >
void graph_convert( const GT1 &g1, GT2 &g2,
					Vconvert f_vc, Econvert f_ec )
{
	int i = 0;
	g2.reserve( g1.range() );
	for( i = 0; i < g1.range(); ++i ) {
		if( g1.is_in_use( i ) ) {
			g2.insert_v_ati( f_vc( g1[i].vertex_d(), i ), i );
		}
	}
	typename GT1::const_e_iterator eit;
	typename GT2::EDT edge2;
	for( i = 0; i < g1.range(); ++i ) {
		for( eit = g1.e_begin( i ); eit != g1.e_end( i ); ++eit ) {
			edge2 = f_ec( eit->edge_d(), i, eit->to() );
			g2.insert_e_byi( i, eit->to(), edge2 );
		}
	}
}

/** convert a vertex to a new int vertex using its original index.
 */
template<class VDT1>
struct v2index: public IviConverter<VDT1, int> {
	int convert( const VDT1 &, int i ) const {
		return i;
	}
};

/** add geometric distance as a new edge weight.
  The original graph is passed to the constructor, and the graph
    is assumed to have vertex which contains x,y coordinates
    accessible through x() and y().
  New edge has two weights: the original one and the new distance.
  GT: original graph type, DT: distance type
*/
template<class GT, class DT>
struct e_add_dist: public IeiConverter < typename GT::EDT,
		CEdgeW2<typename GT::EDT, DT> > {

	const GT &m_gra;

	e_add_dist( const GT &gra ): m_gra( gra )
	{ }

	typedef IeiConverter < typename GT::EDT, CEdgeW2<typename GT::EDT, DT> >
	converter_base_type;

	typename converter_base_type::result_type convert(
		const typename converter_base_type::original_type &e1, int i, int j ) {
		return typename converter_base_type::result_type(
				   e1,
				   ( DT )sqrt(( double )(( m_gra[i].vertex_d().x() - m_gra[j].vertex_d().x() )
										 * ( m_gra[i].vertex_d().x() - m_gra[j].vertex_d().x() ) +
										 ( m_gra[i].vertex_d().y() - m_gra[j].vertex_d().y() )
										 * ( m_gra[i].vertex_d().y() - m_gra[j].vertex_d().y() ) ) ) );
	}
};

/** add random number as a new edge weight.
  New edge has two weights: the original one and a random number.
  EDT1: original edge type;
  DT: random number type (e.g., double or int).
*/
template<class EDT1, class DT>
struct e_add_rand: public IeiConverter<EDT1, CEdgeW2<EDT1, DT> > {

	// range of the random number
	double m_max, m_min;

	e_add_rand( double max = 1, double min = 0 ):
		m_max( max ), m_min( min )
	{ }

	typedef IeiConverter < EDT1, CEdgeW2<EDT1, DT> > converter_base_type;

	typename converter_base_type::result_type convert( const EDT1 &e1, int, int ) {
		return typename converter_base_type::result_type( e1, ( DT )afl::rand::gen( m_min, m_max ) );
	}
};

/** add symmetrical random number as a new edge weight.
  New edge has two weights: the original one and a random number.
  New weight is symmetrical: w(i->j)=w(j->i)
  EDT1: original edge type;
  DT: random number type (e.g., double or int).
*/
template<class EDT1, class DT>
struct e_add_rand_s: public IeiConverter<EDT1, CEdgeW2<EDT1, DT> > {

	typedef IeiConverter < EDT1, CEdgeW2<EDT1, DT> > converter_base_type;

	// range of the random number
	double m_max, m_min;

	std::map< std::pair<int, int>, DT > m_cache;

	/// clear previously stored random numbers
	void clear_cache( void ) {
		m_cache.clear();
	}

	e_add_rand_s( double max = 1.0, double min = 0.0 ):
		m_max( max ), m_min( min )
	{ }

	typename converter_base_type::result_type convert(
		const typename converter_base_type::original_type &e1, int i, int j ) {
		typename std::map< std::pair<int, int>, DT >::iterator im
		= m_cache.find( std::pair<int, int>( j, i ) );
		if( im != m_cache.end() ) {
			return typename converter_base_type::result_type( e1, im->second );
		}

		DT r = ( DT )afl::rand::gen( m_min, m_max );
		m_cache[std::pair<int, int>( i, j )] = r;

		return typename converter_base_type::result_type( e1, r );
	}
};

/**@}*/

} // end of namespace afg

#endif
