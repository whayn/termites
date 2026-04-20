#ifndef COLONIE_HPP
#define COLONIE_HPP

#include "Coord.hpp"
#include <string>
class Colonie {
private:
  int id;
  Coord position;
  int dureeSablier;
  float probaTourner;
  int score;
  std::string couleur;

public:
  Colonie(int id, Coord position, int dureeSablier, float probaTourner,
          std::string couleur)
      : id(id), position(position), dureeSablier(dureeSablier),
        probaTourner(probaTourner), score(0), couleur(couleur) {}

  int getId() const { return id; }
  Coord getPosition() const { return position; }
  int getDureeSablier() const { return dureeSablier; }
  float getProbaTourner() const { return probaTourner; }
  int getScore() const { return score; }
  std::string getCouleur() const { return couleur; }

  void incrementerScore() { score++; }
  void decrementerScore() { score--; }
  void reinitialiserScore() { score = 0; }
};

#endif
