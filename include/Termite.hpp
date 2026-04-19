#ifndef TERMITE_HPP
#define TERMITE_HPP

#include "Coord.hpp"
#include "Grille.hpp"
#include <string>

class Termite {
private:
  int id;
  Coord position;
  Direction cap;
  bool avecBrindille;
  int sablier;
  bool tourneSurPlace;

public:
  Termite(Coord position, int id);

  // Constantes donc pas besoin de getters
  static const int DUREE_SABLIER = 6;
  static constexpr float PROBA_TOURNER = 0.1;

  // Accès
  Direction directionTermite() const;
  std::string toString() const;
  Coord devant() const;
  int getSablier() const;
  int getId() const { return id; }
  Coord getPosition() const { return position; }

  // Prédicat
  bool porteBrindille() const;

  // Modification
  void tourneADroite();
  void tourneAGauche();
  void tourneAleat();

  // Lecture avec le monde
  bool laVoieEstLibre(const Grille &grille) const;
  bool brindilleEnFace(const Grille &grille) const;
  int voisinsLibre(const Grille &grille) const;

  // Actions sur le monde
  void avance(Grille &grille);
  void marcheAleatoire(Grille &grille);
  void chargeBrindille(Grille &grille);
  void dechargeBrindille(Grille &grille);
  void vieTermite(Grille &grille);
};

#endif
