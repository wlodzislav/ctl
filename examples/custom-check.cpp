#include "../ctl.h"

using namespace ctl;

// Example of compatible check function
void is_equal_2(int a) {
	if(a != 2) {
		throw std::runtime_error("Error: Should be equal to 2");
	}
}

int main() {
	describe("Some suite", []{
		it("Some failed test", []{
			is_equal_2(1);
		});
	});
}
