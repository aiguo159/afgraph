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


/* tshortest_path.cpp

  test various shortest path algorithm implemented
    in shortest_path.h

  Dijkstra's algorithm, Floyd_Warshall algorithm

  Aiguo Fei

  1999, 2000, 2001
*/

#include <iostream>
#include <vector>
#include <functional>

#include "afl/util_tl.hpp"

#include "afgraph/edge.h"
#include "afgraph/path.h"
#include "afgraph/graph.h"
#include "afgraph/graph_alg.h"
#include "afgraph/shortest_path.h"

using namespace std;
using namespace afl;
using namespace afg;

int main()
{
	CGraph<char, int> gra2( 4, 'z' );
	gra2.insert_v( 'a' );
	gra2.insert_v( 'b' );
	gra2.insert_v( 'c' );
	gra2.insert_v( 'd' );
	gra2.insert_2e( 'a', 'b', 2 );
	gra2.insert_2e( 'b', 'c', 3 );
	gra2.insert_2e( 'c', 'd', 4 );
	gra2.insert_2e( 'd', 'a', 5 );
	std::cout << "graph size now: " << gra2.size()
			  << ", range: " << gra2.range()
			  << ", graph now 107: " << std::endl << gra2 << std::endl;

	std::cout << "connected?: " << is_connected( gra2 ) << std::endl;

	vector<int> vpred( gra2.size() );
	vector<int> vdist( gra2.size() );

	dijkstra<CGraph<char, int> >(
		gra2, 0, afl::pointer2value<int>(), 100, vpred, vdist );

	int i;
	for( i = 0; i < 4; ++i ) {
		cout << i << ": " << vdist[i] << ", " << vpred[i] << endl;
	}

	CPath lp;
	cout << "find path from node 0 to 2 result: "
		 << dijkstra_t<CGraph<char, int> >(
			 gra2, 0, 2, afl::pointer2value<int>(), 100, lp )
		 << endl << "path: " << lp << endl;


	const CGraph<char, int>& gra3 = gra2;

	dijkstra<CGraph<char, int> >(
		gra3, 0, afl::pointer2value<int>(), 100, vpred, vdist );

	for( i = 0; i < 4; ++i ) {
		cout << i << ": " << vdist[i] << ", " << vpred[i] << endl;
	}

	CPath lp2;
	cout << "find path from node 0 to 2 result: "
		 << dijkstra_t<CGraph<char, int> >(
			 gra3, 0, 2, afl::pointer2value<int>(), 100, lp2 )
		 << endl << "path: " << lp2 << endl;

	// all shortest path
	int n = gra3.range();
	vector<int> vapred( n * n );
	vector<int> vadist( n * n );
	cout << "find all sp: "
		 << floyd_warshall_allsp<CGraph<char, int> >
		 ( gra3, afl::pointer2value<int>(), 100, vapred, vadist )
		 << endl;
	copy( vapred.begin(), vapred.end(), ostream_iterator<int>( std::cout, " " ) );
	cout << endl;
	copy( vadist.begin(), vadist.end(), ostream_iterator<int>( std::cout, " " ) );
	cout << endl;

	return 0;
}
