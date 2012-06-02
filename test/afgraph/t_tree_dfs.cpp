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

#include "afgraph/rtree.h"
#include "afgraph/tree_alg.h"

using namespace std;

using namespace afg;

int main( void )
{
	CrTree<int, double> tree0( 5, -1 );

	tree0.insert_v( 101 );
	tree0.insert_v( 102 );
	tree0.insert_v( 103 );
	tree0.insert_e( 101, 102, 3.7 );
	tree0.insert_e( 101, 103, 4.2 );

	tree0.insert_v( 100 );
	tree0.insert_e( 100, 101, 5.8 );
	cout << "tree: " << endl << tree0 << endl;

	int ns = tree0.find_index( 100 );
	cout << "ns: " << ns << endl;

	tree_dfs_c<CrTree<int, double> > tdfs( tree0 );
	if( ns >= 0 ) {
		const double *pedge;
		tdfs.dfs_init( ns );
		int np, nd;
		pedge = tdfs.dfs_next( np, nd );
		while( pedge ) {
			cout << "dfs: " << np << "->" << nd << ": " << *pedge << endl;
			pedge = tdfs.dfs_next( np, nd );
		}
	}

	cout << endl;
	tree_dfs<CrTree<int, double> > tdfs2( tree0 );
	if( ns >= 0 ) {
		double *pedge;
		tdfs2.dfs_init( ns );
		int np, nd;
		pedge = tdfs2.dfs_next( np, nd );
		while( pedge ) {
			cout << "dfs: " << np << "->" << nd << ": " << *pedge << endl;
			*pedge = 3.3;
			pedge = tdfs2.dfs_next( np, nd );
		}
		cout << "new tree:\n" << tree0 << endl;
	}

	return 0;
}
