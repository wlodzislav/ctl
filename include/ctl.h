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
			std::cout << std::endl;
		}

		void after() {
			std::cout << std::endl;
			if (this->completed > 0) {
				std::cout << padding() << green(std::to_string(this->completed) + " " + (this->completed == 1 ? "test" : "tests") + " completed") << std::endl;
			}
			if (this->failed > 0) {
				std::cout << padding() << red(std::to_string(this->failed) + " " + (this->failed == 1 ? "test" : "tests") + " failed") << std::endl;
			}
			if (this->pending > 0) {
				std::cout << padding() << cyan(std::to_string(this->pending) + " " + (this->pending == 1 ? "test" : "tests") + " pending") << std::endl;
			}
			std::cout << std::endl;
		}

		void suite_begin(const std::string& description) {
			std::cout << padding() << description << std::endl;
			indentation++;
		}

		void suite_end(const std::string& description) {
			indentation--;
		}

		void pending_suite(const std::string& description) {
			std::cout << padding() << cyan("- ") << cyan(description) << std::endl;
		}

		void completed_test(const std::string& description) {
			completed++;
			std::cout << padding() << green("✓ ") << grey(description) << std::endl;
		}

		void failed_test(const std::string& description, const std::string& error_text) {
			failed++;
			std::cout << padding() << red("✖ ") << red(description) << std::endl;
			std::cout << padding() << "  " << red(error_text) << std::endl;
		}

		void pending_test(const std::string& description) {
			pending++;
			std::cout << padding() << cyan("- ") << cyan(description) << std::endl;
		}

	private:
		int indentation = 0;
		int completed = 0;
		int failed = 0;
		int pending = 0;

		std::string padding() {
			return std::string(indentation * 2, ' ');
		}
	};

	class short_reporter : public reporter {
	public:
		void before() {}

		void after() {
			std::cout << std::endl;
			if (this->completed > 0) {
				std::cout << green(std::to_string(this->completed) + " " + (this->completed == 1 ? "test" : "tests") + " completed") << std::endl;
			}
			if (this->failed > 0) {
				std::cout << red(std::to_string(this->failed) + " " + (this->failed == 1 ? "test" : "tests") + " failed") << std::endl;
			}
			if (this->pending > 0) {
				std::cout << cyan(std::to_string(this->pending) + " " + (this->pending == 1 ? "test" : "tests") + " pending") << std::endl;
			}
			std::cout << std::endl;
		}

		void suite_begin(const std::string& description) {}

		void suite_end(const std::string& description) {}

		void pending_suite(const std::string& description) {}

		void completed_test(const std::string& description) {
			completed++;
		}

		void failed_test(const std::string& description, const std::string& error_text) {
			failed++;
			std::cout << red("✖ ") << red(description) << std::endl;
			std::cout << "  " << red(error_text);
			std::cout << std::endl;
		}

		void pending_test(const std::string& description) {
			pending++;
		}

	private:
		int completed = 0;
		int failed = 0;
		int pending = 0;
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

	void expect_ok(bool condition, const std::string& message = "") {
		if(!condition) {
			throw std::runtime_error(std::string("Expected condition to be true. ") + message);
		}
	}

	void expect_fail(bool condition, const std::string& message = "") {
		if(condition) {
			throw std::runtime_error(std::string("Expected condition to be false. ") + message);
		}
	}
 
	template<typename T>
	void expect_equal(const T& actual, const T& expected, const std::string& message = "") {
		if(!(actual == expected)) {
			std::stringstream error;
			error << "Expected \"" << actual << "\" to be equal \"" << expected << "\"";
			throw std::runtime_error(error.str());
		}
	}
}

#endif
