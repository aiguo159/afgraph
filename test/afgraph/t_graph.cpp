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


/* t_graph.cpp

  test graph class defined in graph.h

  Aiguo Fei

    1999-2000

*/

#include <iostream>
#include <vector>
#include <functional>

#include "afgraph/graph.h"

using namespace std;
using namespace afg;

template<class ET, class Fun>
void outputw( ET *pe, Fun fw )
{
	std::cout << "weight of edge: " << fw( pe ) << std::endl;
}

int main()
{
	CiVertex<int, int> vx0;
	CiVertex<int, int> vx1( 101 );
	CiVertex<int, int> vx2( vx1 );
	cout << vx0 << ", " << vx1 << ", " << vx2 << endl;
	vx0 = vx1;
	cout << vx0 << ", " << vx1 << endl;

	CGraph<int, int> gra( 8, -1 );

	gra.insert_v( 0 );
	gra.insert_v( 1 );
	gra.insert_v( 2 );

	cout << "vertex data at 1: " << gra.vertex_d( 1 ) << ", " << gra[1].vertex_d() << endl;
	cout << "vertex at 1: " << gra.at( 1 ) << endl;
	std::cout << "vertex 1: " << gra[1] << std::endl;
	cout << "is in use 1: " << gra.is_in_use( 1 ) << endl;
	cout << "is in use 5: " << gra.is_in_use( 5 ) << endl;

	std::cout << "size: " << gra.size() << std::endl;
	std::cout << "range: " << gra.range() << std::endl;
	std::cout << "index for vertex 1: " << gra.find_index( 1 ) << std::endl;
	std::cout << "index for vertex 5: " << gra.find_index( 5 ) << std::endl;
	std::cout << "graph now: " << std::endl << gra << std::endl;

	for( int i = 3; i < 14; ++i ) {
		gra.insert_v( i );
	}

	std::cout << "size: " << gra.size() << std::endl;
	std::cout << "range: " << gra.range() << std::endl;
	std::cout << "index for vertex 1: " << gra.find_index( 1 ) << std::endl;
	std::cout << "index for vertex 5: " << gra.find_index( 5 ) << std::endl;
	std::cout << "index for vertex 8: " << gra.find_index( 8 ) << std::endl;
	std::cout << "index for vertex 17: " << gra.find_index( 17 ) << std::endl;
	cout << gra[10] << ", " << gra[gra.range()-1] << endl;
	std::cout << "graph now: " << std::endl << gra << std::endl;

	gra.insert_e( 3, 4, 5 );
	gra.insert_e_byi( 3, 6, 2 );
	gra.insert_2e( 5, 7, 100 );
	gra.insert_2e_byi( 8, 9, 200 );
	std::cout << "graph now: " << std::endl << gra << std::endl;

	gra.remove_e( 3, 4 );
	gra.remove_e_byi( 3, 6 );
	gra.remove_2e( 5, 7 );
	gra.remove_2e_byi( 8, 9 );
	std::cout << "graph now: " << std::endl << gra << std::endl;

	gra.insert_e( 3, 4, 5 );
	gra.insert_e( 3, 6, 2 );
	std::cout << "graph now: " << std::endl << gra << std::endl;
	gra.remove_alle( 3 );
	std::cout << "graph now: " << std::endl << gra << std::endl;

	gra.insert_e( 3, 4, 11 );
	gra.insert_e( 6, 7, 19 );
	gra.insert_e( 3, 5, 7 );
	gra.insert_e( 4, 5, 2 );
	gra.insert_e( 5, 6, 2 );
	gra.insert_e( 5, 7, 3 );
	std::cout << "graph now: " << std::endl << gra << std::endl;
	gra.remove_v( 5 );
	std::cout << "graph size now: " << gra.size()
			  << ", graph now 101: " << std::endl << gra << std::endl;

	gra.insert_v( 5 );
	gra.insert_e( 3, 4, 11 );
	gra.insert_e( 6, 7, 19 );
	gra.insert_e( 3, 5, 7 );
	gra.insert_e( 4, 5, 2 );
	gra.insert_e( 5, 6, 2 );
	gra.insert_e( 5, 7, 3 );
	std::cout << "graph now: " << std::endl << gra << std::endl;
	int vi = gra.find_index( 5 );
	gra.remove_v_byi( vi );
	std::cout << "graph size now: " << gra.size()
			  << ", graph now 102: " << std::endl << gra << std::endl;

	gra.remove_v( 13 );
	gra.remove_v( 12 );
	gra.remove_v( 11 );
	gra.insert_v( 8 );
	gra.insert_v( 3 );
	gra.insert_v( 19 );
	gra.insert_v( 5 );
	gra.insert_v( 12 );
	std::cout << "graph now 103: " << std::endl << gra << std::endl;

	gra.insert_e( 3, 4, 11 );
	gra.insert_e( 6, 7, 19 );
	gra.insert_e( 3, 5, 7 );
	gra.insert_e( 4, 5, 2 );
	gra.insert_e( 5, 6, 2 );
	gra.insert_e( 5, 7, 3 );
	gra.insert_e( 10, 2, 3 );
	gra.insert_e( 10, 12, 3 );
	gra.insert_e( 2, 19, 3 );
	std::cout << "graph now 104: " << std::endl << gra << std::endl;

	gra.remove_v_byi( 1 );
	gra.remove_v_byi( 3 );
	gra.remove_v_byi( 4 );
	gra.remove_v_byi( 7 );
	gra.remove_v_byi( 9 );
	gra.remove_v( 1 );
	gra.remove_v( 8 );
	std::cout << "graph now 105: " << std::endl << gra << std::endl;
	gra.reserve( gra.range() + 4 );
	std::cout << "graph size now: " << gra.size()
			  << ", range: " << gra.range()
			  << ", graph now 106: " << std::endl << gra << std::endl;

	CGraph<int, int> gra101 = gra;
	gra.pack( );
	cout << "after pack(0), graph size now: " << gra.size()
		 << ", range: " << gra.range()
		 << ", graph now 1-111: " << std::endl << gra << std::endl;

	gra101.pack( gra101.size() + 2 );
	cout << "after pack(), graph size now: " << gra101.size()
		 << ", range: " << gra101.range()
		 << ", graph now 1-112: " << std::endl << gra << std::endl;


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

	CGraph<char, int>::iterator it = gra2.find_vertex( 'a' );
	if( it != gra2.end() ) {
		cout << "find: " << *it << endl;
	}
	CGraph<char, int>::const_iterator it2 = gra2.find_vertex( 'b' );
	if( it2 != gra2.end() ) {
		cout << "find: " << *it2 << endl;
	}

	int *pn = gra2.get_edge( 'a', 'b' );
	if( pn ) {
		cout << "edge data from 'a' to 'b': " << *pn << endl;
	}
	pn = gra2.get_edge_byi( 0, 1 );
	if( pn ) {
		cout << "edge data from 0 to 1: " << *pn << endl;
	}

	gra.insert_v_ati( 101, 2 );
	std::cout << "graph size now: " << gra.size()
			  << ", range: " << gra.range()
			  << ", graph now 108: " << std::endl << gra << std::endl;
	gra.insert_v_ati( 102, 5 );
	std::cout << "graph size now: " << gra.size()
			  << ", range: " << gra.range()
			  << ", graph now 109: " << std::endl << gra << std::endl;
	gra.insert_v_ati( 103, 20 );
	std::cout << "graph size now: " << gra.size()
			  << ", range: " << gra.range()
			  << ", graph now 110: " << std::endl << gra << std::endl;

	gra[20].vertex_d() = 109;
	cout << "set: " << gra.set_v_ati( 111, 0 ) << endl;
	cout << "set: " << gra.set_v_ati( 112, 3 ) << endl;
	std::cout << "graph size now: " << gra.size()
			  << ", range: " << gra.range()
			  << ", graph now 111: " << std::endl << gra << std::endl;

	gra.clear();
	std::cout << "graph size now: " << gra.size()
			  << ", range: " << gra.range()
			  << ", graph now 110: " << std::endl << gra << std::endl;

	int xi = 100;
	try {
		gra2.at( xi );
	} catch( std::out_of_range &ore ) {
		cout << "exception accessing node indexed " << xi << ": " << ore.what() << endl;
	}

	return 0;
}
