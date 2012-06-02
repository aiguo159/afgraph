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


/**@file graph_convert0.h

  Some old graph convertors.

  Aiguo Fei

  UCLA, 1999-2001
*/

#ifndef _AFG_GRAPH_CONVERT0_H_
#define _AFG_GRAPH_CONVERT0_H_

#include "afl/rand.hpp"

#include "gtypes.h"

/* Before including this file, make sure the header file, in which graph_type
    (required by gtypes.h) is defined, is included first.
    For example, graph.h.
*/

namespace afg
{

/**@addtogroup gconvert
*/

/**@{*/

/* the following two were written in 1999. The were obsoleted
  by facilities in graph_convert.h.
*/

/** graph of single weight -> two double weights.
    In new graph, each edge has two weights: one is the original,
    one is a random number between 0 and 1.
    ngra should be cleared fist (or newly created without inserting any vertex)
    before passed to this function.
    VT is the type of vertex data and WT1 is the type of original weight.
    @param gra0 original graph
    @param ngra new graph

    Note: obsolete.
*/
template<class VDT, class WT1 >
void grfw2grfw2( const graph_type<VDT, WT1>& gra0,
				 graph_type<VDT, CEdgeW2<WT1, double> >& ngra )
{
	int n = gra0.range();
	ngra.reserve( n );
	int i;
	for( i = 0; i < n; ++i ) {
		ngra.insert_v_ati( gra0.vertex_d( i ), i );
	}

	typename graph_type<VDT, WT1>::const_e_iterator it;
	for( i = 0; i < n; ++i ) {
		for( it = gra0.e_begin( i ); it != gra0.e_end( i ); ++it )
			ngra.insert_e_byi( i, it->to( ),
							   CEdgeW2<WT1, double>( it->edge_d( ),
									   afl::rand::gen( 1.0 ) ) );
	}
}

/** graph of single weight -> two double weights.
    Edges of original graph are assumed to be symmetric (edge i->j has same
    "edge data" as edge j->i), added new random weight
  @see grfw2grfw2

  Note: obsolete.
*/
template<class VDT, class WT1 >
void grfw2grfw2_s( const graph_type<VDT, WT1>& gra0,
				   graph_type<VDT, CEdgeW2<WT1, double> >& ngra )
{
	int n = gra0.range();
	ngra.reserve( n );
	int i;
	for( i = 0; i < n; ++i ) {
		ngra.insert_v_ati( gra0.vertex_d( i ), i );
	}

	typename graph_type<VDT, WT1>::const_e_iterator it;
	for( i = 0; i < n; ++i ) {
		for( it = gra0.e_begin( i ); it != gra0.e_end( i ); ++it )
			if( it->to( ) > i )
				ngra.insert_2e_byi( i, it->to( ),
									CEdgeW2<WT1, double>( it->edge_d( ),
											afl::rand::gen( 1.0 ) ) );
	}
}

/**@}*/

} // end of namespace afg

#endif
