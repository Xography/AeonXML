/*
* Part of the Aeon Framework
* -----------------------------------------------------------------------------
* Aeon Expat XML Wrapper.
*
* File:		SAXHandlerAdapter.h
  Desc.:	Adapter to the SAXHandler Interface.
* Author:	Felix Bruckner <xography@gatewayheaven.com>
*
* Copyright (c) 2009-2010, gatewayheaven.com. 
* Licensed under the Boost Software License.
*
* -----------------------------------------------------------------------------
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
* 
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
* -----------------------------------------------------------------------------
*/
#ifndef _SAXHandlerAdapter_h_
#define _SAXHandlerAdapter_h_

#include "SAXHandlerInterface.h"

namespace Aeon {

	/**
	 * Adapter for the SAXHandler Interface, every function is implemented
	 * as an empty method.
	 * This adapter comes in handy if you don't want to fill your implementation
	 * with empty function definitions.
	 */ 
	class SAXHandlerAdapter : public SAXHandlerInterface {
	public:
		virtual void _StartElement(const XML_Char *name, const XML_Char **atts) { };

		virtual void _EndElement(const XML_Char *name) { };

		virtual void _CharacterData(const XML_Char *s, int len) { };

		virtual void _ProcessingInstruction(const XML_Char *target, 
			const XML_Char *data) { };

		virtual void _CommentData(const XML_Char *data) { }

		virtual void _DefaultHandler(const XML_Char *s, int len) { };

		virtual void _CDataStart(void) { };

		virtual void _CDataEnd(void) { };

		virtual void _DoctypeDeclStart(const XML_Char* doctypeName, 
			const XML_Char *systemId, const XML_Char* publicId, 
			int hasInternalSubset) { };

		virtual void _DoctypeDeclEnd(void) { };

		virtual void _NotationDecl(const XML_Char* notationName, 
			const XML_Char* base, const XML_Char* systemId, 
			const XML_Char* publicId) { };

		virtual void _StartNamespaceDecl(const XML_Char* prefix, 
			const XML_Char* uri) { };

		virtual void _EndNamespaceDecl(const XML_Char* prefix) { };

		virtual void _EntityDecl(const XML_Char *entityName, int isParamEntity, 
			const XML_Char *value, int valueLength, const XML_Char *base, 
			const XML_Char *systemId, const XML_Char *publicId, 
			const XML_Char *notationName){ };

		virtual void _SkippedEntity(const XML_Char* entityName, 
			int isParameterEntity) { };

		virtual void _UnparsedEntityDecl(const XML_Char* entityName, 
			const XML_Char* base, const XML_Char* systemId, 
			const XML_Char* publicId, const XML_Char* notationName) { };
	};
}
#endif