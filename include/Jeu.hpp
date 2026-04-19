#ifndef JEU_HPP
#define JEU_HPP

#include "Grille.hpp"
#include "Termite.hpp"
#include <vector>

class Jeu {
private:
  Grille grille;
  std::vector<Termite> termites;
  int numeroEtape;

  void verifieIntegrite() const;

public:
  Jeu(int nbTermites = 20, float densiteBrindilles = 0.05);

  void etapeSuivante();

  std::ostream &afficheJeu(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const Jeu &j);

#endif
