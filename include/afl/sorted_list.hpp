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
@file sorted_list.hpp

@author Aiguo Fei

*/

#ifndef _AFL_SORTED_LIST_H_
#define _AFL_SORTED_LIST_H_

#include <list>
#include <functional>

namespace afl
{


/** A list which is always sorted.

  T is the data type of element kept in the list.

  Provides:
  (1)ordered element access as provided by priority_queue;
  (2)access to element one by one through iterator.

  By default, std::less( ) is used as comparison predicate when
  inserting new element, thus elements are sorted from small to large.
  Use front( ) to access the smallest onea and back( ) to access
  the largest one. Use pop_front( ) and pop_back( ) to remove the
  smallest and largest one, respectively.

  sorted_list inherites from std::list, most member funcations of list
  are still available; however, in order to maintain the
  ordered property, position specific insert operations
  are not allowed and related functions
  (including the followings: push_front( ), push_back( ), insert( iterator pos, ...) )
  are overloaded as "private".

  @version 0.1a, November 2000
  @version 0.2b, March 2001
*/
template < class T, class A = std::allocator<T>,  class Compare = std::less<T> >
class sorted_list: public std::list<T, A>
{
protected:
	Compare m_comp;
public:
	/// Constructor, default comparison predicate is less( ).
	sorted_list( const Compare &comp = Compare() )
		: std::list<T, A>(), m_comp( comp )
	{}

	/// Insert element x into the list, keep ordered property.
	void push( const T &x ) {
		typename std::list<T, A>::iterator it = this->begin();
		while( it != this->end() && m_comp( *it, x ) ) {
			++it;
		}
		(( std::list<T, A>* )this )->insert( it, x );
	}

private:
	void push_front( const T &_X );
	void push_back( const T &_X );

	template <class InputIterator>
	void assign( InputIterator _F, InputIterator _L );

	void assign( typename std::list<T, A>::size_type _N, const T &_X = T() );

	typename std::list<T, A>::iterator insert(
		typename std::list<T, A>::iterator _P, const T &_X = T() );

	void insert( typename std::list<T, A>::iterator _P,
				 typename std::list<T, A>::size_type _M, const T &_X );

	void insert( typename std::list<T, A>::iterator _P,
				 const T *_F, const T *_L );

	template <class InputIterator>
	void insert( typename std::list<T, A>::iterator _P, InputIterator _F,
				 InputIterator _L );

};

}// end of namespace afl

#endif
