CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude -fPIE
LDFLAGS= -pie

# Modules
IMGUI_DIR = ./external/imgui
RLIMGUI_DIR = ./external/rlImGui
DOCTEST_DIR = ./external/doctest/doctest
CXXFLAGS += -I${IMGUI_DIR} -I${RLIMGUI_DIR} -I${DOCTEST_DIR}

# Raylib
CXXFLAGS +=  $(shell pkg-config --cflags raylib)
LDFLAGS += $(shell pkg-config --libs raylib) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

EXEC_FILES = tests main main-cli

all: $(EXEC_FILES)

# IMGUI
imgui.o: $(IMGUI_DIR)/imgui.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
imgui_draw.o: $(IMGUI_DIR)/imgui_draw.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
imgui_tables.o: $(IMGUI_DIR)/imgui_tables.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
imgui_widgets.o: $(IMGUI_DIR)/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
rlImGui.o: $(RLIMGUI_DIR)/rlImGui.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

IMGUI_OBJS = imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o rlImGui.o

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# termites-main: Coord.o main.o
# 	$(CXX) $(LDFLAGS) -o $@ $^

# termites-test: tests.o Coord.o Direction.o
# 	$(CXX) $(LDFLAGS) -o $@ $^

test.o: src/test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

OBJS = Coord.o Grille.o Termite.o Jeu.o Application.o Colonie.o TermiteVisuel.o GestionnaireSprites.o $(IMGUI_OBJS)

tests: test.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main: main.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main-cli: main_cli.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)



Coord.o: include/Coord.hpp include/parametres.hpp
Grille.o: include/Grille.hpp  include/parametres.hpp
Termite.o: include/Termite.hpp  include/parametres.hpp
Jeu.o: include/Jeu.hpp  include/parametres.hpp
Colonie.o: include/Colonie.hpp  include/parametres.hpp
TermiteVisuel.o: include/TermiteVisuel.hpp  include/parametres.hpp
Application.o: include/Application.hpp  include/parametres.hpp

clean:
	rm -f *.o $(EXEC_FILES)

compile_commands.json: clean
	bear -- $(MAKE) all

bear: compile_commands.json

.PHONY: all clean bear
