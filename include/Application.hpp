#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Jeu.hpp"
#include "TermiteVisuel.hpp"
#include <raylib.h>

enum class EtatApp {
  MENU_PRINCIPAL,
  SIMULATION,
};

struct AppConfig {
  int nbTermitesParColonie;
  float densiteBrindilles;
  int nbColonies;
};

struct CaseDecor {
  int idTexture;
  float angle;
};

class Application {
private:
  Jeu *jeu;

  EtatApp etatCourant;
  Camera2D camera;

  // Textures
  Texture2D textureSol;
  Texture2D textureTermites;

  std::vector<TermiteVisuel> rendusTermites; // La liste de nos acteurs animés

  float chronometre;       // Temps écoulé depuis la dernière mise à jour de la
                           // logique du jeu
  float vitesseSimulation; // Le nombre de secondes entre chaque mise à jour de
                           // la logique du jeu
  bool enPause;
  int compteurEtapes = 0;

  AppConfig config;

  void mettreAJourLogique();  // Avance la simulation si pas en pause
  void dessinerMenu();        // Menu de départ ImGui
  void dessinerGrille();      // Appelle les fonctions de dessin de raylib pour
                              // afficher la grille et les éléments du jeu
  void dessinerHUD();         // Stats par ImGui
  void gererEntrees();        // Controle de la caméra et clavier/souris
  void synchroniserVisuels(); // Met à jour les rendus des termites pour les
                              // faire correspondre à l'état du jeu
  void initialiserDecor();    // Précalcule le wang tiling pour les cases
                              // de la grille

  std::vector<Color> couleurs;
  std::vector<std::vector<CaseDecor>> mapDecor;

public:
  Application();  // Constructeur
  ~Application(); // Destructeur

  // Lance la boucle principale de l'application
  void executer();
};
#endif
