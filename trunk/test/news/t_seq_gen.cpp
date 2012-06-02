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


/*@fname t_seq_gen.cpp

  test node addition/deletion sequence generation in news/grp_gen.h.
*/

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

#include <algorithm>

#include <time.h>

#include "afgraph/graph.h"

#include "news/grp_gen.h"

#include "research/import_gitalt.h"


#include "afl/rand.hpp"


typedef std::pair<int, int> INP;

using namespace std;
using namespace afg;

template<class VDT, class EDT>
class CnGraph: public CGraph<VDT, EDT>
{
public:
	CnGraph( int size = 1, const VDT &v = VDT(), double dg = 0.25 )
		: CGraph<VDT, EDT>( size, v, dg )
	{ }

	bool can_be_member( int ) const {
		return true;
	}
};

int main()
{
	afl::rand::init();

	typedef CnGraph<tsVertex, int> GT;

	int nsize = 100;
	GT gts1( nsize );

	std::ifstream fsfrom( "ts100-0.alt" );
	if( fsfrom ) {
		import_gitalt_ts( gts1, fsfrom );

		fsfrom.close();

		std::list<INP> lact;
		std::set<int> sdest;
		int ns = 20;
		double alpha = 0.1;
		int nt = 40;
		news::rand_seq_gen( gts1, alpha, ns, nt, lact, sdest );

		//copy( lact.begin(), lact.end(), ostream_iterator<pair<int, int> >( cout, "\n" ) );
		std::list<INP>::iterator ilp;
		for( ilp = lact.begin(); ilp != lact.end(); ++ilp ) {
			cout << ( *ilp ).first << ", " << ( *ilp ).second << endl;
		}
		copy( sdest.begin(), sdest.end(), ostream_iterator<int>( cout, " " ) );
		cout << endl;
	}

	return 0;
}

