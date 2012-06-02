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


/**@file export_gdl.h

  Export gdl (graph description language) file that can be imported to other
  graph visualization tools.

  Aiguo Fei

  2001/03/02, 03/14
*/

#ifndef _AFG_EXPORT_GDL_H_
#define _AFG_EXPORT_GDL_H_

#include <iostream>
#include <string>

#include <set>
#include <algorithm>

#include <sstream>

#include <functional>
#include <vector>

namespace afg
{

/**@defgroup gexport graph export utilities
@ingroup gutil
*/

/**@{*/

/** return the intex of a node as its title.
*/
template<class VertexT>
struct title_ind: public std::binary_function<int, VertexT *, std::string> {
	std::string operator()( int i, const VertexT * ) const {
		std::ostringstream ost;
		ost << i;
		return ost.str();
	}
};

/** return the data of a node as its title.
*/
template<class VertexT>
struct title_vd: public std::binary_function<int, VertexT *, std::string> {
	std::string operator()( int, const VertexT *v ) const {
		std::ostringstream ost;
		ost << v->vertex_d();
		return ost.str();
	}
};

/** return the combination of index and data of a node as its title.
*/
template<class VertexT>
struct title_ivd: public std::binary_function<int, VertexT *, std::string> {
	std::string operator()( int i, const VertexT *v ) const {
		std::ostringstream ost;
		ost << i << " - " << v->vertex_d();
		return ost.str();
	}
};

/** return empty optional attributes for a node.
*/
template<class VertexT>
struct node_empty: public std::binary_function<int, VertexT *, std::string> {
	std::string operator()( int, const VertexT * ) const {
		return std::string( " " );
	}
};

/** combine two node attribute function objects.
*/
template<class VertexT, class F1, class F2 >
struct node_combine:
	public std::binary_function<int, VertexT *, std::string> {

	typedef node_combine<VertexT, F1, F2> my_type;

	F1 m_f1;
	F2 m_f2;

	node_combine( F1 &f1, F2 &f2 ): m_f1( f1 ), m_f2( f2 )
	{}

	std::string operator()( int i, const VertexT *pv ) const {
		std::string s = m_f1( i, pv ) + " " + m_f2( i, pv );
		return s;
	}
};

/** return x,y location attribute for a node.
*/
template<class VertexT>
struct node_xyloc:
	public std::binary_function<int, VertexT *, std::string> {
	/** Constructor.
	The vertex data of the vertex passed is assumed to have member functions
	  x() and y() to access x/y coordinates. The original coordinates may
	  be double, but will be converted to int
	  (gdl file only support int location).
	@param nxs scale factor for x coordinate; i.e., the x coordinate output
	  to the file will be x*nxs where x is the x coordinate
	  contained in the vertex
	@param nys scale factor for y coordinate
	*/
	node_xyloc( int nxs = 10, int nys = 10 ): xscale( nxs ), yscale( nys )
	{ }

	std::string operator()( int , const VertexT *pe ) const {
		std::ostringstream ost;
		ost << "loc: {x: " << ( int )(( pe->vertex_d() ).x()*xscale )
			<< " y: " << ( int )(( pe->vertex_d() ).y()*yscale ) << "}";
		return ost.str();
	}

	int xscale, yscale;
};

/** return a specific textcolor attribute for nodes in a given set.
*/
template<class VertexT>
struct node_color:
	public std::binary_function<int, VertexT *, std::string> {
	/** Constructor.
	@param nodes set of nodes that should be colored.
	@param scolor color for those nodes
	*/
	node_color( const std::set
				<int>& nodes,
				const std::string &scolor = std::string( "red" ) ):
		m_nodeset( nodes ),
		m_scolor( scolor )
	{ }

	std::string operator()( int i, const VertexT *pe ) const {
		if( std::find( m_nodeset.begin(), m_nodeset.end(), i )
			!= m_nodeset.end() ) {
			std::ostringstream ost;
			ost << "textcolor: " << m_scolor;
			return ost.str();
		} else {
			return ( std::string )" ";
		}
	}

	const std::set
	<int>& m_nodeset;
	const std::string m_scolor;
};

/** return an empty attribute for an edge.
*/
template<class EdgeT>
struct edge_empty: public std::binary_function<int, EdgeT *, std::string> {
	std::string operator()( int, const EdgeT * ) const {
		return std::string( " " );
	}
};

/** return an edge data as its label.
*/
template<class EdgeT>
struct edge_label_ed: public std::binary_function<int, EdgeT *, std::string> {
	std::string operator()( int, const EdgeT *pe ) const {
		std::ostringstream ost;
		ost << "label: " << '"' << pe->edge_d() << '"';
		return ost.str();
	}
};

/** combine two edge attribute function objects.
*/
template<class EdgeT, class F1, class F2 >
struct edge_combine: public std::binary_function<int, EdgeT *, std::string> {

	typedef edge_combine<EdgeT, F1, F2> my_type;

	F1 m_f1;
	F2 m_f2;

	edge_combine( F1 &f1, F2 &f2 ): m_f1( f1 ), m_f2( f2 )
	{}

	std::string operator()( int i, const EdgeT *pe ) const {
		std::string s = m_f1( i, pe ) + " " + m_f2( i, pe );
		return s;
	}
};

/** Export a grpah in gdl (graph description language) format.
  The exported gdl file can be imported to other
    graph visualization tools.
*/
// VC has problem with typename within<> before template<> is finished
#ifdef _MSVC_

template < class GraphT, class Ftitle = title_ind<GraphT::iVT >,
		 class Fv_optional = node_empty<GraphT::iVT>,
		 class Fe_optional = edge_empty<GraphT::iET> >
// and g++ won't compile without the typename quanitifier, what a world!
#else

template < class GraphT, class Ftitle = title_ind<typename GraphT::iVT >,
		 class Fv_optional = node_empty<typename GraphT::iVT>,
		 class Fe_optional = edge_empty<typename GraphT::iET> >
#endif

class export_gdl
{
public:
	export_gdl( Ftitle title = Ftitle(),
				Fv_optional v_optional = Fv_optional(),
				Fe_optional e_optional = Fe_optional() )
		: m_ftitle( title ), m_fvopt( v_optional ), m_feopt( e_optional ),
		  m_sEllipse( " node.shape: ellipse" ),
		  m_sBox( " node.shape: box" ),
		  m_sNoarrow( " edge.arrowstyle: none" )
	{}

	std::string m_sGlobal;

	void set_global( const std::string &s ) {
		m_sGlobal = s;
	}

	/** operator ( ) that do the job.
	@param os output stream
	@param gra the given graph
	@param title function object to return a title (string type) for a node,
	  it must accept two parameters: an integer (index of node in graph) and a const
	  reference of the node (vertex).
	@param v_optional function object to return some optional attributes
	  (string type) for a node, it must accept two parameters:
	  an integer (index of node in graph) and a const reference of the node
	  (vertex).
	@param eoptional function object to return some optional attributes
	  (string type) for an edge, it must accept two parameters:
	  an integer (index of source node in graph) and
	  a const reference of the edge.
	*/
	void export_g( std::ostream &os, const GraphT &gra ) {
		// write global attributes
		os << "graph: {\n"
		   << "\t/* global attributes */\n"
		   << "\t" << m_sGlobal << "\n\n"
		   << "\t/* list of nodes */\n";

		std::vector<std::string> vntitles( gra.range(), "-1" );
		// a vector of node titles

		// write nodes
		int i;
		for( i = 0; i < gra.range(); ++i ) {

			vntitles[i] = m_ftitle( i, &gra[i] );

			os << "\tnode: { title:" << '"' << vntitles[i] << '"' << " "
			   << m_fvopt( i, &gra[i] ) << "}\n";
		}

		// write edges
		os << "\n\t/* list of edges */\n";
		typename GraphT::const_e_iterator eit;
		for( i = 0; i < gra.range(); ++i ) {
			for( eit = gra.e_begin( i ); eit != gra.e_end( i ); ++eit ) {
				os << "\tedge: { sourcename: " << '"' << vntitles[i] << '"' << " targetname: "
				   << '"' << vntitles[eit->to()] << '"'
				   << ' ' << m_feopt( i, &( *eit ) )
				   << "}\n";
			}
		}

		os << "}\n";
	}

	// function objects to compute node titles and node/edge attributes
	Ftitle m_ftitle;
	Fv_optional m_fvopt;
	Fe_optional m_feopt;

	// some pre-defined global attributes
	std::string m_sEllipse; // draw nodes as ellipse
	std::string m_sBox; // draw nodes as boxes (default)
	std::string m_sNoarrow; // don't draw edge arrows
};

/**@}*/
}

#endif
