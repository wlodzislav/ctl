#ifndef CTL_H
#define CTL_H

#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace ctl {
	class reporter {
		public:
			virtual void before() = 0;
			virtual void after() = 0;
			virtual void suite_begin(const char* description) = 0;
			virtual void suite_end(const char* description) = 0;
			virtual void pending_suite(const char* description) = 0;
			virtual void completed_test(const char* description) = 0;
			virtual void failed_test(const char* description, const char* error_text) = 0;
			virtual void pending_test(const char* description) = 0;
			virtual ~reporter() = default;
	};
}

namespace {
	const char* grey_esc = "\x1B[90m";
	const char* green_esc = "\x1B[32m";
	const char* red_esc = "\x1B[31m";
	const char* cyan_esc = "\x1B[36m";

	const char* reset_esc = "\x1B[39m";

	std::string grey(const char* text) {
		return std::string(grey_esc) + text + reset_esc;
	}

	std::string green(const char* text) {
		return std::string(green_esc) + text + reset_esc;
	}

	std::string red(const char* text) {
		return std::string(red_esc) + text + reset_esc;
	}

	std::string cyan(const char* text) {
		return std::string(cyan_esc) + text + reset_esc;
	}

}

namespace ctl {

	class spec_reporter : public reporter {
	public:
		void before() {
			std::clog << std::endl;
		}

		void after() {
			std::clog << std::endl;
			if (this->completed > 0) {
				print_indention();
				std::clog << green_esc;
				std::clog << this->completed << " " << (this->completed == 1 ? "test" : "tests") << " completed" << std::endl;
				std::clog << reset_esc;
			}
			if (this->failed > 0) {
				print_indention();
				std::clog << red_esc;
				std::clog << this->failed << " " << (this->failed == 1 ? "test" : "tests") << " failed" << std::endl;
				std::clog << reset_esc;
			}
			if (this->pending > 0) {
				print_indention();
				std::clog << cyan_esc;
				std::clog << this->pending << " " << (this->pending == 1 ? "test" : "tests") << " pending" << std::endl;
				std::clog << reset_esc;
			}
			std::clog << std::endl;
		}

		void suite_begin(const char* description) {
			print_indention();
			std::clog << description;
			std::clog << std::endl;
			indention++;
		}

		void suite_end(const char* description) {
			indention--;
		}

		void pending_suite(const char* description) {
			print_indention();
			std::clog << cyan("- ");
			std::clog << cyan(description);
			std::clog << std::endl;
		}

		void completed_test(const char* description) {
			completed++;
			print_indention();
			std::clog << green("✓ ");
			std::clog << grey(description);
			std::clog << std::endl;
		}

		void failed_test(const char* description, const char* error_text) {
			failed++;
			print_indention();
			std::clog << red("✖ ");
			std::clog << red(description);
			std::clog << std::endl;
			indention++;
			print_indention();
			indention--;
			std::clog << red(error_text);
			std::clog << std::endl;
		}

		void pending_test(const char* description) {
			pending++;
			print_indention();
			std::clog << cyan("- ");
			std::clog << cyan(description);
			std::clog << std::endl;
		}

	private:
		int indention = 2;
		int completed = 0;
		int failed = 0;
		int pending = 0;

		void print_indention() {
			for(int i = 0; i < this->indention; ++i) {
				std::clog << "  ";
			}
		}
	};
}

namespace {
	std::unique_ptr<ctl::reporter> current_reporter{new ctl::spec_reporter()};

	struct guard {
		guard() {
			current_reporter->before();
			}
		~guard() {
			current_reporter->after();
			}
	} guard;
}

namespace ctl {
	void describe(const char* description, std::function<void (void)> body) {
		current_reporter->suite_begin(description);
		body();
		current_reporter->suite_end(description);
	}

	void describe(const char* description) {
		current_reporter->pending_suite(description);
	}

	void it(const char* description, std::function<void (void)> body) {
		try {
			body();
			current_reporter->completed_test(description);
		}
		catch (std::exception &e) {
			current_reporter->failed_test(description, e.what());
		}
	}

	void it(const char* description) {
		current_reporter->pending_test(description);
	}
}

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
