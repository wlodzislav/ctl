#ifndef ASSERT_H
#define ASSERT_H

#include <sstream>
#include <stdexcept>

namespace assert {

	void ok(const bool condition, const char* maessage = "", const char* file = "", const int line = -1) {
		if(!condition) {
			throw std::runtime_error("Expected condition to be true");
		}
	}

	void fail(const bool condition, const char* message = "", const char* file = "", const int line = -1) {
		if(condition) {
			throw std::runtime_error("Expected condition to be false");
		}
	}

	template<typename Type>
	void equal(Type actual, Type expected, const char* message = "", const char* file = "", const int line = -1) {
		if(actual != expected) {
			std::stringstream error;
			error << "Expected \"" << actual << "\" to be equal \"" << expected << "\"";
			throw std::runtime_error(error.str());
		}
	}
}


#endif
