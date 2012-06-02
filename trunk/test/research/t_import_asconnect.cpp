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

#include "research/import_asconnect.h"

using namespace std;
using namespace afg;

int main()
{
	typedef CGraph<int, int> GT;

	int nsize = 3400;
	GT gasc( nsize, -1 );
	std::ifstream fsfrom( "ASconnlist.19980105.txt" );
	if( fsfrom ) {
		//import_asconnect<GT>( gasc, fsfrom);
		import_asconnect( gasc, fsfrom );
	} else {
		cout<<"failed to open file ASconnlist.19980105.txt,\n"
			"make sure you are running the test program from where the file is located.\n";
	}

	gasc.pack();
	cout << "graph: " << gasc.size() << endl << gasc << endl;
	return 0;
}
