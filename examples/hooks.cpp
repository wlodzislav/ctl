#include <iostream>
#include "../ctl.h"

using namespace ctl;

int main() {
	before([] {
		std::cout << "before all suits" << std::endl;
	});
	after([] {
		std::cout << "after all suits" << std::endl;
	});

	describe("Suite", []{
		before([] {
			std::cout << "before all tests in suit" << std::endl;
		});
		after([] {
			std::cout << "after all tests in suit" << std::endl;
		});
		before_each([] {
			std::cout << "before each test in suit" << std::endl;
		});
		after_each([] {
			std::cout << "after each test in suit" << std::endl;
		});

		it("Test 1", []{
			std::cout << "Test 1" << std::endl;
		});

		describe("Nested suite", []{
			before([] {
				std::cout << "nested suit: before all tests in suit" << std::endl;
			});
			after([] {
				std::cout << "nested suit: after all tests in suit" << std::endl;
			});
			before_each([] {
				std::cout << "nested suit: before each test in suit" << std::endl;
			});
			after_each([] {
				std::cout << "nested suit: after each test in suit" << std::endl;
			});

			it("Nested suit test 1", []{
				std::cout << "Nested suit test 1" << std::endl;
			});

			it("Nested suit test 2", []{
				std::cout << "Nested suit test 2" << std::endl;
			});
		});

		it("Test 2", []{
			std::cout << "Test 2" << std::endl;
		});

		describe("Pending suite");

		it("Pending test");
	});
}

