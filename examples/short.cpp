#include "../ctl.h"

using namespace ctl;

int main() {
	current_reporter.reset(new ctl::short_reporter());

	describe("Suite", []{
		describe("Nested suite", []{
			it("Success test", []{
			});
			it("Fail test", []{
				expect_ok(1 == 2);
			});
		});
		describe("Pending suite");
		it("Pending test");
	});
}

