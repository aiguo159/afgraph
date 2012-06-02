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



#include <iostream>

#include "afgraph/graph.h"
#include "afgraph/graph_gen.h"

#include "afgraph/graph_convert0.h"
#include "afgraph/graph_convert.h"

using namespace std;
using namespace afg;

int main()
{

	int n = 3, m = 4;
	T_xynGraph grf( n * m, xyVertex<int>( -1, -1 ) );
	grid_graph_gen( grf, n, m );
	cout << "grf: \n" << grf << endl;

	int n2 = 10;
	T_xydGraph grf2( n2, xyVertex<int>( -1, -1 ) );
	afl::rand::init();
	int nlinks = bmw_graph_gen( grf2, 10, n2, 10, 0.5, 0.4 );
	cout << "# links: " << nlinks << endl
		 << "graph 2:\n" << grf2 << endl;

	T_xyd2Graph grf3( n2, xyVertex<int>( -1, -1 ) );
	grfw2grfw2( grf2, grf3 );
	cout << "new graph 3: \n" << grf3 << endl;

	T_xyd2Graph grf4( n2, xyVertex<int>( -1, -1 ) );
	grfw2grfw2_s( grf2, grf4 );
	cout << "new graph 4: \n" << grf4 << endl;

	// add geometric distance as a 2nd weight
	typedef CGraph< xyVertex<int>, CEdgeW2<double, double> > nGT;
	nGT grf5( grf2.range() );
	graph_convert( grf2, grf5,
				   viConverter<xyVertex<int>, xyVertex<int> >(),
				   e_add_dist< T_xydGraph, double>( grf2 ) );
	cout << "grf 5: \n" << grf5 << endl;

	// add a random number as a 2nd weight
	nGT grf6( grf2.range() );
	graph_convert( grf2, grf6,
				   viConverter<xyVertex<int>, xyVertex<int> >(),
				   e_add_rand< double, double>( 1.0, 0 ) );
	cout << "grf 6: \n" << grf6 << endl;

	// add a symmetric random number as a 2nd weight,
	// convert vertex to integer
	CGraph<int, T_d2Edge> grf7( grf2.range() );
	graph_convert( grf2, grf7,
				   v2index<xyVertex<int> >(),
				   e_add_rand_s< double, double>( 1.0, 0 ) );

	cout << "grf 7: \n" << grf7 << endl;

	return 0;
}
