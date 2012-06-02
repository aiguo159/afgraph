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


/* e_graph.cpp

  Graph example.

  Aiguo Fei

  April 2012

*/

#include <iostream>
#include <vector>
#include <functional>

#include <afgraph/graph.h>
#include <afgraph/graph_alg.h>
#include <afgraph/path.h>
#include <afgraph/shortest_path.h>

using namespace std;
using namespace afg;

//typedef CiVertex<char, int> ciVertex_t;

// mem_fun should give us this; but couldn't get it pass the compiler.
// Too much templating, I guess.
char get_vertex_d( const CiVertex<char, int> &v )
{
	return v.vertex_d();
}

int main()
{
	// Create a graph with char vertext type (i.e., each vertex is identified
	// by a char) and integer edge type (i.e., each edge has an integer value).
	// It has 10 vertices and 'z' will not be used as a vertex id.
	// An unused vertex id is needed to populate empty spots (if any) in the
	// vertex array.
	CGraph<char, int> gra( 6, 'z' );

	// insert all the nodes; their IDs are ['a'..'f'].
	for( int i=0; i<6; ++i ){
		gra.insert_v( i+97 ); // 97 is ASCII 'a'
	}

	// Now insert all the edges. It is a non-directed graph, each edge is
	// represented by 2 directed edges.
	gra.insert_2e( 'a', 'b', 3 );
	gra.insert_2e( 'a', 'd', 4 );
	gra.insert_2e( 'b', 'c', 2 );
	gra.insert_2e( 'b', 'd', 3 );
	gra.insert_2e( 'c', 'f', 6 );
	gra.insert_2e( 'd', 'e', 2 );
	gra.insert_2e( 'e', 'f', 3 );

	// print out the graph; note the difference between vertex ID and vertex index.
	std::cout << "graph now: " << std::endl << gra << std::endl;

	// let's check if this graph is connected.
	std::cout << "connected?: " << (is_connected( gra )?"yes":"no") << std::endl;

	// All graph algorithms operate on vertex indices; let's find the index
	// for node 'c' and 'e'.
	int idxC = gra.find_index( 'c' );
	int idxE = gra.find_index( 'e' );
	cout << "node 'c' is at "<< idxC << ", and node 'e' is at " << idxE << endl;

	/* Run dijkstra algorithm to find the shorted path from 'c' to 'e', using
	each edge's integer value as the weight -- so we pass functor
	afl::pointer2value<int>() for that purpose. The edge weight function passed
	to dijkstra_t() will be called with an pointer to an edge.
	*/
	CPath lp;
	cout << "shortest path by edge weight from node 'c' to 'e' result: "
		 << dijkstra_t<CGraph<char, int> >(
			 gra, idxC, idxE, afl::pointer2value<int>(), 1000, lp )
		 << endl << "path found (by indices): " << lp << endl;
	cout << "path found (by ID): " ;
	lp.output( cout, gra, get_vertex_d ) << endl;
	//lp.output( cout, gra, mem_fun( &ciVertex_t::vertex_d ) ) << endl;

	/* Run dijkstra algorithm to find the shorted path from 'c' to 'e', each
	edge having weight 1 -- equivalent to finding minimum hop-count path.
	*/
	CPath lp2;
	cout << "shortest path by hop-count from node 'c' to 'e' result: "
		 << dijkstra_t<CGraph<char, int> >(
			 gra, idxC, idxE, afl::runit_p<int>(), 1000, lp2 )
		 << endl << "path found (by indices): " << lp2 << endl;
	cout << "path found (by ID): " ;
	lp2.output( cout, gra, get_vertex_d ) << endl;

	return 0;
}

/*
	gra.insert_2e( 'e', 'g', 2 );
	gra.insert_2e( 'f', 'h', 3 );
	gra.insert_2e( 'g', 'h', 2 );
	gra.insert_2e( 'g', 'i', 3 );
	gra.insert_2e( 'i', 'j' ,2 );
*/
