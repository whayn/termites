#ifndef GRILLE_HPP
#define GRILLE_HPP
#include "Coord.hpp"
#include <ostream>
#include <vector>

// Structure dont les attributs sont publics et qui sera cachée de
// l'utilisateur par le type abstrait Grille.
struct Case {
  bool contientBrindille;
  int idTermite; // -1 si aucune termite

  Case() : contientBrindille(false), idTermite(-1) {}
};

class Grille {
private:
  std::vector<std::vector<Case>> cases;

public:
  Grille(int t) : cases(t, std::vector<Case>(t)) {}

  void poseBrindille(Coord c);
  void enleveBrindille(Coord c);
  bool contientBrindille(Coord c) const;

  void poseTermite(Coord c, int idT);
  void enleveTermite(Coord c);
  int numéroTermite(Coord c) const;

  bool estVide(Coord c) const;

  // Plus tard on réalisera une fonction afficheJeu
  std::ostream &print(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const Grille &g);

#endif
