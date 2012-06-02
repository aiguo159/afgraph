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

	cout << tree0 << endl;

	CrTree<int, double> tree1( tree0 );
	cout << tree1 << endl;
	CrTree<int, double> tree2 = tree1;
	cout << tree2 << endl;

	tree1.insert_v( 100 );
	tree1.insert_e( 100, 101, 5.8 );
	cout << "tree1: " << endl << tree1 << endl;
	tree2.insert_v( 100 );
	tree2.insert_e( 100, 101, 5.8 );
	tree2.remove_e( 101, 102 );
	cout << "tree2: " << endl << tree2 << endl;
	tree2.insert_e_byi( 0, 1, 4.7 );
	cout << "tree2: " << endl << tree2 << endl;

	tree0.copy_vertices(( CrTree<int, double>::GT )tree2 );
	cout << "tree0: " << endl << tree0 << endl;

	return 0;
}
