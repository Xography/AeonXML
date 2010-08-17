/*
* Part of the Aeon Framework
* -----------------------------------------------------------------------------
* Aeon Expat XML Wrapper.
*
* File:		AeonPlatform.h
  Desc.:	Used to determine the platform we're compiling on.
*
* Copyright (c) 2009-2010, gatewayheaven.com. 
* Licensed under the Boost Software License.
*
* -----------------------------------------------------------------------------
*/
#ifndef _AeonPlatform_h_
#define _AeonPlatform_h_

#define AEON_WIN32 1
#define AEON_MACOS 2
#define AEON_LINUX 3

#if defined( __WIN32__ ) || defined( _WIN32 )
#		define AEON_PLATFORM AEON_WIN32
#elif defined(__APPLE__) || defined(__MACH__)
#		define AEON_PLATFORM AEON_MACOS
#else
#		define AEON_PLATFORM AEON_LINUX
#endif

#endif