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


/* t_path.cpp

  test CPath defined in path.h

  Aiguo Fei

  1999, 2000
*/

#include <iostream>

#include <vector>

#include "afgraph/path.h"

using afg::CPath;

int main()
{
	CPath p( 2 );
	p.push_back( 4 );
	p.push_back( 3 );
	p.insert_before( 5, 4 );
	p.insert_after( 1, 4 );
	std::cout << p << std::endl;

	int pna[5] = { -1, 3, 0, 0, 1};
	std::cout << "pred2path result: " << afg::pred2path( 5, pna, 0, 4, p )
			  << ", " << p << std::endl;

	std::vector<int> vpred( 5 );
	vpred[0] = -1;
	vpred[1] = 3;
	vpred[2] = 0;
	vpred[3] = 0;
	vpred[4] = 1;
	std::cout << "pred2path result: " << afg::pred2path( vpred, 0, 4, p )
			  << ", " << p << std::endl;

	return 0;
}
