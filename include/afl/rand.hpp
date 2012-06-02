/*@copyright

Copyright (c) 2000-2012 Aiguo Fei
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

/**
@file rand.hpp

@author Aiguo Fei
@version 1.0b, 1998
@version 1.1, 2012
Random number related utilities.

*/

#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef _AFL_RAND_H_
#define _AFL_RAND_H_

namespace afl
{

namespace rand
{
/**@defgroup rand Random number related utilities
*/

/**@{*/

/** Initialize the random seed with system time.
    Use system time divided by a specific integer (default is 111193) as the seed.
    Thus makes it possible for a program to initialize the seed with
    a different number every time it runs though the integer passed to init_rand()
    is the same.

    @return none
*/
inline void init( int n = 111193 )
{
	if( n <= 0 ) {
		n = -n;
	}

	int rt = time( NULL ) % n;
	srand( rt );
	// initialize random seed with a pretty good random number
}

/** Generate a random number within the given range.
  @return a double random number between d1 and d2
*/
inline double gen( double d1, double d2 )
{
	if( d1 == d2 ) {
		return d1;
	}

	return ((( double )::rand() / ( double )RAND_MAX ) * ( d2 - d1 ) + d1 );
}

/** Generate a random number within the given range.
    @return a double random number between 0 and d
*/
inline double gen( double d = 1.0 )
{
	if( d == 0 ) {
		return d;
	}

	return ((( double )::rand() / ( double )RAND_MAX ) * d );
}

/** function object to return a constant number.
*/
struct CConst {
	/** Constructor to init object with a double number that
	will be returned every time member function gen_rand() is called
	or operator() is called.
	*/
	CConst( double d )
		: m_dconst( d )
	{}

	/** Operator to return the constant number given at construction. */
	double operator()( void ) const {
		return m_dconst;
	}

	/** Function to return the constant number given at construction. */
	double gen_rand( void ) const {
		return m_dconst;
	}

	double m_dconst;
};

/** function object to generate uniformly distributed random numbers
  within a range.
*/
struct CUniform {
	/** Constructor to init object with a range within which to generate
	random numbers. */
	CUniform( double dmax, double dmin = 0 )
		: m_maxt( dmax ), m_mint( dmin )
	{}

	/** Operator to return a random number within the given range. */
	double operator()( void ) const {
		return afl::rand::gen( m_mint, m_maxt );
	}

	/** Function to return a random number within the given range. */
	double gen_rand( void ) const {
		return afl::rand::gen( m_mint, m_maxt );
	}

	double m_maxt;
	double m_mint;
};

/** function object to generate exponentially distributed random numbers
  with a given average.
*/
struct CExp {
	/** Constructor to init object with the average.
	@param davg the average; it has to be >0, otherwise it will be set to 1.0.
	*/
	CExp( double davg ) {
		if( davg <= 0 ) {
			m_avg = 1.0;
		} else {
			m_avg = davg;
		}
	}

	/** Operator to return a random number */
	double operator()( void ) const {
		return gen_rand( );
	}

	/** Function to return a random number */
	double gen_rand( void ) const {
		double x;
		do {
			x = afl::rand::gen( 0, 1.0 );
		} while( x == 0 ); // avoid the unlikely event

		return -1.0 * m_avg * log( x );
	}

	double m_avg;
};

/**@}*/

} // end of namespace rand

} // end of namespace afl

#endif
