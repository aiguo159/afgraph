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
#include <vector>
#include <set>
//#include <functional>

#include "afl/util_tl.hpp"
#include "afgraph/graph.h"
#include "afgraph/shortest_path.h"
#include "afgraph/rtree.h"
#include "afgraph/tree_util.h"
#include "afgraph/tree_alg.h"

using namespace std;
using namespace afg;

int main( void )
{
	CGraph<int, double> g0( 5, -1 );

	g0.insert_v( 101 );
	g0.insert_v( 102 );
	g0.insert_v( 103 );
	g0.insert_e( 101, 102, 3.7 );
	g0.insert_e( 101, 103, 4.2 );

	int ns = g0.insert_v( 100 );
	g0.insert_e( 100, 101, 5.8 );
	g0.insert_e( 100, 103, 6.3 );
	cout << "graph: " << endl << g0 << endl;

	if( ns >= 0 ) {
		vector<int> vpred( g0.range() );
		vector<double> vdist( g0.range() );
		try {
			dijkstra<CGraph<int, double> >( g0, ns,
											afl::pointer2value<double>(), 100, vpred, vdist );
		} catch( ... ) {
			cerr << "error in dijkstra, quit\n";
			exit( 1 );
		}

		CrTree<int, double> tr0( 5, -1 );
		bool bre = pred2tree< CGraph<int, double>, CrTree<int, double> >
				   ( g0, vpred, tr0 );
		if( bre ) {
			cout << "shorest path tree from 100\n" << tr0 << endl;
		}

		CrTree<int, double> tr01( 5, -1 );
		bre = pred2tree_s< CrTree<int, double> >
			  ( vpred, vdist, tr01 );
		if( bre ) {
			cout << "simple shorest path tree from 100\n" << tr01 << endl;
		}

		set<int> inset;
		inset.insert( ns );
		inset.insert( tr0.find_index( 103 ) );
		CrTree<int, double> tr1 = tr0;
		prune_tree<CrTree<int, double> >( tr1, inset );
		cout << "pruned tree: \n" << tr1 << endl;

		inset.insert( tr0.find_index( 102 ) );
		tr1 = tr0;
		cout << "tr1: \n" << tr1 << endl;
		prune_tree( tr1, inset );
		cout << "pruned tree: \n" << tr1 << endl;

		// extend tree
		g0.pack();
		int n = g0.range();
		vector<int> vapred( n * n );
		vector<double> vadist( n * n );
		cout << "find all sp: "
			 << floyd_warshall_allsp<CGraph<int, double> >( g0,
					 afl::pointer2value<double>(),
					 1000, vapred, vadist )
			 << endl;
		copy( vapred.begin(), vapred.end(), ostream_iterator<double>( std::cout, " " ) );
		cout << endl;
		copy( vadist.begin(), vadist.end(), ostream_iterator<double>( std::cout, " " ) );
		cout << endl;
		CrTree<int, double> tree3( 4, -1 );
		tree3.insert_v( g0.find_index( 100 ) );
		cout << "tree before extend: \n" << tree3 << endl;
		set<int> snodes;
		snodes.insert( g0.find_index( 102 ) );
		cout << "ext: "
			 << extend_greedy_s<CrTree<int, double>, double >( tree3, snodes, g0.range(),
					 vapred, vadist, 1000 )
			 << endl;
		cout << "extended tree: \n" << tree3 << endl;

		snodes.insert( g0.find_index( 103 ) );
		cout << "ext: "
			 << extend_greedy_s<CrTree<int, double>, double >( tree3, snodes, g0.range(),
					 vapred, vadist, 1000 )
			 << endl;
		cout << "extended tree: \n" << tree3 << endl;

		snodes.insert( g0.find_index( 101 ) );
		cout << "ext: "
			 << extend_greedy_s<CrTree<int, double>, double >( tree3, snodes, g0.range(),
					 vapred, vadist, 1000 )
			 << endl;
		cout << "extended tree: \n" << tree3 << endl;

	}
	return 0;
}
