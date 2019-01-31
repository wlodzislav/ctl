# CTL - C++ Testing Library

## Features

## Usage

	ctl::describe("Smth", []() {
		ctl::describe("part of Smth", []() {
			ctl::it("should do smth", []() {
				/* do smth*/
			});
		});
	});

## Public API

	ctl::describe(const char* description, std::function<void (void)> body);

	ctl::it(const char* description, std::function<void (void)> body);
