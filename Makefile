CXX = clang++
CFLAGS = -std=c++14

EXAMPLES = $(patsubst %.cpp, %, $(wildcard examples/*.cpp))
.PHONY: examples
examples: $(EXAMPLES)

%: %.cpp ctl.h
	$(CXX) $(CFLAGS) -o $@ $< && ./$@

.PHONY: clean
clean:
	rm ${EXAMPLES}
