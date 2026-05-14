#include "Application.hpp"
#include "Colonie.hpp"
#include "Coord.hpp"
#include "GestionnaireSprites.hpp"
#include "Jeu.hpp"
#include "TermiteVisuel.hpp"
#include "imgui.h"
#include "parametres.hpp"
#include "raymath.h"
#include "utilitaires.hpp"
#include <cmath>
#include <cstdint>
#include <raylib.h>
#include <rlImGui.h>
#include <vector>

const AppConfig JEU_PAR_DEFAUT(50,    // tailleGrille
                               0.08f, // densiteBrindilles
                               3,     // nbColonies
                               std::vector<ColonieConfig>{
                                   ColonieConfig{20, 6, 0.1f},
                                   ColonieConfig{20, 6, 0.1f},
                                   ColonieConfig{20, 6, 0.1f}} // coloniesConfig
);

void ConfigurerInterface() {
  ImGuiStyle &style = ImGui::GetStyle();

  // padding et arrondis
  style.WindowRounding = 12.0f;
  style.FrameRounding = 8.0f;
  style.GrabRounding = 10.0f;
  style.PopupRounding = 8.0f;
  style.WindowPadding = ImVec2(15, 15);
  style.FramePadding = ImVec2(10, 5);
  style.ItemSpacing = ImVec2(10, 8);
  style.WindowBorderSize = 0.0f;

  // Couleurs
  ImVec4 *colors = style.Colors;

  // Fond des fenêtres (Marron terre très sombre et un peu transparent)
  colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.14f, 0.12f, 0.85f);

  // Titres (Marron un peu plus clair)
  colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.20f, 0.17f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.35f, 0.28f, 0.24f, 1.00f);

  // Widgets (Boutons, Sliders, Checkboxes) : Vert Mousse
  colors[ImGuiCol_FrameBg] = ImVec4(0.24f, 0.35f, 0.20f, 0.70f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.45f, 0.25f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.30f, 0.15f, 1.00f);

  // Boutons : Vert Forêt
  colors[ImGuiCol_Button] = ImVec4(0.24f, 0.45f, 0.20f, 0.80f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.35f, 0.60f, 0.30f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.35f, 0.10f, 1.00f);

  // Sliders & Scrollbars : Lichen
  colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.65f, 0.30f, 0.80f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.80f, 0.40f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.80f, 0.40f, 1.00f);

  // En-têtes (Collapsing Headers)
  colors[ImGuiCol_Header] = ImVec4(0.35f, 0.28f, 0.24f, 0.80f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.38f, 0.34f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.18f, 0.14f, 1.00f);

  // Texte
  colors[ImGuiCol_Text] = ImVec4(0.95f, 0.90f, 0.85f, 1.00f); // Crème/Off-white
}

Application::Application()
    : jeu(nullptr), etatCourant(EtatApp::MENU_PRINCIPAL),
      gestionnaireSpritesPetit(TAILLE_TUILE),
      gestionnaireSpritesMoyen(TAILLE_TUILE * 2), chronometre(0),
      vitesseSimulation(1.f), enPause(false), config(JEU_PAR_DEFAUT) {

  // Couleurs des colonies
  couleurs = {
      GRAY,      YELLOW,  GOLD, ORANGE,   PINK,   RED,    GREEN,      LIME,
      DARKGREEN, SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BROWN,
  };

  melanger(couleurs);

  // Initialisation de raylib
  InitWindow(1000, 1000, "Termites");
  SetTargetFPS(60);
  // Initialisation d'ImGui
  rlImGuiSetup(true);
  ConfigurerInterface();

  // Lancement du jeu
  camera = Camera2D{};
  reinitialiserJeu(JEU_PAR_DEFAUT, true);

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
  gestionnaireSpritesMoyen.ajouter("NID_MASQUE", 0, 1);

  // Chargement des textures
  textureSol = LoadTexture("assets/Sprite_Sol.png");
  textureTermites = LoadTexture("assets/Sprite_Termite.png");
  textureBrindille = LoadTexture("assets/Sprite_Brindille.png");
  textureNid = LoadTexture("assets/Sprite_Nid.png");
}

Application::~Application() {
  UnloadTexture(textureSol);
  UnloadTexture(textureTermites);
  UnloadTexture(textureBrindille);
  UnloadTexture(textureNid);

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

    BeginMode2D(camera);
    dessinerGrille();
    EndMode2D();

    if (etatCourant == EtatApp::MENU_PRINCIPAL) {
      DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                    ColorAlpha(BLACK, 0.6f));
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
  if (etatCourant == EtatApp::SIMULATION) {
    float largeurEcran = (float)GetScreenWidth();
    float hauteurEcran = (float)GetScreenHeight();

    float tailleMonde = config.tailleGrille * TAILLE_TUILE;

    // Zoom
    float changementZoom = GetMouseWheelMove() * 0.1f;
    if (changementZoom != 0) {
      camera.zoom += changementZoom;
    }

    float zoomMin =
        std::max(largeurEcran / tailleMonde, hauteurEcran / tailleMonde);

    if (camera.zoom < zoomMin)
      camera.zoom = zoomMin;
    if (camera.zoom > 3.0f)
      camera.zoom = 3.0f;

    // Panning
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 mouseDelta = GetMouseDelta();
      camera.target.x -= mouseDelta.x / camera.zoom;
      camera.target.y -= mouseDelta.y / camera.zoom;
    }

    // On garde la caméra dans notre bounding box
    // On calcule l'emprise de la caméra dans le monde
    // camera.offset est {400, 400}, soit le centre de l'écran
    float moitieLargeurVue = camera.offset.x / camera.zoom;
    float moitieHauteurVue = camera.offset.y / camera.zoom;

    // On bloque la Target pour que les bords de la vue ne dépassent jamais [0,
    // tailleMonde]
    if (camera.target.x < moitieLargeurVue)
      camera.target.x = moitieLargeurVue;

    if (camera.target.x >
        tailleMonde - (largeurEcran - camera.offset.x) / camera.zoom)
      camera.target.x =
          tailleMonde - (largeurEcran - camera.offset.x) / camera.zoom;

    if (camera.target.y < moitieHauteurVue)
      camera.target.y = moitieHauteurVue;

    if (camera.target.y >
        tailleMonde - (hauteurEcran - camera.offset.y) / camera.zoom)
      camera.target.y =
          tailleMonde - (hauteurEcran - camera.offset.y) / camera.zoom;

    // Raccourcis clavier
    if (IsKeyPressed(KEY_SPACE)) {
      enPause = !enPause;
    }

    // Inspection
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
      idTermiteInspecte = -1; // Reset

      for (const auto &rendu : rendusTermites) {
        Vector2 termitePos = {
            rendu.getPosArrivee().x * TAILLE_TUILE + (TAILLE_TUILE / 2.0f),
            rendu.getPosArrivee().y * TAILLE_TUILE + (TAILLE_TUILE / 2.0f)};
        float dist = Vector2Distance(mouseWorldPos, termitePos);
        if (dist < 15.0f) { // Si on clique assez près
          idTermiteInspecte = rendu.getIdTermite();

          break;
        }
      }
    }
  }
}

void Application::mettreAJourLogique() {
  if (enPause || jeu == nullptr)
    return;

  if (etatCourant == EtatApp::MENU_PRINCIPAL) {
    camera.rotation += 0.005f;
  }

  // Si vitesse = 2, le délai devient 0.25s
  float delaiReel = laboConfig.delaiDeBase / vitesseSimulation;

  float dt = GetFrameTime();

  // Logique Caméra
  if (idTermiteInspecte != -1) {
    for (const auto &rendu : rendusTermites) {
      if (rendu.getIdTermite() == idTermiteInspecte) {
        Vector2 cible = {
            rendu.getPosArrivee().x * TAILLE_TUILE + (TAILLE_TUILE / 2.0f),
            rendu.getPosArrivee().y * TAILLE_TUILE + (TAILLE_TUILE / 2.0f)};

        // Lerp du zoom vers la target
        camera.target.x += (cible.x - camera.target.x) * 5.0f * dt;
        camera.target.y += (cible.y - camera.target.y) * 5.0f * dt;
        camera.zoom += (4.0f - camera.zoom) * 3.0f * dt; // Zoom sur le termite
        break;
      }
    }
  } else if (etatCourant == EtatApp::MENU_PRINCIPAL) {
    camera.rotation += 0.05f * dt; // Rotation plus fluide
  }

  chronometre += dt;
  if (chronometre >= delaiReel) {
    jeu->etapeSuivante(laboConfig);
    synchroniserVisuels();
    chronometre -= delaiReel;
    compteurEtapes++;

    // Graphique de l'évolution des scores
    for (int i = 0; i < (int)jeu->getColonies().size(); i++) {
      if ((int)historiqueScores.size() <= i)
        historiqueScores.push_back({});
      historiqueScores[i].push_back((float)jeu->getColonies()[i].getScore());
      if ((int)historiqueScores[i].size() > maxHistorique) {
        historiqueScores[i].erase(historiqueScores[i].begin());
      }
    }
  }

  for (TermiteVisuel &rendu : rendusTermites) {
    rendu.mettreAJour(dt, delaiReel);
  }
}

void Application::dessinerGrille() {
  // ORigine du sprite au millieu pour les rotations
  Vector2 origine = {TAILLE_TUILE / 2, TAILLE_TUILE / 2};

  // Fond + Brindilles
  for (int i = 0; i < config.tailleGrille; i++) {
    for (int j = 0; j < config.tailleGrille; j++) {
      CaseDecor &cd = mapDecor[i][j];

      Rectangle destFond = {j * TAILLE_TUILE, i * TAILLE_TUILE, TAILLE_TUILE,
                            TAILLE_TUILE};
      DrawRectangleRec(destFond, cd.couleurFond);

      Rectangle destCentre = {j * TAILLE_TUILE + origine.x,
                              i * TAILLE_TUILE + origine.y, TAILLE_TUILE,
                              TAILLE_TUILE};

      // Props
      if (cd.nomSprite != "SOL_VIDE") {
        Rectangle source = gestionnaireSpritesPetit.getRectangle(cd.nomSprite);

        if (cd.flipH)
          source.width = -source.width;
        if (cd.flipV)
          source.height = -source.height;

        DrawTexturePro(textureSol, source, destCentre, origine, cd.angle,
                       WHITE);
      }

      // Pheromones
      if (afficherPheromones) {
        for (int idCol = 0; idCol < config.nbColonies; idCol++) {
          float intensite =
              jeu->getGrille().getIntensitePheromone(Coord(i, j), idCol);

          // On affiche seulement si la trace est assez forte (optimisation
          // visuelle)
          if (intensite > 0.02f) {
            Color couleurColonie = couleurs[idCol % (int)couleurs.size()];
            // On multiplie par l'opacité visuelle max pour ne jamais rendre le
            // sol totalement opaque, même à intensité 1.0
            DrawRectangleRec(
                destFond, ColorAlpha(couleurColonie,
                                     intensite * laboConfig.opaciteVisuelle));
          }
        }
      }

      // Brindilles
      if (jeu->getGrille().contientBrindille(Coord(i, j))) {
        Rectangle source = gestionnaireSpritesPetit.getRectangle("BRINDILLE");

        if (cd.flipHBrindille)
          source.width = -source.width;
        if (cd.flipVBrindille)
          source.height = -source.height;

        DrawTexturePro(textureBrindille, source, destCentre, origine,
                       cd.angleBrindille, WHITE);
      }
    }
  }

  // Nids
  Vector2 origineNid = {TAILLE_TUILE, TAILLE_TUILE}; // Centre de 2x2 tuiles

  for (const Colonie &colonie : jeu->getColonies()) {
    Coord pos = colonie.getPosition();
    Color couleurNid = couleurs[colonie.getId() % (int)couleurs.size()];

    Rectangle sourceNid = gestionnaireSpritesMoyen.getRectangle("NID");

    Rectangle sourceMasqueNid =
        gestionnaireSpritesMoyen.getRectangle("NID_MASQUE");

    Rectangle destNid = {pos.getCol() * TAILLE_TUILE + origineNid.x,
                         pos.getLig() * TAILLE_TUILE + origineNid.y,
                         TAILLE_TUILE * 2.0f, TAILLE_TUILE * 2.0f};

    DrawTexturePro(textureNid, sourceNid, destNid, origineNid, 0.0f, WHITE);
    DrawTexturePro(textureNid, sourceMasqueNid, destNid, origineNid, 0.0f,
                   couleurNid);
  }

  for (const TermiteVisuel &rendu : rendusTermites) {
    rendu.dessiner(textureTermites, gestionnaireSpritesPetit, TAILLE_TUILE);
  }

  // Si on inspecte un termite, on dessine un cadre autour de lui
  if (idTermiteInspecte != -1) {
    for (const auto &rendu : rendusTermites) {
      if (rendu.getIdTermite() == idTermiteInspecte) {
        // On force les coordonnées en entiers pour éviter le flou
        int px = (int)(rendu.getPosArrivee().x * TAILLE_TUILE);
        int py = (int)(rendu.getPosArrivee().y * TAILLE_TUILE);
        int t = (int)TAILLE_TUILE;

        // Couleur pulsée
        Color c = WHITE;

        // On dessine 4 coins de 3 pixels de long
        // Coin haut-gauche
        DrawRectangle(px - 1, py - 1, 3, 1, c);
        DrawRectangle(px - 1, py, 1, 2, c);

        // Coin haut-droite
        DrawRectangle(px + t - 2, py - 1, 3, 1, c);
        DrawRectangle(px + t, py, 1, 2, c);

        // Coin bas-gauche
        DrawRectangle(px - 1, py + t, 3, 1, c);
        DrawRectangle(px - 1, py + t - 2, 1, 2, c);

        // Coin bas-droite
        DrawRectangle(px + t - 2, py + t, 3, 1, c);
        DrawRectangle(px + t, py + t - 2, 1, 2, c);

        break;
      }
    }
  }
}

void Application::dessinerMenu() {
  float largeurEcran = (float)GetScreenWidth();
  float hauteurEcran = (float)GetScreenHeight();

  // Plein écran
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(largeurEcran, hauteurEcran));
  ImGui::Begin("MenuFond", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

  // Centrage du contenu
  ImGui::SetNextWindowPos(ImVec2(largeurEcran / 2.0f, hauteurEcran / 2.0f),
                          ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(450, 0)); // Largeur fixe, hauteur auto

  ImGui::Begin("Menu", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
  // Titre
  ImGui::Spacing();

  ImGui::TextColored(ImVec4(0.4f, 0.6f, 0.3f, 1.0f), "Stigmergy in Termites");
  ImGui::Separator();
  ImGui::Spacing();

  // Configuration
  ImGui::Text("Config");
  ImGui::Spacing();

  ImGui::SliderInt("Grid Size", &config.tailleGrille, 10, 200);
  ImGui::SliderFloat("Twigs Density", &config.densiteBrindilles, 0.01f, 0.3f,
                     "%.2f");

  if (ImGui::SliderInt("Colonies", &config.nbColonies, 1, 8)) {
    config.coloniesConfig.resize(config.nbColonies);
  }

  for (int i = 0; i < config.nbColonies; i++) {
    if (ImGui::TreeNode((void *)(intptr_t)i, "Colony %d", i + 1)) {
      ImGui::SliderInt("Population", &config.coloniesConfig[i].population, 5,
                       200);
      ImGui::SliderInt("Action Timer", &config.coloniesConfig[i].tempsSablier,
                       1, 20);
      ImGui::SliderFloat("Turn Probability",
                         &config.coloniesConfig[i].probaTourner, 0.01f, 1.0f,
                         "%.2f");
      ImGui::TreePop();
    }
  }

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  // LAncement
  if (ImGui::Button("Start the simulation", ImVec2(-1, 50))) {
    reinitialiserJeu(config, false);
    etatCourant = EtatApp::SIMULATION;
    enPause = false;
  }

  ImGui::End();
  ImGui::End();
}

void Application::dessinerHUD() {
  float largeurEcran = (float)GetScreenWidth();
  float hauteurEcran = (float)GetScreenHeight();

  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
  ImGui::Begin("Stats", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
                   ImGuiWindowFlags_NoMove);
  for (const Colonie &colonie : jeu->getColonies()) {

    Color c = couleurs[colonie.getId() % couleurs.size()];
    ImVec4 couleurImGui =
        ImVec4(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f);

    ImGui::TextColored(couleurImGui, "Colony %d", colonie.getId());
    ImGui::SameLine();
    ImGui::Text(": %d twigs", colonie.getScore());
  }
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(10, 130), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(300, 0));
  ImGui::Begin("Graphs", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_AlwaysAutoResize);
  for (size_t i = 0; i < historiqueScores.size(); i++) {
    char label[32];
    sprintf(label, "Colony %zu", i);
    if (!historiqueScores[i].empty()) {
      ImGui::PlotLines(label, historiqueScores[i].data(),
                       historiqueScores[i].size(), 0, nullptr, 0.0f, 50.0f,
                       ImVec2(0, 40));
    }
  }
  ImGui::End();

  if (idTermiteInspecte != -1) {
    const Termite &t = jeu->getTermites()[idTermiteInspecte];
    ImGui::SetNextWindowPos(ImVec2(largeurEcran - 260, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250, 0));
    ImGui::Begin("Termite Inspector", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    ImGui::Text("ID : %d (Colony %d)", t.getId(), t.getIdColonie());
    ImGui::Separator();
    ImGui::Text("State : %s",
                t.porteBrindille() ? "Carries a Twig" : "Empty handed");
    ImGui::ProgressBar((float)t.getSablier() / t.getDureeSablier(),
                       ImVec2(-1, 0), "Action Timer");
    if (ImGui::Button("Close Inspection", ImVec2(-1, 0)))
      idTermiteInspecte = -1;
    ImGui::End();
  }

  ImGui::SetNextWindowPos(ImVec2(10, hauteurEcran - 150), ImGuiCond_Always);
  ImGui::Begin("Simulation", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_AlwaysAutoResize);

  if (ImGui::Button(enPause ? " Play " : " Pause ")) {
    enPause = !enPause;
  }
  ImGui::SameLine();
  ImGui::SliderFloat("Speed", &vitesseSimulation, 0.1f, 25.0f, "%.1fx");

  ImGui::Checkbox("View Pheromones", &afficherPheromones);

  if (ImGui::Button("Quit")) {
    reinitialiserJeu(JEU_PAR_DEFAUT, true);

    etatCourant = EtatApp::MENU_PRINCIPAL;
  }
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2(largeurEcran - 510, hauteurEcran - 460),
                          ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(500, 0));
  ImGui::Begin("Lab", nullptr, ImGuiWindowFlags_NoResize);

  if (ImGui::CollapsingHeader("Physics & Environment",
                              ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::SliderFloat("Evaporation rate", &laboConfig.tauxEvaporation, 0.0f,
                       0.05f, "%.4f");
    ImGui::SliderFloat("Pheromone deposite", &laboConfig.depotPheromone, 0.0f,
                       1.0f, "%.2f");
    ImGui::SliderFloat("Visual Opacity", &laboConfig.opaciteVisuelle, 0.0f,
                       1.0f, "%.2f");
    ImGui::SliderFloat("Base Delay", &laboConfig.delaiDeBase, 0.01f, 2.0f,
                       "%.2f");
  }

  if (ImGui::CollapsingHeader("Termites Behavior",
                              ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::SliderFloat("Anti-Clog (Radius)", &laboConfig.rayonAntiBouchon, 1.0f,
                       15.0f, "%.1f");
    ImGui::SliderInt("Nest Compacity ", &laboConfig.compaciteNid, 0, 8);
    ImGui::SliderFloat("GPS Noise (%)", &laboConfig.bruitGPS, 0.0f, 100.0f,
                       "%.0f");
  }

  if (ImGui::CollapsingHeader("Terrain Generation",
                              ImGuiTreeNodeFlags_DefaultOpen)) {
    bool modifierDecor = false;
    if (ImGui::SliderFloat("Noise Scale", &laboConfig.zoomPerlin, 0.5f, 10.0f,
                           "%.1f"))
      modifierDecor = true;
    if (ImGui::SliderInt("Dry Climate Threshold", &laboConfig.seuilClimatSec, 0,
                         255))
      modifierDecor = true;
    if (ImGui::SliderInt("Wet Climate Threshold", &laboConfig.seuilClimatHumide,
                         0, 255))
      modifierDecor = true;

    if (modifierDecor) {
      initialiserDecor();
    }
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
  mapDecor.assign(config.tailleGrille,
                  std::vector<CaseDecor>(
                      config.tailleGrille)); // On remplit la map de décor
                                             // avec des cases vides

  Image noise = GenImagePerlinNoise(config.tailleGrille, config.tailleGrille,
                                    laboConfig.offsetX, laboConfig.offsetY,
                                    laboConfig.zoomPerlin);
  Color *pixels = LoadImageColors(noise);

  Color colHumide = {78, 67, 56, 255};   // Terre sombre et grasse
  Color colNormal = {119, 98, 77, 255};  // Terre classique
  Color colAride = {161, 137, 113, 255}; // Terre sèche / Sableuse

  for (int i = 0; i < config.tailleGrille; i++) {
    for (int j = 0; j < config.tailleGrille; j++) {
      float perlinValue = pixels[i * config.tailleGrille + j].r;

      CaseDecor &cd = mapDecor[i][j];
      std::string sprite = "SOL_VIDE";
      int r = GetRandomValue(1, 100);

      if (perlinValue < laboConfig.seuilClimatSec) { // Humide
        cd.couleurFond = colHumide;
        if (r <= 5)
          sprite = "SOL_CHAMPI";
        else if (r <= 10)
          sprite = "SOL_MOUSSE";
      } else if (perlinValue < laboConfig.seuilClimatHumide) { // Normal
        cd.couleurFond = colNormal;
        if (r <= 3)
          sprite = "SOL_RACINE_2";
        else if (r <= 7)
          sprite = "SOL_CAILLOU_1";
        else if (r <= 10)
          sprite = "SOL_RACINE_1";
      } else { // Aride
        cd.couleurFond = colAride;
        if (r == 1)
          sprite = "SOL_CAILLOU_RARE"; // Vraiment rare
        else if (r <= 5)
          sprite = "SOL_CAILLOU_1";
        else if (r <= 10)
          sprite = "SOL_CAILLOU_2";
      }

      cd.nomSprite = sprite;
      cd.angle = GetRandomValue(0, 3) * 90.0f;
      cd.flipH = GetRandomValue(0, 1) == 1;
      cd.flipV = GetRandomValue(0, 1) == 1;

      cd.angleBrindille = GetRandomValue(0, 3) * 90.0f;
      cd.flipHBrindille = GetRandomValue(0, 1) == 1;
      cd.flipVBrindille = GetRandomValue(0, 1) == 1;
    }
  }
  UnloadImageColors(pixels);
  UnloadImage(noise);
}

void Application::reinitialiserJeu(const AppConfig &nouvelleConfig,
                                   bool pourMenu) {

  // Supprimer l'ancien jeu et les rendus associés
  if (jeu != nullptr) {
    delete jeu;
    rendusTermites.clear();
  }

  // Mise à jour de la configuration
  config = nouvelleConfig;
  jeu = new Jeu(config);

  // Reinitialiser le décor et les rendus
  initialiserDecor();
  synchroniserVisuels();

  // Réinitialiser les scores et la caméra
  historiqueScores.clear();
  for (int i = 0; i < config.nbColonies; i++) {
    historiqueScores.push_back({});
  }
  chronometre = 0.f;
  compteurEtapes = 0;
  idTermiteInspecte = -1; // On désinspecte tout termite à la réinitialisation
  vitesseSimulation = 1.f;

  // Réinitialiser la caméra pour centrer sur le monde
  float largeurEcran = (float)GetScreenWidth();
  float hauteurEcran = (float)GetScreenHeight();
  float tailleMonde = config.tailleGrille * TAILLE_TUILE;

  camera.target = {tailleMonde / 2.0f, tailleMonde / 2.0f};
  camera.offset = {largeurEcran / 2.0f, hauteurEcran / 2.0f};
  camera.rotation = 0.0f;

  if (pourMenu) {
    // Zoom correct pour le menu
    camera.zoom =
        std::sqrt(largeurEcran * largeurEcran + hauteurEcran * hauteurEcran) /
        tailleMonde;

    afficherPheromones =
        false; // Pas besoin d'afficher les phéromones dans le menu
  } else {
    camera.zoom =
        std::max(largeurEcran / tailleMonde, hauteurEcran / tailleMonde);
  }
}
