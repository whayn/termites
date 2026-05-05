#ifndef GESTIONNAIRESPRITES_HPP
#define GESTIONNAIRESPRITES_HPP

#include "string"
#include "unordered_map" // On va utiliser une table de hachage pour stocker les textures chargées
#include <raylib.h>
#include <string>
#include <unordered_map>

struct RegionTexture {
  int col;
  int lig;
  int nbFrames; // Permets de gérer les animations et les sprites staiques
};

class GestionnaireSprites {
private:
  std::unordered_map<std::string, RegionTexture> sprites;
  int tailleTuile;

public:
  GestionnaireSprites(int tailleTuille) : tailleTuile(tailleTuille) {};

  void ajouter(const std::string &nom, int col, int lig, int nbFrames = 1);

  Rectangle getRectangle(const std::string &nom, int frameCourante = 0) const;
};

#endif
