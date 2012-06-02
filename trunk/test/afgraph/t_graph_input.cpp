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


/*
	Test graph input facilities.

	Run the test program from the source directory (i.e., where this file
	and sample graph data files are located), so it can open the data files.

*/

#include <string>
#include <iostream>
#include <fstream>

#include "afl/exceptions.hpp"
#include "afgraph/graph.h"
#include "afgraph/graph_input.h"

using namespace std;

using namespace afg;

using namespace afl;

//general_except x;

int main()
{
	typedef CGraph<int, int> GT;
	GT gra1( 2, -1 );
	std::ifstream fsfrom1( "gra1.die" );
	if( fsfrom1 ) {
		try {
			input_gra_die( gra1, fsfrom1 );
		} catch( general_except<std::string>& ge ) {
			cout << ge.what() << endl;
		}

		fsfrom1.close();
		cout << "graph 1:\n" << gra1 << endl;
	} else {
		cout << "failed to open graph data file: gra1.die\n";
	}

	GT gra2( 5, -1 );
	std::ifstream fsfrom2( "gra2.vie" );
	if( fsfrom2 ) {
		try {
			input_gra_vie( gra2, fsfrom2 );
		} catch( general_except<std::string>& ge ) {
			cout << ge.what() << endl;
		}

		fsfrom2.close();
		cout << "graph 2:\n" << gra2 << endl;
	} else {
		cout << "failed to open graph data file: gra2.vie\n";
	}

	CGraph<string, int> gra3( 31, "null" );
	std::ifstream fsfrom3( "indn_edcnet.vie" );
	if( fsfrom3 ) {
		try {
			input_gra_vie( gra3, fsfrom3 );
		} catch( general_except<std::string>& ge ) {
			cout << ge.what() << endl;
		}

		fsfrom3.close();
		cout << "graph 3 (Indiana EDC Network):\n" << gra3 << endl;
	} else {
		cout << "failed to open graph data file: indn_edcnet.vie\n";
	}

	CGraph<string, int> gra4( 84, "null" );
	std::ifstream fsfrom4( "mbone-all-199405.vve" );
	if( fsfrom4 ) {
		try {
			input_gra_vve< CGraph<string, int>, string>( gra4, fsfrom4 );
		} catch( general_except<std::string>& ge ) {
			cout << ge.what() << endl;
		}

		fsfrom4.close();
		cout << "graph 4 (MBone as of 1994):\n" << gra4 << endl;
	} else {
		cout << "failed to open graph data file: mbone-all-199405.vve\n";
	}

	return 0;
}
