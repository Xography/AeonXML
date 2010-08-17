// ----------------------------------------------------------------------------
// Aeon Expat XML Wrapper example
#include "SAXParser.h"
#include "SAXHandler.h"
#include "XMLString.h"
#include "XMLAttributes.h"

#include <iostream>
#include <time.h>
using namespace Aeon;

// ----------------------------------------------------------------------------
// Example Handler
// Note: Since this example makes excessive use of STL cout and strings, 
// output will be relatively slow, processing everything without printing
// is much faster. :) 
class MySaxHandler : public Aeon::SAXHandler {

public:
	
	void startElement(const XMLString& name, const XMLAttributes& attributes) {
		std::cout << "<" << name;
		
		// Just print all attributes
		AttributesVector vec = attributes.vector();

		AttributesVector::const_iterator it;
		for (it = vec.begin(); it != vec.end(); ++it) {
			std::cout <<  it->key << "=\"" << it->value << "\""; 
		}
		
		std::cout << ">";

		// Another example:
		// If we have a tag named item, try to access the attribute id,
		// the get-function returns an empty string if the attribute does not
		// exist.
		if(name.compare("item") == 0) {
			// Do something wit the data.
			XMLString attributeData = attributes.get("id");
		}
	}

	void endElement(const XMLString& name) {
		std::cout << "</" << name << ">";
	}

	void characterData(const XMLString &data) {
		// Data between Tags
		std::cout << data;
	}

	void processingInstruction(const XMLString& target, const XMLString& data) {
		// Ignored for this example.
	}

	void commentData(const XMLString& data) {
		std::cout << "<!-- " << data << "-->";
	}

	void startCData(void) {
		// Ignored for this example.
	}

	void endCData(void) {
		// Ignored for this example.
	}


};

// ----------------------------------------------------------------------------
int main(void) {
	Aeon::SAXParser parser(new MySaxHandler());
	
	clock_t start = clock();

	try {
		parser.parse("test.xml");
	} catch (Aeon::SAXParserException &e) {
		std::cout << e.what();
	}
	std::cout << std::endl << "Parsed in " 
		<< ((double)clock() - start) / CLOCKS_PER_SEC 
		<< " seconds" << std::endl;

	return 0;
}
// ----------------------------------------------------------------------------