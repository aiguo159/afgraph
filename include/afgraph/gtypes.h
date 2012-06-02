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


/**@file gtypes.h

  some graph related typde definitions

  Aiguo Fei, UCLA 2000
*/


/** This file provides a number of type definitions to save some typing time.
Before including any file that uses types defined in this file, make sure
  the header file(s) in which graph_type and/or rtree_type are defined
  is(are) included first.
  For example, graph.h provides graph_type and
    rtree.h provides rtree_type.
This way, types defined here and other functions/clsses
  that use these tyepes will not be tied to a specific
  graph or rtree type.
*/

#ifndef _AFG_GTYPES_H_
#define _AFG_GTYPES_H_

#include "edge.h"
#include "vertex.h"

namespace afg
{

/**@defgroup gtypes Type Definitions
Type definitions to save some typing.
@ingroup graphd
*/

/**@{*/

/// edge type with 2 double weights
typedef CEdgeW2<double, double> T_d2Edge;

/// edge type with 3 double weights
typedef CEdgeW3<double, double, double> T_d3Edge;

#ifdef _GRAPHDEFINED

/// graph with an integer id only
typedef graph_type<int, Ce_nCare> T_nGraph;

/// graph with an integer id and a double weight
typedef graph_type<int, double> T_ndGraph;

/// graph with integer xyVertex and a double weight
typedef graph_type<xyVertex<int>, double> T_xydGraph;

/// graph with integer xyVertex and an integer weight
typedef graph_type<xyVertex<int>, int> T_xynGraph;

/// graph with an integer index and an edge of two double numbers
typedef graph_type<int, T_d2Edge> T_nd2Graph;

/// graph with integer xyVertex and an edge of two double numbers
typedef graph_type<xyVertex<int>, T_d2Edge> T_xyd2Graph;

/// transit-stub graph
typedef graph_type<tsVertex, int> T_tsGraph;

#endif

#ifdef _RTREEDEFINED

/// rtree with an integer id only
typedef rtree_type<int, Ce_nCare> T_nrTree;

/// rtree with an integer id and a double weight
typedef rtree_type<int, double> T_ndrTree;

/// rtree with integer xyVertex and a double weight
typedef rtree_type<xyVertex<int>, double> T_xydrTree;

/// rtree with integer xyVertex and an integer weight
typedef rtree_type<xyVertex<int>, int> T_xynrTree;

/// rtree with an integer index and an edge of two double numbers
typedef rtree_type<int, T_d2Edge> T_nd2rTree;

/// rtree with integer xyVertex and an edge of two double numbers
typedef rtree_type<xyVertex<int>, T_d2Edge> T_xyd2rTree;

#endif

/**@}*/

} // end of namespace afg

#endif
