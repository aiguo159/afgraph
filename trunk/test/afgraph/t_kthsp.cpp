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


/* tkthsp.cpp

  test the kth shortest path algorithm implemented in kthsp.h

  Aiguo Fei, UCLA 2000
*/

#include <vector>
#include <set>
#include <functional>

#ifdef _NEEDCMEMFUN_ // g++ doesn't need this
#include "my_mem_fun.h"
#endif

#include "afgraph/graph.h"
#include "afgraph/edge.h"
#include "afgraph/gtypes.h"
#include "afgraph/kthsp.h"

using namespace std;

using namespace afg;

int main()
{
	int n = 13;
	typedef CGraph<int, T_d3Edge> T_d3Graph;
	T_d3Graph gra3( n, -1 );
	for( int ii = 0; ii < n; ++ii ) {
		gra3.insert_v( ii );
	}
	gra3.insert_2e( 0, 1, T_d3Edge( 20, 30, 30 ) );
	gra3.insert_2e( 0, 4, T_d3Edge( 15, 30, 20 ) );
	gra3.insert_2e( 0, 5, T_d3Edge( 20, 30, 20 ) );
	gra3.insert_2e( 1, 2, T_d3Edge( 30, 20, 20 ) );
	gra3.insert_2e( 1, 6, T_d3Edge( 10, 30, 30 ) );
	gra3.insert_2e( 2, 3, T_d3Edge( 20, 20, 20 ) );
	gra3.insert_2e( 2, 7, T_d3Edge( 10, 20, 20 ) );
	gra3.insert_2e( 3, 7, T_d3Edge( 10, 10, 10 ) );
	gra3.insert_2e( 4, 6, T_d3Edge( 15, 20, 20 ) );
	gra3.insert_2e( 5, 6, T_d3Edge( 20, 20, 20 ) );
	gra3.insert_2e( 5, 9, T_d3Edge( 10, 25, 10 ) );
	gra3.insert_2e( 6, 7, T_d3Edge( 10, 20, 10 ) );
	gra3.insert_2e( 6, 10, T_d3Edge( 10, 20, 20 ) );
	gra3.insert_2e( 7, 8, T_d3Edge( 10, 10, 10 ) );
	gra3.insert_2e( 7, 11, T_d3Edge( 20, 25, 20 ) );
	gra3.insert_2e( 9, 10, T_d3Edge( 10, 25, 10 ) );
	gra3.insert_2e( 10, 11, T_d3Edge( 10, 25, 20 ) );
	gra3.insert_2e( 10, 12, T_d3Edge( 10, 10, 20 ) );

	cout << "graph: \n" << gra3 << endl;

	CkthSP< T_d3Graph, const_mem_fun_t<double, T_d3Edge> >
	ksp( gra3, 0, 7, mem_fun<double, T_d3Edge>( &T_d3Edge::weight1 ), 10000 );
	vector<int> vpred( n, -1 );
	ksp.get_first( );

	double dretn;
	CPath lp;
	dretn = ksp.get_path( 11, 1, lp );
	cout << "1st sp from 0 to 11: " << lp << ": " << dretn << endl << endl;

	ksp.get_next( );
	dretn = ksp.get_path( 11, 2, lp );
	cout << "2nd sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	dretn = ksp.get_path( 0, 2, lp );
	cout << "2nd sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	ksp.get_next();
	dretn = ksp.get_path( 11, 3, lp );
	cout << "3rd sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	dretn = ksp.get_path( 0, 3, lp );
	cout << "3nd sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	ksp.get_next();
	dretn = ksp.get_path( 11, 4, lp );
	cout << "4th sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	dretn = ksp.get_path( 0, 4, lp );
	cout << "4th sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	ksp.get_next();
	dretn = ksp.get_path( 11, 5, lp );
	cout << "5th sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	dretn = ksp.get_path( 0, 5, lp );
	cout << "5th sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	ksp.get_next();
	dretn = ksp.get_path( 11, 6, lp );
	cout << "6th sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	dretn = ksp.get_path( 0, 6, lp );
	cout << "6th sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	ksp.get_next();
	dretn = ksp.get_path( 11, 7, lp );
	cout << "7th sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl
		 << "path length on weight 2: "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight2 ) )
		 << endl
		 << "path length on weight 3: "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight3 ) )
		 << endl
		 << endl;

	dretn = ksp.get_path( 0, 7, lp );
	cout << "7th sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	ksp.get_next();
	dretn = ksp.get_path( 11, 8, lp );
	cout << "8th sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	dretn = ksp.get_path( 0, 8, lp );
	cout << "8th sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	ksp.increase_k( 10 );
	ksp.get_next();
	dretn = ksp.get_path( 11, 8, lp );
	cout << "8th sp from 0 to 11: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	dretn = ksp.get_path( 0, 8, lp );
	cout << "8th sp from 0 to 0: " << lp << ": " << dretn
		 << ", path length from path_length(): "
		 << path_length( gra3, lp, mem_fun( &T_d3Edge::weight1 ) )
		 << endl << endl;

	return 0;
}
