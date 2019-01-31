#include "include/ctl.h"
#include "include/assert.h"

using namespace ctl;
int main() {
	describe("Some suite", []{
		describe("Inner suite", []{
			it("Inner test", []{
			});
			it("Another inner test", []{
			});
			it("Failed inner test", []{
					assert::ok(1 == 2);
			});
			describe("Pending inner suite");
		});
		it("Some test", []{
		});
		it("Pending test");
	});
}
