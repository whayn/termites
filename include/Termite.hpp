#ifndef TERMITE_HPP
#define TERMITE_HPP

#include "Coord.hpp"
#include "Grille.hpp"
#include "parametres.hpp"
#include <string>

class Termite {
private:
  int id;
  int idColonie;
  Coord position;
  Coord posNid;
  Direction cap;
  bool avecBrindille;
  int sablier;
  bool tourneSurPlace;
  int dureeSablier;
  float probaTourner;

public:
  Termite(int id, int idColonie, Coord position, Coord posNid,
          int dureeSablier = 6, float probaTourner = 0.1);

  // Accès
  Direction directionTermite() const;
  std::string toString() const;
  Coord devant() const;
  int getSablier() const;
  int getId() const { return id; }
  int getIdColonie() const { return idColonie; }
  Coord getPosition() const { return position; }
  int getDureeSablier() const { return dureeSablier; }
  float getProbaTourner() const { return probaTourner; }

  // Prédicat
  bool porteBrindille() const;

  // Modification
  void tourneADroite();
  void tourneAGauche();
  void tourneAleat();

  // Lecture avec le monde
  bool laVoieEstLibre(const Grille &grille) const;
  bool brindilleEnFace(const Grille &grille) const;
  bool nidEnFace(const Grille &grille) const;
  int voisinsLibre(const Grille &grille) const;

  // Actions sur le monde
  void avance(Grille &grille);
  void marcheAleatoire(Grille &grille);
  void chargeBrindille(Grille &grille);
  void dechargeBrindille(Grille &grille);
  void vieTermite(Grille &grille, const LaboConfig &laboConfig);
};

#endif
