/**
@file exceptions.hpp

  Exception definitions.

  Aiguo Fei, 2000-2001

*/

#ifndef _AFL_EXCEPTIONS_H_
#define _AFL_EXCEPTIONS_H_

#include <string>
#include <exception>
#include <stdexcept>

namespace afl
{

/**@defgroup exceptions Exception classes
*/

/**@{*/

/// General exception
template < class str_type = std::string >
class general_except//: public std::exception
{
	str_type m_swhat;
public:
	/** Constructor
	@param s error message for this exception
	*/
	explicit general_except(
		const str_type &s = str_type( "general exception" ) )
		: m_swhat( s )
	{}

	virtual ~general_except( )
	{}

	/** Get the error message.
	*/
	virtual const char *what() const {
		return m_swhat.c_str();
	}

	virtual const str_type &what_s() const {
		return m_swhat;
	}
};

/** Unknown exception.
No specific information will be given for this exception.
*/
template < class str_type = std::string >
class unknown_except: public general_except<str_type>
{
public:
	explicit unknown_except(
		const str_type &s = str_type( "unknown exception" ) )
		: general_except<str_type>( s )
	{}

	virtual ~unknown_except( )
	{}
}
;

/// Initialization failure exception.
template < class str_type = std::string >
class init_failed: public general_except<str_type>
{
public:
	///@param s specifies where or how the initialization failed
	///(such as " in myclass::myclass( ) (memory allocation)" ).
	explicit init_failed(
		const str_type &s = str_type( "" ) )
		: general_except<str_type>( str_type( "initialization failed" ) + s )
	{}

	virtual ~init_failed( )
	{}
}
;

/// Memory allocation failure exception.
template < class str_type = std::string >
class mem_alloc_failed: public general_except<str_type>
{
public:
	///@param s specifies where or how the initialization failed
	///(such as " in myclass::myclass( ) (memory allocation)" ).
	explicit mem_alloc_failed(
		const str_type &s )
		: general_except<str_type>( str_type( "memory allocation failed" ) + s )
	{}

	explicit mem_alloc_failed( const char *p = NULL ):
		general_except<str_type>( str_type( "memory allocation failed" )
								  + str_type( p ) )
	{}

	virtual ~mem_alloc_failed( )
	{}
}
;

/**@}*/

} // end of namespace afl

#endif // end of _AFL_EXCEPTIONS_H_
