#ifndef PARAMETRES_HPP
#define PARAMETRES_HPP

#include <vector>

const float TAILLE_TUILE =
    16.0f; // En pixels, pour l'affichage, ne devrait pas
           // être modifié sauf si l'on change les sprites

struct ColonieConfig {
  int population = 20;
  int tempsSablier = 6;
  float probaTourner = 0.1f;

  ColonieConfig() = default;
  ColonieConfig(int pop, int ts, float pt)
      : population(pop), tempsSablier(ts), probaTourner(pt) {}
};
struct AppConfig {
  int tailleGrille = 50;
  float densiteBrindilles = 0.08f;
  int nbColonies = 3;
  std::vector<ColonieConfig> coloniesConfig;

  AppConfig() = default;
  AppConfig(int tg, float db, int nbc, std::vector<ColonieConfig> cc)
      : tailleGrille(tg), densiteBrindilles(db), nbColonies(nbc),
        coloniesConfig(cc) {}
};

struct LaboConfig {
  float tauxEvaporation = 0.005f;
  float depotPheromone = 0.15f;
  float opaciteVisuelle = 0.6f;
  float delaiDeBase = 0.5f;
  float rayonAntiBouchon = 6.0f;
  int compaciteNid = 1;   // 1 = aéré, 0 = dense
  float bruitGPS = 10.0f; // en pourcentage

  // Terrain - offsets fixés au début du jeu pour garder le même terrain
  int offsetX = 0;
  int offsetY = 0;
  float zoomPerlin = 4.0f;
  int seuilClimatSec = 90;
  int seuilClimatHumide = 170;
};

#endif
