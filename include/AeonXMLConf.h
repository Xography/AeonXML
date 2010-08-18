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
#endif

/* we will assume all platforms are little endian */
#define BYTEORDER 1234

/* Windows has memmove() available. */
#define HAVE_MEMMOVE

#define XML_CONTEXT_BYTES 1024


#define EXPAT_CHUNK_SIZE 10240


#endif
