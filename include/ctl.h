#ifndef CTL_H
#define CTL_H

#include <functional>
#include <memory>

#include "reporter.h"
#include "reporters/spec.h"

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

#endif
