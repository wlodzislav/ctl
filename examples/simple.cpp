#include "../ctl.h"

using namespace ctl;

int main() {
	describe("Suite", []{
		describe("Nested suite", []{
			it("Success test", []{
				expect_fail(1 == 2);
			});
			it("Fail test", []{
				expect_ok(1 == 2);
			});
		});
		describe("Pending suite");
		it("Pending test");
	});
}

