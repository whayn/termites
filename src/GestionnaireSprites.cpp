#include "GestionnaireSprites.hpp"
#include "raylib.h"
#include <stdexcept>

void GestionnaireSprites::ajouter(const std::string &nom, int col, int lig,
                                  int nbFrames) {
  if (sprites.find(nom) != sprites.end()) {
    throw std::invalid_argument("Animation déjà définie : " + nom);
  }

  sprites[nom] = {col, lig, nbFrames};
}

Rectangle GestionnaireSprites::getRectangle(const std::string &nom,
                                            int frameCourante) const {
  auto it = sprites.find(nom);
  if (it == sprites.end()) {
    throw std::invalid_argument("Sprite non trouvé : " + nom);
  }

  const RegionTexture &reg = it->second;

  int frame =
      frameCourante % reg.nbFrames; // Si statique, frame vaudra toujours 0

  Rectangle source = {(float)((reg.col + frame) * tailleTuile),
                      // frame
                      (float)(reg.lig * tailleTuile), // ligne de sprite
                      (float)tailleTuile, (float)tailleTuile};

  return source;
}
