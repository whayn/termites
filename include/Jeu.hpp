#ifndef JEU_HPP
#define JEU_HPP

#include "Colonie.hpp"
#include "Grille.hpp"
#include "Termite.hpp"
#include "parametres.hpp"
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
  Jeu(const AppConfig &config);

  const std::vector<Termite> &getTermites() const { return termites; }
  const std::vector<Colonie> &getColonies() const { return colonies; }
  const Grille &getGrille() const { return grille; }

  void etapeSuivante(const LaboConfig &laboConfig);

  std::ostream &afficheJeu(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const Jeu &j);

#endif
