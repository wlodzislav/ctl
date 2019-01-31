CXX = clang++
CFLAGS = -std=c++11

.PHONY: examples
examples: example0
	./example0

example0: example0.cpp include/ctl.h
	$(CXX) $(CFLAGS) -o $@ example0.cpp

.PHONY: clean
clean:
	rm ./example0
