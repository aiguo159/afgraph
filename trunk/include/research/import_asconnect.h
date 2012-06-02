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


/**@file import_asconnect.h

*/

#ifndef _AFG_IMPORT_ASCONNECT_H_
#define _AFG_IMPORT_ASCONNECT_H_

#include <string>
#include <iostream>

#include <sstream>

namespace afg
{

/**@addtogroup research
@{
*/

/** import graph from an "asconnect" file.
Asconnect file is a list of AS's and their interconnections,
  available for download from:
  http://www.antc.uoregon.edu/route-views/.

File format per line: as_index -> #_of_neighbors : a list of
  AS neighbors separated by ":".
The imported graph will have vertex data of int type (AS index), each
  edge will have weight 1(type int). All links are assumed to
  be bi-directional.
*/
template< class GT>
void import_asconnect( GT &gra, std::istream &is )
{
	std::string sbuf;
	int node;

	std::string sall;

	while( is && std::getline( is, sbuf ) ) {
		std::istringstream iste( sbuf );
		sall += sbuf + '\n';
		iste >> node;
		gra.insert_v( node );
	}

	std::istringstream isall( sall );

	std::string sgarb;
	char c;
	int nt, n2;
	while( isall && std::getline( isall, sbuf ) ) {

		std::istringstream iste( sbuf );

		iste >> node >> sgarb >> nt;
		//cout<<node<<" "<<sgarb<<" "<<nt<<": ";
		c = 'x';
		while( c != ':' ) {
			iste >> c;
		}
		while( iste ) {
			iste >> n2;
			gra.insert_2e( node, n2, 1 );
			if( iste ) {
				iste >> c;
			}
			//cout<<n2<<", ";
		}
		//cout<<endl;
	}
}

/**@}*/

}// namespace afg

#endif
