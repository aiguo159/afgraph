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


/**@file graph_input.h

*/
#ifndef _AFG_GRAPH_INPUT_H_
#define _AFG_GRAPH_INPUT_H_

#include <string>

#include <sstream>

#include "gexception.h"

namespace afg
{

/**@defgroup ginput graph input utilities
@ingroup gutil
*/

/**@{*/

/* get header information for a graph
sformat: format code, nn: # of nodes, nl: # of edges (links),
sdir: directional information
*/
void input_gra_header( std::istream &is, std::string &sformat,
					   int &nn, int &nl,
					   std::string &sdir )
{
	std::string sbuf;
	while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
		;
	sformat = sbuf;
	while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
		;

	std::istringstream ist( sbuf );

	ist >> nn;
	while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
		;

	std::istringstream ist2( sbuf );

	ist2 >> nl;
	while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
		;
	sdir = sbuf;
}
//}

/** input graph for type die: default integer ids (from 0 to n-1) and
  links with certain type of edge data,
  nodes are not explicitly listed in the file
  @exception general_except with a specific error message
*/
template< class GT>
void input_gra_die( GT &gra, std::istream &is )
{
	int nnodes = -1, nlinks = -1;
	std::string sformat, sdir, sbuf;
	input_gra_header( is, sformat, nnodes, nlinks, sdir );
	//std::cout<<sformat<<", "<<nnodes<<", "<<nlinks<<", "<<sdir<<endl;

	if( sformat.size() < 3 || sformat.c_str()[0] != 'd' ||
		sformat.c_str()[1] != 'i' || sformat.c_str()[2] != 'e' ) {
		throw afl::general_except<std::string>(
			std::string( "unmatched input graph file format" ) );
	}

	if( nnodes <= 0 || nlinks <= 0 || sdir.size() < 2 ) {
		throw afl::general_except<std::string>( "illegal graph parameter" );
	}

	typedef typename GT::EDT EDT;
	int i;
	// insert all vertices
	gra.reserve( nnodes );
	for( i = 0; i < nnodes; ++i ) {
		gra.insert_v_ati( i, i );
	}

	//std::cout<<gra<<std::endl;

	int ntotal = 0;
	int j;
	EDT w;

	if( sdir.c_str()[0] == 'd' && sdir.c_str()[1] == 'i' ) {
		// uni-directional
		while( ntotal < nlinks && is ) {
			while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
				;

			std::istringstream iste( sbuf );

			iste >> i >> j >> w;
			if( i >= 0 && i < nnodes && j >= 0 && j < nnodes ) {
				gra.insert_e_byi( i, j, w );
			} else {
				throw afl::general_except<std::string>( "input edge error" );
			}
			ntotal++;
		}
	} else {
		// treat as bi-directional
		while( ntotal < nlinks && is ) {
			while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
				;

			std::istringstream iste( sbuf );

			iste >> i >> j >> w;
			if( i >= 0 && i < nnodes && j >= 0 && j < nnodes ) {
				gra.insert_2e_byi( i, j, w );
			} else {
				throw afl::general_except<std::string>( "input edge error" );
			}
			ntotal++;
		}
	}
}

/** input graph for type vie
  @exception general_except with a specific error message
*/
template< class GT>
void input_gra_vie( GT &gra, std::istream &is )
{
	int nnodes = -1, nlinks = -1;
	std::string sformat, sdir, sbuf;
	input_gra_header( is, sformat, nnodes, nlinks, sdir );
	//std::cout<<sformat<<", "<<nnodes<<", "<<nlinks<<", "<<sdir<<endl;

	if( sformat.size() < 3 || sformat.c_str()[0] != 'v' ||
		sformat.c_str()[1] != 'i' || sformat.c_str()[2] != 'e' ) {
		throw afl::general_except<std::string>( "unmatched input graph file format" );
	}

	if( nnodes <= 0 || nlinks <= 0 || sdir.size() < 2 ) {
		throw afl::general_except<std::string>( "illegal graph parameter" );
	}

	typedef typename GT::VDT VDT;
	typedef typename GT::EDT EDT;
	int i;
	int ntotal = 0;
	VDT vd;
	//cout<<"reading vertices\n";
	while( ntotal < nnodes && is ) {
		while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
			;

		std::istringstream istv( sbuf );

		istv >> vd;
		gra.insert_v_ati( vd, ntotal );

		ntotal++;
	}

	//std::cout<<gra<<std::endl;

	int j;
	EDT edg;
	ntotal = 0;
	//cout<<"reading links\n";
	if( sdir.c_str()[0] == 'd' && sdir.c_str()[1] == 'i' ) {
		// uni-directional
		while( ntotal < nlinks && is ) {
			while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
				;

			std::istringstream iste( sbuf );

			iste >> i >> j >> edg;
			if( i >= 0 && i < nnodes && j >= 0 && j < nnodes ) {
				gra.insert_e_byi( i, j, edg );
			} else {
				throw afl::general_except<std::string>( "input edge error" );
			}
			ntotal++;
		}
	} else {
		// treat as bi-directional
		while( ntotal < nlinks && is ) {
			while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
				;

			std::istringstream iste( sbuf );

			iste >> i >> j >> edg;
			if( i >= 0 && i < nnodes && j >= 0 && j < nnodes ) {
				gra.insert_2e_byi( i, j, edg );
			} else {
				throw afl::general_except<std::string>( "input edge error" );
			}
			ntotal++;
		}
	}
	//std::cout<<gra<<std::endl;
}

/** input graph for type vve
  @exception general_except with a specific error message
*/
template< class GT, class VIDT >
void input_gra_vve( GT &gra, std::istream &is )
{
	int nnodes = -1, nlinks = -1;
	std::string sformat, sdir, sbuf;
	input_gra_header( is, sformat, nnodes, nlinks, sdir );
	//std::cout<<sformat<<", "<<nnodes<<", "<<nlinks<<", "<<sdir<<endl;

	if( sformat.size() < 3 || sformat.c_str()[0] != 'v' ||
		sformat.c_str()[1] != 'v' || sformat.c_str()[2] != 'e' ) {
		throw afl::general_except<std::string>( "unmatched input graph file format" );
	}

	if( nnodes <= 0 || nlinks <= 0 || sdir.size() < 2 ) {
		throw afl::general_except<std::string>( "illegal graph parameter" );
	}

	typedef typename GT::VDT VDT;
	typedef typename GT::EDT EDT;

	int ntotal = 0;
	VDT vd;
	//cout<<"reading vertices\n";
	while( ntotal < nnodes && is ) {
		while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
			;

		std::istringstream istv( sbuf );

		istv >> vd;
		gra.insert_v_ati( vd, ntotal );
		//cout<<ntotal<<": "<<vd<<endl;
		ntotal++;
	}

	//std::cout<<gra<<std::endl;

	VIDT vid1, vid2;
	EDT edg;
	ntotal = 0;
	//cout<<"reading links\n";
	if( sdir.c_str()[0] == 'd' && sdir.c_str()[1] == 'i' ) {
		// uni-directional
		while( ntotal < nlinks && is ) {
			while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
				;

			//cout<<sbuf<<endl;
			std::istringstream iste( sbuf );

			iste >> vid1 >> vid2 >> edg;
			gra.insert_e( VDT( vid1 ), VDT( vid2 ), edg );
			ntotal++;
		}
	} else {
		// treat as bi-directional
		while( ntotal < nlinks && is ) {
			while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
				;

			std::istringstream iste( sbuf );

			iste >> vid1 >> vid2 >> edg;
			//cout<<vid1<<", "<<vid2<<": "<<edg<<endl;
			gra.insert_2e( VDT( vid1 ), VDT( vid2 ), edg );
			ntotal++;
		}
	}
	//std::cout<<gra<<std::endl;
}

// input graph with vertex data, edges by indices and double edges
template< class GT>
bool input_gra_i_2e( GT &gra, std::istream &is )
{
	typedef typename GT::VDT VDT;
	typedef typename GT::EDT EDT;

	//int nmaxline=127;
	int nnodes, nlinks;
	std::string sbuf;
	while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
		;
	std::istringstream ist( sbuf );
	ist >> nnodes;
	//std::cout<<nnodes<<std::endl;
	while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
		;
	std::istringstream ist2( sbuf );
	ist2 >> nlinks;
	//std::cout<<nlinks<<std::endl;

	//cout<<"read: "<<nnodes<<", "<<nlinks<<endl;
	if( nnodes <= 0 || nlinks <= 0 ) {
		//cout<<"illegal input: "<<nnodes<<", "<<nlinks<<endl;
		return false;
	}

	int i;
	int ntotal = 0;
	VDT vd;
	//cout<<"reading vertices\n";
	while( ntotal < nnodes && is ) {
		while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
			;
		std::istringstream istv( sbuf );
		istv >> vd;
		gra.insert_v_ati( vd, ntotal );

		ntotal++;
	}

	//std::cout<<gra<<std::endl;

	int j;
	EDT edg;
	ntotal = 0;
	//cout<<"reading links\n";
	while( ntotal < nlinks && is ) {
		while( std::getline( is, sbuf ) && ( sbuf.size() < 1 || sbuf.at( 0 ) == '#' ) )
			;
		std::istringstream iste( sbuf );
		iste >> i >> j >> edg;
		//cout<<i<<", "<<", "<<edg<<endl;
		if( i >= 0 && i < nnodes && j >= 0 && j < nnodes ) {
			gra.insert_2e_byi( i, j, edg );
		} else {
			return false;
		}
		//cout<<i<<": "<<x<<", "<<y<<endl;
		ntotal++;
	}

	//std::cout<<gra<<std::endl;

	return true;

}

/**@}*/

}

#endif
