#include "include/ctl.h"

using namespace ctl;
int main() {
	current_reporter.reset(new ctl::spec_reporter()); // to change reporter

	describe("Some suite", []{
		describe("Inner suite", []{
			it("Inner test", []{
				expect_ok(1 == 1);
			});
			it("Another inner test", []{
				expect_fail(1 == 2);
			});
			it("Failed inner test", []{
				expect_equal(1, 2);
			});
			describe("Pending inner suite");
		});
		it("Some test", []{
		});
		it("Pending test");
	});
}
