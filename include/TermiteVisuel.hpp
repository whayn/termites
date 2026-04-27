#ifndef TERMITEVISUEL_HPP
#define TERMITEVISUEL_HPP

#include "Coord.hpp"
#include "raylib.h"
class TermiteVisuel {
private:
  int idTermite;
  Vector2 posDepart;
  Vector2 posActuelle;
  Vector2 posArrivee;
  float
      progression; // Entre 0 et 1, indique la progression d'une case à l'autre
  int frameCourante; // Pour l'animation de marche
  float timerAnimation;
  Color couleur;
  bool porteBrindille;
  Direction cap;

public:
  TermiteVisuel(int idTermite, Vector2 posDepart, Vector2 posArrivee,
                Color couleur)
      : idTermite(idTermite), posDepart(posDepart), posActuelle(posDepart),
        posArrivee(posArrivee), progression(0.0f), frameCourante(0),
        timerAnimation(0.0f), couleur(couleur), porteBrindille(false),
        cap(Direction::NORD) {}

  int getIdTermite() const { return idTermite; }
  Vector2 getPosDepar() const { return posDepart; }
  Vector2 getPosArrivee() const { return posArrivee; }

  void setPosArrivee(Vector2 pos) { posArrivee = pos; }
  void setPosDepart(Vector2 pos) { posDepart = pos; }
  void setProgression(float p) { progression = p; }
  void setPorteBrindille(bool porte) { porteBrindille = porte; }
  void setCap(Direction c) { cap = c; }

  void mettreAJour(float delta, float vitesseSimulation);
  void dessiner(Texture2D &texture, float tailleCase) const;
};

#endif
