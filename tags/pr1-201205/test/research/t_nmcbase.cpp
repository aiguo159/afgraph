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


/*@fname t_nmcbase.cpp

  Aiguo Fei
  2000, 2001
*/

#include <iostream>
#include <fstream>

#include "afl/util_tl.hpp"
#include "afgraph/graph.h"
#include "afgraph/edge.h"
#include "afgraph/graph_convert.h"

#include "research/import_gitalt.h"
#include "afgraph/tree_alg.h"

#include "research/nmcbase.h"

using namespace std;
using namespace afg;

int main()
{
	typedef CGraph<tsVertex, int> GT;

	// load a GIT graph file in alt format
	int nsize = 100;
	GT gts1( nsize, -1 );
	std::ifstream fsfrom( "ts100-0.alt" );
	if( fsfrom ) {
		import_gitalt_ts( gts1, fsfrom );
	}

	gts1.pack();
	cout << "graph: " << gts1.size() << endl << gts1 << endl;

	// convert it by adding inter-neighbor distance as another edge weight
	typedef CGraph< tsVertex, CEdgeW2<int, double> > nGT;
	nGT ngts1( gts1.range() );
	graph_convert< GT, nGT, viConverter<tsVertex, tsVertex >, e_add_dist<GT, double> >
	( gts1, ngts1, viConverter<tsVertex, tsVertex >(),
	  e_add_dist< GT, double>( gts1 ) );

	CnmcBase<nGT> tmcbase( ngts1 );

	tmcbase.init( std::mem_fun( &CEdgeW2<int, double>::weight1 ),
				  std::mem_fun( &CEdgeW2<int, double>::weight2 ),
				  afl::runit_p< CEdgeW2<int, double> >() );

	cout << "check: " << tmcbase.check() << endl;

	// mark nodes
	for( int ix = 0; ix < ngts1.size(); ++ix )
		if( ngts1[ix].vertex_d().type() == 'T' ) {
			tmcbase.set_mark( ix, false );
		}

	cout << "\ntmcbase: \n" << tmcbase << endl;

	int i = 42, j = 34;
	cout << i << "->" << j << ": "
		 << tmcbase.m_vW1[i * ngts1.size() + j] << ", "
		 << tmcbase.m_vW2[i * ngts1.size() + j] << ", "
		 << tmcbase.m_vW3[i * ngts1.size() + j] << endl;
	CPath lp;
	tmcbase.get_path( i, j, lp );
	cout << "path: " << lp << endl;

	return 0;
}
