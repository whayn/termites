CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude -fPIE
LDFLAGS= -pie

EXEC_FILES = tests projet main

all: $(EXEC_FILES)

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# termites-main: Coord.o main.o
# 	$(CXX) $(LDFLAGS) -o $@ $^

# termites-test: tests.o Coord.o Direction.o
# 	$(CXX) $(LDFLAGS) -o $@ $^

test.o: src/test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests: test.o Coord.o Grille.o Termite.o Jeu.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

projet: projet.o Coord.o Grille.o Termite.o Jeu.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main: main.o Coord.o Grille.o Termite.o Jeu.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Coord.o: include/Coord.hpp include/parametres.hpp
Grille.o: include/Grille.hpp  include/parametres.hpp
Termite.o: include/Termite.hpp  include/parametres.hpp
Jeu.o: include/Jeu.hpp  include/parametres.hpp

clean:
	rm -f *.o $(EXEC_FILES)

compile_commands.json: clean
	bear -- $(MAKE) all

bear: compile_commands.json

.PHONY: all clean bear
