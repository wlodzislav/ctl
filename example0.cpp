#include "include/ctl.h"

using namespace ctl;

// Example of class that could be used in expect_equal
class custom {
public:
	custom(int a, int b): a(a), b(b) {}
	bool operator==(const custom& other) const {
		return a == other.a && b == other.b;
	}
private:
	friend std::ostream& operator<<(std::ostream &ss, const custom& a);
	int a = 0;
	int b = 0;
};

std::ostream& operator<<(std::ostream &ss, const custom& c) {
	return ss << "{ a: " << c.a << ", b: " << c.b << " }";
}

int main() {
	// change reporter, if not changed spec_reporter is used
	current_reporter.reset(new ctl::spec_reporter());
	// current_reporter.reset(new ctl::short_reporter());

	describe("Some suite", []{
		describe("Inner suite", []{
			it("Inner test", []{
				expect_ok(1 == 1);
			});
			it("Another inner test", []{
				expect_fail(1 == 2);
			});
			it("Failed inner test", []{
				custom a(1, 2);
				custom b(3, 4);
				expect_equal(a, b);
			});
			describe("Pending inner suite");
		});
		it("Some test", []{
		});
		it("Pending test");
	});
}
