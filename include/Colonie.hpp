#ifndef COLONIE_HPP
#define COLONIE_HPP

#include "Coord.hpp"
class Colonie {
private:
  int id;
  Coord position;
  int dureeSablier;
  float probaTourner;
  int score;

public:
  Colonie(int id, Coord position, int dureeSablier, float probaTourner)
      : id(id), position(position), dureeSablier(dureeSablier),
        probaTourner(probaTourner), score(0) {}

  int getId() const { return id; }
  Coord getPosition() const { return position; }
  int getDureeSablier() const { return dureeSablier; }
  float getProbaTourner() const { return probaTourner; }
  int getScore() const { return score; }

  void incrementerScore() { score++; }
};

#endif
