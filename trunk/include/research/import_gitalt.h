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


/**@file import_gitalt.h

*/

#ifndef _AFG_IMPORT_GITALT_H_
#define _AFG_IMPORT_GITALT_H_

#include <string>
#include <iostream>
#include <cstring>

#include <sstream>

#include "afgraph/vertex.h"

/**@defgroup research Stuff for Research Work
*/

namespace afg
{

/**@addtogroup research
@{
*/

/** import graph from the alt file by GeorgiaTech graph generator.
  File format (transit stub graph): first line is some explaination<br>
    2nd line: number_of_nodes  number_of_edges keyword"transtub"<br>
    3rd line: empty line<BR>
    4th line: vertex line<BR>
    list of nodes (as many as number_nodes)<BR>
    empty line<BR>
    edge line<BR>
    list of links (as many as half of number_edges -- bidirectional links)<BR>
  GT is the graph type, it must have vertex data type tsVertex and edge
    data type int (or convertions from them provided).
*/
template< class GT>
bool import_gitalt_ts( GT &gra, std::istream &is )
{
	std::string sbuf;
	if( is && std::getline( is, sbuf ) ) {
		// get first line
		if( std::getline( is, sbuf ) ) {
			// second line
			int nnodes, nedges;
			std::string skey;

			std::istringstream ist1( sbuf );

			ist1 >> nnodes >> nedges >> skey;
			//std::cout<<nnodes<<", "<<nedges<<skey<<std::endl;
			if( nnodes > 0 && strncmp( skey.c_str(), "transtub", 8 ) == 0 ) {
				std::getline( is, sbuf ); // 3rd line
				std::getline( is, sbuf ); // 4th line
				int i;
				for( i = 0; i < nnodes; ++i ) {
					tsVertex v;
					is >> v;
					//cout<<"v: "<<v<<endl;
					if( !gra.insert_v_ati( v, v.id() ) ) {
						return false;
					}
				}

				std::getline( is, sbuf ); // get the end of line from last getline
				while( sbuf.c_str()[0] != 'E' ) { // get until the explaination line
					std::getline( is, sbuf );
				}
				int nfrom, nto, nleng, nweight;
				for( i = 0; i < nedges / 2; ++i ) {
					is >> nfrom >> nto >> nleng >> nweight;
					//std::cout<<nfrom<<", "<<nto<<", "<<nweight<<std::endl;
					if( !gra.insert_2e_byi( nfrom, nto, nweight ) ) {
						return false;
					}
				}
				return true;
			}
		}
	}

	return false;
}

/**@}*/

}// namespace afg

#endif
