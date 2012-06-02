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


/**@file gexception.h

  Exception definitions.

  Aiguo Fei, 2000-2001

*/

#ifndef _AFG_GEXCEPTION_H_
#define _AFG_GEXCEPTION_H_
/*
#include <string>
#include <exception>
#include <stdexcept>
*/

#include "afl/exceptions.hpp"

namespace afg
{

/**@addtogroup exceptions
*/

/**@{
*/

/// invalid path.
class invalid_path: public afl::general_except<std::string>
{
public:
	explicit invalid_path( const std::string &s = std::string( "invalid path" ) )
		: afl::general_except<std::string>( s )
	{}
	virtual ~invalid_path( )
	{}
}
;

/**@}*/ // doxygen group exceptions

} // end of namespace afg

#endif // end of _AFG_GEXCEPTION_H_
