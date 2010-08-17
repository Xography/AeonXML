/*
* Part of the Aeon Framework
* -----------------------------------------------------------------------------
* Aeon Expat XML Wrapper.
*
* File:		SAXParser.cpp
  Desc.:	SAX Parser Implementation.
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
#include "SAXParser.h"

namespace Aeon {
	// ------------------------------------------------------------------------
	SAXParser::SAXParser(SAXHandlerInterface * handler) 
		:	_handler(handler),
			_parser(0), 
			_bufsize(EXPAT_CHUNK_SIZE),
			_ready(false),
			_failure(false),
			_file(0),
			_fname("") {
		bootstrap();
	}

	// ------------------------------------------------------------------------
	SAXParser::SAXParser(SAXHandlerInterface * handler, size_t chunkSize) 
		:	_handler(handler),
			_parser(0), 
			_bufsize(chunkSize),
			_ready(false),
			_failure(false),
			_file(0),
			_fname("") {
		bootstrap();
	}

	// ------------------------------------------------------------------------
	SAXParser::~SAXParser() {
		_ready = false;

		XML_ParserFree(_parser);
		_parser = 0;

		delete _buffer;
		_buffer = 0;

		if(_file != NULL) {
			fclose(_file);
			_file = 0;
		}
	}

	// ------------------------------------------------------------------------
	void SAXParser::bootstrap() {
		// Allocate buffer
		_buffer = new XML_Char[_bufsize];
	//	memset(_buffer, 0, _bufsize * sizeof(XML_Char));

		// Create Parser
		_parser = XML_ParserCreate(NULL);

		if(_parser != NULL && _buffer != NULL) {
			_ready = true;

			// Register Handlers
			XML_SetUserData(_parser, (void*)this);
			XML_SetElementHandler(_parser, &SAXParser::_handleElementStart, SAXParser::_handleElementEnd);
			XML_SetCharacterDataHandler(_parser, &SAXParser::_handleCharacterData);
			XML_SetProcessingInstructionHandler(_parser, &SAXParser::_handleProcessingInstruction);
			XML_SetCommentHandler(_parser, &SAXParser::_handleComments);
			XML_SetCdataSectionHandler(_parser, &SAXParser::_handleCDATAStart,&SAXParser::_handleCDATAEnd);
			XML_SetDoctypeDeclHandler(_parser, &SAXParser::_handleDoctypeDeclStart, &SAXParser::_handleDoctypeDeclEnd);
			XML_SetUnparsedEntityDeclHandler(_parser, &SAXParser::_handleUnparsedEntityDecl);
			XML_SetNotationDeclHandler(_parser, &SAXParser::_handleNotationDecl);
			XML_SetDoctypeDeclHandler(_parser,  &SAXParser::_handleDoctypeDeclStart,  &SAXParser::_handleDoctypeDeclEnd);
			XML_SetEntityDeclHandler(_parser, &SAXParser::_handleEntityDecl);
			XML_SetSkippedEntityHandler(_parser, &SAXParser::_handleSkippedEntity);

			XML_SetDefaultHandler(_parser, &SAXParser::_handleDefault);
		} else {
			// If anything goes wrong, we 'silently' fail and register the error.
			delete _buffer;
			_buffer = 0;
			_failure = true;
			_ready = false;
		}
	}

	// ------------------------------------------------------------------------
	ssize_t SAXParser::_readblock() {
		if(_file == NULL) return -1;
		size_t _s = fread(_buffer, sizeof(_buffer)[0], _bufsize, _file);
		ssize_t code = (ssize_t)_s;
		
		if(_s < _bufsize) {
			
			// If we're at the end of the file
			if(feof(_file)) {
				_error = XML_ERROR_FINISHED;
				return _s;
			}

			// If there was an error reading the chunk
			if(ferror(_file)) {
				long s = _s;
				_status = XML_STATUS_ERROR;
				_error = XML_ERROR_NO_ELEMENTS;
				fseek(_file, -s, SEEK_CUR);
			}
		}

		// If we've finished reading the file.
		if(_s == 0) {
			_status = XML_STATUS_OK;
			_error = XML_ERROR_FINISHED;
			code = -1;
		}

		return code;
	}

	// ------------------------------------------------------------------------
	bool SAXParser::parse(const std::string& filename) {
		// Open file
		_fname = filename;
		_file = fopen(_fname.c_str(), "r");

		if(_file == NULL) _failure = true;

		ssize_t bytes_read;

		// Ensure that the parser is ready
		if(_failure) {
			throw SAXParserException("Failed to open file: "+_fname);
		}

		if(!_ready)
			throw SAXParserException("Parser not ready.");

		// Loop, reading the XML source block by block 
		while((bytes_read = _readblock()) >= 0) {
			if(bytes_read > 0) {
				XML_Status status = XML_Parse(_parser, _buffer, bytes_read, XML_FALSE);

				// In case of Error
				if(status != XML_STATUS_OK) {
					_status = status;
					_error = XML_GetErrorCode(_parser);
					fclose(_file);
					_ready = false;
					
					throw SAXParserException(_fname + ": " + xmlErrorString());
				}

				// Break on successful "short read", in event of EOF
				if(_error == XML_ERROR_FINISHED)
					break;
			}

		}

		// Finalize the parser
		if((_status == XML_STATUS_OK) || (_status == XML_ERROR_FINISHED)) {
			XML_Parse(_parser, _buffer, 0, XML_TRUE);
			return true;
		}
		
		// Return false in case of error
 		return false;
	}

	// ------------------------------------------------------------------------
	XML_Error SAXParser::xmlError() {
		return _error;
	}

	// ------------------------------------------------------------------------
	std::string SAXParser::xmlErrorString() {
		return XML_ErrorString(xmlError()); 

	}

	// ------------------------------------------------------------------------
	XML_Status SAXParser::xmlStatus(){
		return _status;
	}

	// ------------------------------------------------------------------------
	bool SAXParser::isReady() {
		return _ready;
	}

	// ------------------------------------------------------------------------
	// Static handlers, very repetitive stuff ahead.
	//
	// ------------------------------------------------------------------------
	void SAXParser::_handleElementStart(void *userData,
		const XML_Char *name, const XML_Char **atts) {
			SAXParser* self = reinterpret_cast<SAXParser*>(userData);

			if (self->_handler)
				self->_handler->_StartElement(name, atts);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleElementEnd(void *userData,
		const XML_Char *name) {
			SAXParser* self = reinterpret_cast<SAXParser*>(userData);

			if (self->_handler)
				self->_handler->_EndElement(name);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleCharacterData(void *userData,
		const XML_Char *s, int len) {
			SAXParser* self = reinterpret_cast<SAXParser*>(userData);
			if (self->_handler)
				self->_handler->_CharacterData(s, len);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleProcessingInstruction(void *userData,
		const XML_Char *target, const XML_Char *data) {
			SAXParser* self = reinterpret_cast<SAXParser*>(userData);

			if (self->_handler)
				self->_handler->_ProcessingInstruction(target, data);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleComments(void *userData,
		const XML_Char *data) {
			SAXParser* self = reinterpret_cast<SAXParser*>(userData);

			if (self->_handler)
				self->_handler->_CommentData(data);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleDefault(void *userData,
		const XML_Char *s, int len) {
			SAXParser* self = reinterpret_cast<SAXParser*>(userData);

			if (self->_handler)
				self->_handler->_DefaultHandler(s, len);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleCDATAStart(void *userData) {
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_CDataStart();
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleCDATAEnd(void *userData) {
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_CDataEnd();
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleDoctypeDeclStart(void* userData, 
		const XML_Char* doctypeName, const XML_Char *systemId, 
		const XML_Char* publicId, int hasInternalSubset) {

		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_DoctypeDeclStart(doctypeName, systemId, publicId, hasInternalSubset);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleDoctypeDeclEnd(void* userData) {
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_DoctypeDeclEnd();
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleNotationDecl(void* userData, 
		const XML_Char* notationName, const XML_Char* base, 
		const XML_Char* systemId, const XML_Char* publicId) {

		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_NotationDecl(notationName, base, systemId, publicId);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleStartNamespaceDecl(void* userData, 
		const XML_Char* prefix, const XML_Char* uri) {

		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_StartNamespaceDecl(prefix, uri);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleEndNamespaceDecl(void* userData, const XML_Char* prefix) {
		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_EndNamespaceDecl(prefix);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleEntityDecl(void *userData, const XML_Char *entityName, 
		int isParamEntity, const XML_Char *value, int valueLength, 
		const XML_Char *base, const XML_Char *systemId, 
		const XML_Char *publicId, const XML_Char *notationName) {

			SAXParser* self = reinterpret_cast<SAXParser*>(userData);

			if (self->_handler)
				self->_handler->_EntityDecl(entityName, isParamEntity, 
				value, valueLength, base, systemId, publicId, notationName);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleSkippedEntity(void* userData, 
		const XML_Char* entityName, int isParameterEntity) {

		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_SkippedEntity(entityName, isParameterEntity);
	}

	// ------------------------------------------------------------------------
	void SAXParser::_handleUnparsedEntityDecl(void* userData, 
		const XML_Char* entityName, const XML_Char* base, 
		const XML_Char* systemId, const XML_Char* publicId, 
		const XML_Char* notationName) {

		SAXParser* self = reinterpret_cast<SAXParser*>(userData);

		if (self->_handler)
			self->_handler->_UnparsedEntityDecl(entityName, base, systemId, publicId, notationName);
	}

}