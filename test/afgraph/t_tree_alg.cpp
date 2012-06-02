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
#include <vector>
#include <set>
//#include <functional>

#include "afl/util_tl.hpp"
#include "afgraph/graph.h"
#include "afgraph/shortest_path.h"
#include "afgraph/rtree.h"
#include "afgraph/path.h"
#include "afgraph/tree_alg.h"

using namespace std;
//using namespace afl;
using namespace afg;

template < class GT, class WT1 = int, class WT2 = double, class WT3 = int >
class CnmcTree
{
};

/*
template<class T1, class T=double, class T2=int, class T3=int>
class X
{
};
*/
int main( void )
{
	CGraph<int, double> g0( 5, -1 );

	g0.insert_v( 101 );
	g0.insert_v( 102 );
	g0.insert_v( 103 );
	g0.insert_e( 101, 102, 3.7 );
	g0.insert_e( 101, 103, 4.2 );

	int ns = g0.insert_v( 100 );
	g0.insert_e( 100, 101, 5.8 );
	g0.insert_e( 100, 103, 6.3 );
	cout << "graph: " << endl << g0 << ", ns=" << ns << endl;

	CrTree<int, double> tr0( 5, -1 );
	tr0.copy_vertices( g0 );
	cout << "tree 1:\n" << tr0 << endl;
	/*
	  CrTree<int, double> tr1( 5, -1);
	  sptree_all( g0, ns, tr1, afl::pointer2value<double>(), 100 );
	  cout<<"sp tree: "<<endl<<tr1<<endl;

	  CrTree<int, double> tr2( 5, -1);
	  set<int> sm;
	  //sm.insert( g0.find_index(100) );
	  sm.insert( g0.find_index(101) );
	  sptree( g0, ns, tr2, sm, afl::pointer2value<double>(), 100 );
	  cout<<"sp tree: "<<endl<<tr2<<endl;
	  tr2.root()=tr2.find_index( 100);
	  tr2.pack();
	  cout<<"tree after pack: "<<endl<<tr2<<endl
	      <<"root: "<<tr2.root()<<endl;
	*/

	return 0;
}
