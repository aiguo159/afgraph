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


/* t_basics.cpp

  test edge classes, some utility templates (util_tl.h),
  const member function adaptors (my_mem_fun.h), which is needed
  for VC but not for g++.

  Aiguo Fei, 2000
*/

#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>

#include "afgraph/edge.h"
#include "afl/util_tl.hpp"

//using namespace std;
using namespace afg;
//using namespace afl;

template<class ET, class Fun>
void outputw( const ET &e, Fun fw )
{
	std::cout << "weight of edge: " << fw( e ) << std::endl;
}

template<class ET, class Fun>
void p_outputw( const ET &e, Fun fw )
{
	std::cout << "weight of edge: " << fw( &e ) << std::endl;
}


int main()
{
	// test CEdgeW2 class and helper function objects
	afg::CEdgeW2<int, double> e1( 12, 3.4 );
	afg::CEdgeW2<int, double> e2( 13, 2.5 );

	std::cout << e1 << std::endl;
	std::cout << e2 << std::endl;

	std::list<CEdgeW2<int, double> > le;
	le.push_back( e1 );
	le.push_back( e2 );
	std::for_each( le.begin(), le.end(),
				   std::mem_fun_ref( &CEdgeW2<int, double>::weight1 ) );

	outputw( e1, afl::runit< CEdgeW2<int, double> >() );
	p_outputw( e1, afl::runit_p< CEdgeW2<int, double> >() );
	outputw( e1, std::mem_fun_ref( &CEdgeW2<int, double>::weight1 ) );
	outputw( e1, std::mem_fun_ref( &CEdgeW2<int, double>::weight2 ) );

	int nw = 101;
	outputw( nw, afl::runit<int>() );
	outputw( nw, afl::ref2value<int>() );

	return 0;
}
