#ifndef SPEC_REPORTER_H
#define SPEC_REPORTER_H

#include <iostream>

namespace colors {

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
				std::clog << colors::green_esc;
				std::clog << this->completed << " " << (this->completed==1 ? "test" : "tests") << " completed" << std::endl;
				std::clog << colors::reset_esc;
			}
			if (this->failed > 0) {
				print_indention();
				std::clog << colors::red_esc;
				std::clog << this->failed << " " << (this->failed==1 ? "test" : "tests") << " failed" << std::endl;
				std::clog << colors::reset_esc;
			}
			if (this->pending > 0) {
				print_indention();
				std::clog << colors::cyan_esc;
				std::clog << this->pending << " " << (this->pending==1 ? "test" : "tests") << " pending" << std::endl;
				std::clog << colors::reset_esc;
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
			std::clog << colors::cyan("- ");
			std::clog << colors::cyan(description);
			std::clog << std::endl;
		}

		void completed_test(const char* description) {
			completed++;
			print_indention();
			std::clog << colors::green("✓ ");
			std::clog << colors::grey(description);
			std::clog << std::endl;
		}

		void failed_test(const char* description, const char* error_text) {
			failed++;
			print_indention();
			std::clog << colors::red("✖ ");
			std::clog << colors::red(description);
			std::clog << std::endl;
			indention++;
			print_indention();
			indention--;
			std::clog << colors::red(error_text);
			std::clog << std::endl;
		}

		void pending_test(const char* description) {
			pending++;
			print_indention();
			std::clog << colors::cyan("- ");
			std::clog << colors::cyan(description);
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

#endif
