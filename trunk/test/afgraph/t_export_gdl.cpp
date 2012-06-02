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
#include <fstream>

#include "afgraph/graph.h"
#include "afgraph/graph_input.h"
#include "afgraph/export_gdl.h"

#include "afgraph/graph_gen.h"

using namespace std;

using namespace afg;

using namespace afl;

int main()
{

	CGraph<string, int> gra4( 84, "null" );
	std::ifstream fsfrom4( "mbone-all-199405.vve" );
	if( fsfrom4 ) {
		try {
			input_gra_vve< CGraph<string, int>, string>( gra4, fsfrom4 );
		} catch( afl::general_except<std::string>& ge ) {
			cout << ge.what() << endl;
		}
	}
	fsfrom4.close();
	cout << "graph 4:\n" << gra4 << endl;

	std::ofstream outto( "mbone.gdl" );
	//typedef CGraph<string, int> MT;
	export_gdl<CGraph<string, int> > ogdl;
	if( outto ) {
		ogdl.export_g( outto, gra4 );
	}
	outto.close();

	int n2 = 10;
	typedef T_xydGraph GraphT;
	T_xydGraph grf2( n2, xyVertex<int>( -1, -1 ) );
	afl::rand::init();
	int nlinks = bmw_graph_gen( grf2, 10, n2, 10, 0.5, 0.4 );
	cout << "# links: " << nlinks << endl
		 << "graph:\n" << grf2 << endl;

	/* get some garbage output using cygwin (g++ 2.95.6-2) on Win2k,
	  while ok with mbone above. The most bizarre bug hehavior I have
	  ever seen, and counldn't figure out why and how!
	  This problem also occurs on Linux using g++ 2.91.66, but not on Solaris
	  with the same version of g++.
	*/
	export_gdl<GraphT> ogdl2_0;
	cout << " graph random 2_0:\n";
	ogdl2_0.export_g( cout, grf2 );

	std::ofstream randto( "random.gdl" );
	title_vd<GraphT::iVT> tvd;
	node_xyloc<GraphT::iVT> nloc( 30, 20 );
	export_gdl<GraphT, title_vd<GraphT::iVT>, node_xyloc<GraphT::iVT> >
	ogdl2( tvd, nloc );
	if( randto ) {
		ogdl2.export_g( randto, grf2 );
	}
	randto.close();

	cout << " graph random 2:\n";
	ogdl2.export_g( cout, grf2 );

	set<int> smem;
	smem.insert( 3 );
	smem.insert( 5 );
	smem.insert( 6 );
	smem.insert( 7 );
	title_ind<GraphT::iVT> tind;
	node_color<GraphT::iVT> ncolor( smem );
	export_gdl<GraphT, title_ind<GraphT::iVT>, node_color<GraphT::iVT> >
	ogdl3( tind, ncolor );
	/*export_gdl<GraphT, title_ind<GraphT::iVT>, node_color<GraphT::iVT> >
	    ogdl3( title_ind<GraphT::iVT>(), node_color<GraphT::iVT>( smem) );
	*/
	ogdl3.export_g( cout, grf2 );

	node_xyloc<GraphT::iVT> nxy( 60, 40 );
	node_color<GraphT::iVT> nc( smem );
	typedef node_combine < GraphT::iVT, node_xyloc<GraphT::iVT>,
			node_color<GraphT::iVT> > nxyc_t;
	nxyc_t nxyc( nxy, nc );
	export_gdl<GraphT, title_ind<GraphT::iVT>, nxyc_t >
	ogdl4( tind, nxyc );

	ogdl4.export_g( cout, grf2 );

	ogdl4.set_global( ogdl4.m_sEllipse + ogdl4.m_sNoarrow );
	std::ofstream rand2( "random2.gdl" );
	ogdl4.export_g( rand2, grf2 );
	rand2.close();

	return 0;
}
