/*
* Part of the Aeon Framework
* -----------------------------------------------------------------------------
* Aeon Expat XML Wrapper.
*
* File:		XMLString.h
  Desc.:	String Wrapper.
*
* Copyright (c) 2009-2010, gatewayheaven.com. 
* Licensed under the Boost Software License.
*
* -----------------------------------------------------------------------------
*/
#ifndef _XMLString_h_
#define _XMLString_h_
#include <string>

namespace Aeon {
	typedef std::string XMLString;

	namespace XMLUtility {
		inline bool equals(const XMLString& lhs, const XMLString& rhs) {
			return (rhs.compare(lhs) == 0);
		}
	}
}

#endif