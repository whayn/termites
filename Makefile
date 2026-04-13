CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude
LDFLAGS=

EXEC_FILES = termites-main termites-test

all: tests

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# termites-main: Coord.o main.o
# 	$(CXX) $(LDFLAGS) -o $@ $^

# termites-test: tests.o Coord.o Direction.o
# 	$(CXX) $(LDFLAGS) -o $@ $^

test.o: src/test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: test.o Coord.o
		$(CXX) -o $@ $^ $(LDFLAGS)

Coord.o: include/Coord.hpp

clean:
	rm -f *.o $(EXEC_FILES)

compile_commands.json: clean
	bear -- $(MAKE) all

bear: compile_commands.json

.PHONY: all clean bear
