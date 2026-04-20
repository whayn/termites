#ifndef JEU_HPP
#define JEU_HPP

#include "Colonie.hpp"
#include "Grille.hpp"
#include "Termite.hpp"
#include <vector>

class Jeu {
private:
  Grille grille;
  std::vector<Termite> termites;
  std::vector<Colonie> colonies;
  int numeroEtape;

  void verifieIntegrite() const;
  void calculerScores();

public:
  Jeu(int nbTermitesParColonie = 20, float densiteBrindilles = 0.05,
      int nbColonies = 2);

  void etapeSuivante();

  std::ostream &afficheJeu(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const Jeu &j);

#endif
