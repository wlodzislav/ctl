#ifndef REPORTER_H
#define REPORTER_H

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

#endif
