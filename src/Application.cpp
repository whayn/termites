#include "Application.hpp"
#include "Coord.hpp"
#include "GestionnaireSprites.hpp"
#include "Jeu.hpp"
#include "TermiteVisuel.hpp"
#include "imgui.h"
#include "parametres.hpp"
#include "utilitaires.hpp"
#include <raylib.h>
#include <rlImGui.h>
#include <vector>

Application::Application()
    : jeu(nullptr), etatCourant(EtatApp::MENU_PRINCIPAL), chronometre(0),
      vitesseSimulation(1.f), enPause(false), gestionnaireSpritesPetit(16),
      gestionnaireSpritesMoyen(32) {

  config = {.nbTermitesParColonie = 20,
            .densiteBrindilles = 0.05f,
            .nbColonies = 2,
            .tailleGrille = 20};

  // Couleurs des colonies
  couleurs = {
      GRAY,      YELLOW,  GOLD, ORANGE,   PINK,   RED,    GREEN,      LIME,
      DARKGREEN, SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BROWN,
  };

  melanger(couleurs);

  // Initialisation de raylib
  InitWindow(800, 800, "Termites");
  SetTargetFPS(60);
  // Initialisation d'ImGui
  rlImGuiSetup(true);

  // Caméra
  camera = {.offset = {400, 400},
            .target = {(config.tailleGrille * TAILLE_TUILE) / 2.0f,
                       (config.tailleGrille * TAILLE_TUILE) / 2.0f},
            .rotation = 0,
            .zoom = 800.f / (config.tailleGrille * TAILLE_TUILE)};

  // Sprites
  // Sol
  gestionnaireSpritesPetit.ajouter("SOL_VIDE", 0, 0);
  gestionnaireSpritesPetit.ajouter("SOL_CAILLOU_1", 1, 0);
  gestionnaireSpritesPetit.ajouter("SOL_CAILLOU_2", 2, 0);
  gestionnaireSpritesPetit.ajouter("SOL_CAILLOU_RARE", 3, 0);
  gestionnaireSpritesPetit.ajouter("SOL_RACINE_1", 4, 0);
  gestionnaireSpritesPetit.ajouter("SOL_RACINE_2", 5, 0);
  gestionnaireSpritesPetit.ajouter("SOL_CHAMPI", 6, 0);
  gestionnaireSpritesPetit.ajouter("SOL_MOUSSE", 7, 0);

  // Brindilles
  gestionnaireSpritesPetit.ajouter("BRINDILLE", 0, 0);

  // Termites
  gestionnaireSpritesPetit.ajouter("TERMITE_ORTHO", 0, 0, 4);
  gestionnaireSpritesPetit.ajouter("TERMITE_DIAG", 0, 1, 4);
  gestionnaireSpritesPetit.ajouter("TERMITE_ORTHO_BRINDILLE", 0, 2, 4);
  gestionnaireSpritesPetit.ajouter("TERMITE_DIAG_BRINDILLE", 0, 3, 4);

  // Masques termites
  gestionnaireSpritesPetit.ajouter("TERMITE_ORTHO_MASQUE", 0, 4, 4);
  gestionnaireSpritesPetit.ajouter("TERMITE_DIAG_MASQUE", 0, 5, 4);
  gestionnaireSpritesPetit.ajouter("TERMITE_ORTHO_BRINDILLE_MASQUE", 0, 6, 4);
  gestionnaireSpritesPetit.ajouter("TERMITE_DIAG_BRINDILLE_MASQUE", 0, 7, 4);

  // Nid
  gestionnaireSpritesMoyen.ajouter("NID", 0, 0);

  // Chargement des textures
  textureSol = LoadTexture("assets/Sprite_Sol.png");
  textureTermites = LoadTexture("assets/Sprite_Termite.png");
  textureBrindille = LoadTexture("assets/Sprite_Brindille.png");
  textureNid = LoadTexture("assets/Sprite_Nid.png");
}

Application::~Application() {
  UnloadTexture(textureSol);
  UnloadTexture(textureTermites);

  rlImGuiShutdown();
  CloseWindow();
  delete jeu;
}

void Application::executer() {
  while (!WindowShouldClose()) {
    gererEntrees();

    mettreAJourLogique();

    BeginDrawing();

    ClearBackground(DARKGRAY);

    if (etatCourant == EtatApp::SIMULATION) {
      BeginMode2D(camera);
      dessinerGrille();
      EndMode2D();
    }

    rlImGuiBegin();
    if (etatCourant == EtatApp::MENU_PRINCIPAL) {
      dessinerMenu();
    } else if (etatCourant == EtatApp::SIMULATION) {
      dessinerHUD();
    }
    rlImGuiEnd();
    EndDrawing();
  }
}

void Application::gererEntrees() {

  // Zoom
  float zoomChange = GetMouseWheelMove() * 0.1f;
  if (zoomChange != 0) {
    camera.zoom += zoomChange;
    camera.zoom = std::max(0.1f, std::min(camera.zoom, 3.f));
  }

  // Raccourcis clavier
  if (IsKeyPressed(KEY_SPACE)) {
    enPause = !enPause;
  }

  // Panning
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 mouseDelta = GetMouseDelta();
    camera.target.x -= mouseDelta.x / camera.zoom;
    camera.target.y -= mouseDelta.y / camera.zoom;
  }
}

void Application::mettreAJourLogique() {
  if (enPause || jeu == nullptr)
    return;

  // Si vitesse = 2, le délai devient 0.25s
  float delaiBase = 0.5f;
  float delaiReel = delaiBase / vitesseSimulation;

  float dt = GetFrameTime();
  chronometre += dt;

  if (chronometre >= delaiReel) {
    jeu->etapeSuivante();
    synchroniserVisuels();
    chronometre -= delaiReel;
    compteurEtapes++;
  }

  for (TermiteVisuel &rendu : rendusTermites) {
    rendu.mettreAJour(dt, delaiReel);
  }
}

void Application::dessinerGrille() {
  Vector2 origine = {TAILLE_TUILE / 2, TAILLE_TUILE / 2};

  // Sol
  for (int i = 0; i < config.tailleGrille; i++) {
    for (int j = 0; j < config.tailleGrille; j++) {
      CaseDecor &cd = mapDecor[i][j];
      std::string nomSprite = "SOL_VIDE";
      if (cd.idTexture == 1)
        nomSprite = "SOL_RACINE_1";
      else if (cd.idTexture == 2)
        nomSprite = "SOL_RACINE_2";
      else if (cd.idTexture == 3)
        nomSprite = "SOL_CAILLOU_1";
      else if (cd.idTexture == 4)
        nomSprite = "SOL_CAILLOU_2";

      Rectangle source = gestionnaireSpritesPetit.getRectangle(nomSprite);

      Rectangle dest = {j * TAILLE_TUILE + origine.x,
                        i * TAILLE_TUILE + origine.y, TAILLE_TUILE,
                        TAILLE_TUILE};

      DrawTexturePro(textureSol, source, dest, origine, cd.angle, WHITE);

      if (jeu->getGrille().contientBrindille(Coord(i, j))) {
        DrawRectangle(j * TAILLE_TUILE + origine.x - 5,
                      i * TAILLE_TUILE + origine.y - 5, 10, 10, BROWN);
      }
      if (jeu->getGrille().contientNid(Coord(i, j))) {
        DrawCircle(j * TAILLE_TUILE + origine.x, i * TAILLE_TUILE + origine.y,
                   10,
                   couleurs[jeu->getGrille().proprietaireCase(Coord(i, j)) %
                            (int)couleurs.size()]);
      }
    }
  }

  // Termites
  for (const TermiteVisuel &rendu : rendusTermites) {
    rendu.dessiner(textureTermites, gestionnaireSpritesPetit, TAILLE_TUILE);
  }
}

void Application::dessinerMenu() {
  // Centrons le menu
  ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_Appearing,
                          ImVec2(0.5f, 0.5f));
  ImGui::Begin("Menu Principal", nullptr,
               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
  ImGui::Text("Termites");
  ImGui::Separator();

  // Configuration de la simulation
  ImGui::InputInt("Nombre de colonies", &config.nbColonies, 1, 5);
  ImGui::InputInt("Termites par colonie", &config.nbTermitesParColonie, 1, 5);
  ImGui::SliderFloat("Densité de brindilles", &config.densiteBrindilles, 0.f,
                     1.f);
  ImGui::InputInt("Taille de la grille", &config.tailleGrille, 1, 10);

  if (ImGui::Button("Lancer la simulation")) {
    if (jeu != nullptr) {
      delete jeu;
      rendusTermites.clear();
    }

    jeu = new Jeu(config.nbTermitesParColonie, config.densiteBrindilles,
                  config.nbColonies, config.tailleGrille);

    initialiserDecor();
    synchroniserVisuels();

    etatCourant = EtatApp::SIMULATION;
    enPause = false;
  }
  ImGui::End();
}

void Application::dessinerHUD() {
  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
  ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Étape : %d", compteurEtapes);
  for (const Colonie &colonie : jeu->getColonies()) {
    ImGui::Text("Colonie %d - Score : %d", colonie.getId(), colonie.getScore());
  }
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(10, 300), ImGuiCond_FirstUseEver);
  ImGui::Begin("Contrôles", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Espace : Pause/Play");
  if (ImGui::Button(enPause ? "Play" : "Pause", ImVec2(100, 30))) {
    enPause = !enPause;
  }

  ImGui::SliderFloat("Vitesse Tick", &vitesseSimulation, 0.01f, 20.0f);

  ImGui::Separator();

  if (ImGui::Button("Quitter vers le Menu")) {
    etatCourant = EtatApp::MENU_PRINCIPAL;
  }

  ImGui::End();
}

void Application::synchroniserVisuels() {
  // Initialisation des rendus des termites s'ils n'existent pas encore
  if (rendusTermites.size() == 0) {
    for (const Termite &t : jeu->getTermites()) {
      Vector2 pos = {(float)t.getPosition().getCol(),
                     (float)t.getPosition().getLig()};
      rendusTermites.emplace_back(
          t.getId(), pos, pos,
          couleurs[jeu->getColonies()[t.getIdColonie()].getId() %
                   (int)couleurs.size()]);
    }
  }

  for (TermiteVisuel &rendu : rendusTermites) {
    rendu.setPosDepart(rendu.getPosArrivee());
    const Termite &t = jeu->getTermites()[rendu.getIdTermite()];

    Vector2 pos = {(float)t.getPosition().getCol(),
                   (float)t.getPosition().getLig()};
    rendu.setPosArrivee(pos);

    rendu.setPorteBrindille(t.porteBrindille());
    rendu.setCap(t.directionTermite());

    rendu.setProgression(0.f);
  }
}

void Application::initialiserDecor() {
  mapDecor.assign(
      config.tailleGrille,
      std::vector<CaseDecor>(config.tailleGrille)) // On initialise la map de
                                                   // décor avec des cases vides
      ;
  for (int i = 0; i < config.tailleGrille; i++) {
    for (int j = 0; j < config.tailleGrille; j++) {
      int r = GetRandomValue(0, 100);
      if (r < 95) {
        mapDecor[i][j].idTexture = 0; // Texture de base
      } else if (r < 98) {
        mapDecor[i][j].idTexture =
            GetRandomValue(0, 100) < 50 ? 1 : 2; // Racine
      } else {
        mapDecor[i][j].idTexture =
            GetRandomValue(0, 100) < 99 ? 4 : 3; // Caillou normal et rare
      }

      mapDecor[i][j].angle = GetRandomValue(0, 3) * 90.0f;
    }
  }
}
