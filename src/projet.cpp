#define DOCTEST_CONFIG_IMPLEMENT
#include "Coord.hpp"
#include "Grille.hpp"
#include "doctest.h"
#include "parametres.hpp"
#include <iostream>

int main() {
  Grille g(TAILLE_GRILLE);

  g.poseBrindille(Coord(5, 5));
  g.poseBrindille(Coord(5, 6));
  g.poseBrindille(Coord(10, 10));

  g.poseTermite(Coord(0, 0), 1);
  g.poseTermite(Coord(19, 19), 2);
  g.poseTermite(Coord(10, 11), 3);

  std::cout << "Affichage de la grille (* = brindille, T = termite) :"
            << std::endl;
  std::cout << g << std::endl;

  std::cout << "Test de estVide sur (1,1) : "
            << (g.estVide(Coord(1, 1)) ? "Vide" : "Pas vide") << std::endl;
  std::cout << "Test de estVide sur (5,5) : "
            << (g.estVide(Coord(5, 5)) ? "Vide" : "Pas vide") << std::endl;
  std::cout << "Numéro du termite en (19,19) : "
            << g.numéroTermite(Coord(19, 19)) << std::endl;

  return 0;
}
