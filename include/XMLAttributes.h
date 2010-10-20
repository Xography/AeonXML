/*
* Part of the Aeon Framework
* -----------------------------------------------------------------------------
* Aeon Expat XML Wrapper.
*
* File:		XMLAttributes.h
  Desc.:	A class to represent XML Attributes
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
#ifndef _XMLAttributes_h_
#define _XMLAttributes_h_
#include <vector>
#include <vector>
#include "AeonXMLFwd.h"

#include "XMLString.h"

namespace Aeon {

	/**
	 * A very basic XML Attribute representation, consisting of a key and a value.
	 *
	 */ 
	typedef struct {
		XMLString key;
		XMLString value;
	} Attribute;

	typedef std::vector<Attribute> AttributesVector;
	
	/**
	* Very basic and quick implementation of a supporting XMLAttributes class
	* helping to (temporarily) store XML Element Attributes in a vector.
	*
	*/
	class XMLAttributes {
	public:
		
		/**
		 * Inserts a new Attribute Pair (Key and Value) into the Vector.
		 *
		 * @param  key the key of the attribute.
		 * @param  value the value of the attribute.
		 */ 
		void insert(const XMLString &key, const XMLString &value) {
			Attribute _a;
			_a.key = key;
			_a.value = value;
			vec.push_back(_a);
		}

		/**
		 * Look up a key of the attribute list, if the key is found, the value
		 * will be returned, an empty XMLString otherwise.
		 *
		 * @param key the key of the attribute.
		 * @return the value of the attribute.
		 */ 
		const XMLString& get(const XMLString &key) const {
			int pos = find(key);
			if(pos != -1) {
				return vec.at(pos).value;
			} else {
				return empty;
			}
		}

		const XMLString& get(const int pos) const {
			return vec.at(pos).value;
		}
		
		int size() const {
			return vec.size();
		}

		/**
		* Clears the attribute list.
		*/
		const void clear() {
			vec.clear();
		}

		const AttributesVector& vector() const {
			return vec;
		} 
	private:
		AttributesVector vec;
		std::string empty;

		// Used to find an element in the  list, return -1 if element is not found
		size_t find(const XMLString &key) const {
			size_t i = 0;

			AttributesVector::const_iterator it;
			for (it = vec.begin(); it != vec.end(); ++it) {
				if (it->key == key) 
					return i;
				++i;
			}

			return -1;
		}
	};
}

#endif
