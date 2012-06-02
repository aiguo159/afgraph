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


/* t_graph_alg.cpp

  test some graph algorithms defined in graph_alg.h

  Aiguo Fei

  1999-2001
*/

#include <iostream>
#include <vector>
#include <functional>

#include "afgraph/edge.h"
#include "afgraph/graph.h"
#include "afgraph/graph_alg.h"

#include "afl/util_tl.hpp"

using namespace std;
using namespace afg;

int main()
{
	CGraph<int, int> gra( 8, -1 );
	gra.insert_v( 0 );
	gra.insert_v( 1 );
	gra.insert_v( 2 );

	for( int i = 3; i < 14; ++i ) {
		gra.insert_v( i );
	}

	gra.insert_e( 3, 4, 5 );
	gra.insert_e_byi( 3, 6, 2 );
	gra.insert_2e( 5, 7, 100 );
	gra.insert_2e_byi( 8, 9, 200 );
	std::cout << "graph now: " << std::endl << gra << std::endl;

	gra.insert_e( 3, 4, 11 );
	gra.insert_e( 6, 7, 19 );
	gra.insert_e( 3, 5, 7 );
	gra.insert_e( 4, 5, 2 );
	gra.insert_e( 5, 6, 2 );
	gra.insert_e( 5, 7, 3 );

	gra.insert_v( 5 );
	gra.insert_e( 3, 4, 11 );
	gra.insert_e( 6, 7, 19 );
	gra.insert_e( 3, 5, 7 );
	gra.insert_e( 4, 5, 2 );
	gra.insert_e( 5, 6, 2 );
	gra.insert_e( 5, 7, 3 );

	std::cout << "graph size now: " << gra.size()
			  << ", range: " << gra.size()
			  << ", graph now 106: " << std::endl << gra << std::endl;

	std::cout << "connected?: " << is_connected( gra ) << std::endl;


	CGraph<char, int> gra2( 6, 'z' );
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

	gra2.insert_v( 'e' );
	gra2.insert_v( 'f' );
	gra2.insert_e( 'a', 'e', 4 );
	gra2.insert_e( 'e', 'c', 3 );
	gra2.remove_v( 'f' );
	gra2.remove_v( 'd' );
	std::cout << "graph size now: " << gra2.size()
			  << ", range: " << gra2.range()
			  << ", graph now 111: " << std::endl << gra2 << std::endl;
	std::cout << "connected?: " << is_connected( gra2 ) << std::endl;

	gra2.pack();
	std::cout << "graph size now: " << gra2.size()
			  << ", range: " << gra2.range()
			  << ", graph now 113: " << std::endl << gra2 << std::endl;

	std::cout << "connected?: " << is_connected( gra2 ) << std::endl;

	gra2.remove_e( 'e', 'c' );
	std::cout << "graph size now: " << gra2.size()
			  << ", range: " << gra2.range()
			  << ", graph now 112: " << std::endl << gra2 << std::endl;

	std::cout << "connected?: " << is_connected( gra2 ) << std::endl;

	int ngw = graph_cost( gra2, afl::pointer2value<int>() );
	cout << "graph cost: " << ngw << endl;

	return 0;
}
