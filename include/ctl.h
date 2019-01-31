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
			virtual void suite_begin(const std::string& description) = 0;
			virtual void suite_end(const std::string& description) = 0;
			virtual void pending_suite(const std::string& description) = 0;
			virtual void completed_test(const std::string& description) = 0;
			virtual void failed_test(const std::string& description, const std::string& error_text) = 0;
			virtual void pending_test(const std::string& description) = 0;
			virtual ~reporter() = default;
	};
}

namespace {
	const std::string grey_esc = "\x1B[90m";
	const std::string green_esc = "\x1B[32m";
	const std::string red_esc = "\x1B[31m";
	const std::string cyan_esc = "\x1B[36m";

	const std::string reset_esc = "\x1B[39m";

	std::string grey(const std::string& text) {
		return grey_esc + text + reset_esc;
	}

	std::string green(const std::string& text) {
		return green_esc + text + reset_esc;
	}

	std::string red(const std::string& text) {
		return red_esc + text + reset_esc;
	}

	std::string cyan(const std::string& text) {
		return cyan_esc + text + reset_esc;
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
				print_indentation();
				std::clog << green(std::to_string(this->completed) + " " + (this->completed == 1 ? "test" : "tests") + " completed") << std::endl;
				std::clog << reset_esc;
			}
			if (this->failed > 0) {
				print_indentation();
				std::clog << red(std::to_string(this->failed) + " " + (this->failed == 1 ? "test" : "tests") + " failed") << std::endl;
			}
			if (this->pending > 0) {
				print_indentation();
				std::clog << cyan(std::to_string(this->pending) + " " + (this->pending == 1 ? "test" : "tests") + " pending") << std::endl;
			}
			std::clog << std::endl;
		}

		void suite_begin(const std::string& description) {
			print_indentation();
			std::clog << description;
			std::clog << std::endl;
			indentation++;
		}

		void suite_end(const std::string& description) {
			indentation--;
		}

		void pending_suite(const std::string& description) {
			print_indentation();
			std::clog << cyan("- ") << cyan(description) << std::endl;
		}

		void completed_test(const std::string& description) {
			completed++;
			print_indentation();
			std::clog << green("✓ ") << grey(description) << std::endl;
		}

		void failed_test(const std::string& description, const std::string& error_text) {
			failed++;
			print_indentation();
			std::clog << red("✖ ") << red(description) << std::endl;
			indentation++;
			print_indentation();
			indentation--;
			std::clog << red(error_text);
			std::clog << std::endl;
		}

		void pending_test(const std::string& description) {
			pending++;
			print_indentation();
			std::clog << cyan("- ") << cyan(description) << std::endl;
		}

	private:
		int indentation = 2;
		int completed = 0;
		int failed = 0;
		int pending = 0;

		void print_indentation() {
			for(int i = 0; i < this->indentation; ++i) {
				std::clog << "  ";
			}
		}
	};
}

namespace {
	std::unique_ptr<ctl::reporter> current_reporter = std::make_unique<ctl::spec_reporter>();

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
	void describe(const std::string& description, std::function<void (void)> suit) {
		current_reporter->suite_begin(description);
		suit();
		current_reporter->suite_end(description);
	}

	void describe(const std::string& description) {
		current_reporter->pending_suite(description);
	}

	void it(const std::string& description, std::function<void (void)> test) {
		try {
			test();
			current_reporter->completed_test(description);
		}
		catch (std::exception &e) {
			current_reporter->failed_test(description, e.what());
		}
	}

	void it(const std::string& description) {
		current_reporter->pending_test(description);
	}

	void expect_ok(const bool condition, const std::string& message = "") {
		if(!condition) {
			throw std::runtime_error(std::string("Expected condition to be true. ") + message);
		}
	}

	void expect_fail(const bool condition, const std::string& message = "") {
		if(condition) {
			throw std::runtime_error(std::string("Expected condition to be false. ") + message);
		}
	}

	template<typename Type>
	void expect_equal(Type actual, Type expected, const std::string& message = "") {
		if(actual != expected) {
			std::stringstream error;
			error << "Expected \"" << actual << "\" to be equal \"" << expected << "\"";
			throw std::runtime_error(error.str());
		}
	}
}

#endif
