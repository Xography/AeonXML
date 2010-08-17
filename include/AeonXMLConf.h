/*
* Part of the Aeon Framework
* -----------------------------------------------------------------------------
* Aeon Expat XML Wrapper.
*
* File:		AeonXMLConf.h
  Desc.:	Provides Baseline Configuration for Expat to be used by AeonXML.
*
* Copyright (c) 2009-2010, gatewayheaven.com. 
* Licensed under the Boost Software License.
*
* -----------------------------------------------------------------------------
*/
#ifndef _AeonXMLConf_h_
#define _AeonXMLConf_h_

#include "AeonPlatform.h"

// Disable expats Export/Import symbols since we're using it standalone
#define XMLCALL
#define XMLIMPORT

#if AEON_PLATFORM == AEON_WIN32
	// We'll disable the Microsoft warning, that some C functions are unsafe, 
	// since they're proven rather save every day.
	#ifdef _MSC_VER
		#pragma warning(disable : 4996)
	#endif
	
	/* we will assume all Windows platforms are little endian */
	#define BYTEORDER 1234

	/* Windows has memmove() available. */
	#define HAVE_MEMMOVE

	#define XML_CONTEXT_BYTES 1024

#elif AEON_PLATFORM == AEON_MACOS
	
	/* 1234 = LIL_ENDIAN, 4321 = BIGENDIAN */
	#define BYTEORDER  4321

	/* Define to 1 if you have the `bcopy' function. */
	#undef HAVE_BCOPY

	/* Define to 1 if you have the `memmove' function. */
	#define HAVE_MEMMOVE

	/* Define to 1 if you have a working `mmap' system call. */
	#undef HAVE_MMAP

	/* Define to 1 if you have the <unistd.h> header file. */
	#undef HAVE_UNISTD_H

	/* whether byteorder is bigendian */
	#define WORDS_BIGENDIAN

	/* Define to specify how much context to retain around the current parse
	   point. */
	#undef XML_CONTEXT_BYTES

	/* Define to make parameter entity parsing functionality available. */
	#define XML_DTD

	/* Define to make XML Namespaces functionality available. */
	#define XML_NS

	/* Define to empty if `const' does not conform to ANSI C. */
	#undef const

	/* Define to `long' if <sys/types.h> does not define. */
	#define off_t  long

	/* Define to `unsigned' if <sys/types.h> does not define. */
	#undef size_t

	#define XML_NS 1
	#define XML_DTD 1
	#define XML_CONTEXT_BYTES 1024
#endif

#define EXPAT_CHUNK_SIZE 10240


#endif