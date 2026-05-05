#include "TermiteVisuel.hpp"
#include "GestionnaireSprites.hpp"
#include "parametres.hpp"
#include "raylib.h"
#include <algorithm>
#include <raymath.h>

void TermiteVisuel::mettreAJour(float delta, float vitesseSimulation) {
  progression += delta / vitesseSimulation;
  posActuelle = Vector2Lerp(posDepart, posArrivee, std::min(progression, 1.0f));

  bool estEnMouvement =
      (posDepart.x != posArrivee.x || posDepart.y != posArrivee.y);

  if (progression < 1.0f && estEnMouvement) {
    timerAnimation += delta;
    if (timerAnimation >= 0.1f) { // Vitesse de l'animation
      timerAnimation = 0;
      frameCourante = (frameCourante + 1) % 4;
    }
  } else {
    frameCourante = 0;
  }
}

void TermiteVisuel::dessiner(Texture2D &texture,
                             const GestionnaireSprites &gestionnaireSprites,
                             float tailleCase) const {
  bool estDiagonale =
      (cap == Direction::NORD_EST || cap == Direction::SUD_EST ||
       cap == Direction::SUD_OUEST || cap == Direction::NORD_OUEST);

  std::string nomBase = "TERMITE_";
  nomBase += estDiagonale ? "DIAG" : "ORTHO";
  if (porteBrindille)
    nomBase += "_BRINDILLE";

  std::string nomMasque = nomBase + "_MASQUE";

  Rectangle sourceBase =
      gestionnaireSprites.getRectangle(nomBase, frameCourante);

  Rectangle sourceMasque =
      gestionnaireSprites.getRectangle(nomMasque, frameCourante);

  Rectangle dest = {
      std::round(posActuelle.x * tailleCase) + (tailleCase / 2.0f),
      std::round(posActuelle.y * tailleCase) + (tailleCase / 2.0f), tailleCase,
      tailleCase};
  Vector2 origineRotation = {tailleCase / 2.0f, tailleCase / 2.0f};

  float angleRotation = 0.0f;
  if (cap == Direction::NORD || cap == Direction::NORD_EST) {
    angleRotation = 0.0f;
  } else if (cap == Direction::EST || cap == Direction::SUD_EST) {
    angleRotation = 90.0f;
  } else if (cap == Direction::SUD || cap == Direction::SUD_OUEST) {
    angleRotation = 180.0f;
  } else if (cap == Direction::OUEST || cap == Direction::NORD_OUEST) {
    angleRotation = -90.0f;
  }

  DrawTexturePro(texture, sourceBase, dest, origineRotation, angleRotation,
                 WHITE);
  DrawTexturePro(texture, sourceMasque, dest, origineRotation, angleRotation,
                 couleur);
}
