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

  float tauxEvaporationPheromones;
  float quantitePheromone;

  void verifieIntegrite() const;
  void calculerScores();

public:
  Jeu(int nbTermitesParColonie = 20, float densiteBrindilles = 0.08,
      int nbColonies = 3, int tailleGrille = 50,
      float tauxEvaporationPheromones = 0.005f,
      float quantitePheromone = 0.15f);

  const std::vector<Termite> &getTermites() const { return termites; }
  const std::vector<Colonie> &getColonies() const { return colonies; }
  const Grille &getGrille() const { return grille; }

  void etapeSuivante();

  std::ostream &afficheJeu(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const Jeu &j);

#endif
