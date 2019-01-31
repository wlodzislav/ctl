#include "../ctl.h"

using namespace ctl;

// Example of class that could be used in expect_equal
class custom {
public:
	custom(int a, int b): a(a), b(b) {}
	bool operator==(const custom& other) const {
		return this->a == other.a && this->b == other.b;
	}
	int a = 0;
	int b = 0;
};

std::ostream& operator<<(std::ostream &ss, const custom& c) {
	return ss << "{ a: " << c.a << ", b: " << c.b << " }";
}

int main() {
	describe("Suite", []{
		describe("Nested suite", []{
			it("Success test", []{
			});
			it("Fail test", []{
				expect_ok(1 == 2);
			});
			it("Pending test");
			it("Success test 2", []{
			});
			it("Another failing test with expect_equal", []{
				custom a(1, 2);
				custom b(3, 4);
				expect_equal(a, b);
			});
		});
	});
	describe("Pending suite");
}

