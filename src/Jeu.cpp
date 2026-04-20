#include "Jeu.hpp"
#include "Colonie.hpp"
#include "Couleurs.hpp"
#include "Grille.hpp"
#include "Termite.hpp"
#include "parametres.hpp"
#include "utilitaires.hpp"
#include <cstdlib>
#include <doctest.h>
#include <stdexcept>
#include <vector>

Jeu::Jeu(int nbTermitesParColonie, float densiteBrindilles, int nbColonies)
    : grille(TAILLE_GRILLE), numeroEtape(0) {

  if (nbColonies > TAILLE_GRILLE * TAILLE_GRILLE / 8) {
    throw std::invalid_argument(
        "Le nombre de colonies est trop élevé pour la taille de la grille");
  }

  std::vector<std::string> couleurs = {
      Console::ROUGE,         Console::VERT,       Console::JAUNE,
      Console::BLEU,          Console::MAGENTA,    Console::CYAN,
      Console::ROUGE_CLAIR,   Console::VERT_CLAIR, Console::BLEU_CLAIR,
      Console::MAGENTA_CLAIR,
  };

  melanger(couleurs);

  // https://en.wikipedia.org/wiki/K-means%2B%2B#Improved_initialization_algorithm
  Colonie premiereColonie(0,
                          Coord(rand() % TAILLE_GRILLE, rand() % TAILLE_GRILLE),
                          6, 0.1, couleurs[0]);
  colonies.push_back(premiereColonie);

  for (int k = 1; k < nbColonies; k++) {
    std::vector<double> distanceCarrees;
    double sommeDistances = 0;

    for (int i = 0; i < TAILLE_GRILLE; i++) {
      for (int j = 0; j < TAILLE_GRILLE; j++) {
        Coord c(i, j);

        double distPlusProche = -1;
        for (const auto &colonie : colonies) {
          double d = c.distance(colonie.getPosition());

          if (distPlusProche == -1 || d < distPlusProche)
            distPlusProche = d;
        }

        distanceCarrees.push_back(distPlusProche * distPlusProche);
        sommeDistances += (distPlusProche * distPlusProche);
      }
    }

    // Choix de la position des nids basé sur la distance au carré
    double cible = (double)rand() / RAND_MAX * sommeDistances;
    double cumul = 0;
    for (int i = 0; i < (int)distanceCarrees.size(); i++) {
      cumul += distanceCarrees[i];
      if (cumul >= cible) {
        int x = i / TAILLE_GRILLE;
        int y = i % TAILLE_GRILLE;
        Colonie nouvelleColonie(k, Coord(x, y), 6, 0.1,
                                couleurs[k % (int)couleurs.size()]);
        colonies.push_back(nouvelleColonie);
        break;
      }
    }
  }

  // Placement des nids
  for (const Colonie &colonie : colonies) {
    grille.poseNid(colonie.getPosition(), colonie.getId());
  }

  // Placement des termites
  int idTermite = 0;

  for (const Colonie &colonie : colonies) {
    for (int i = 0; i < nbTermitesParColonie; i++) {
      Coord pos(0, 0);
      do {
        pos = Coord(rand() % TAILLE_GRILLE, rand() % TAILLE_GRILLE);
      } while (!grille.estVide(pos));
      grille.poseTermite(pos, idTermite);
      termites.push_back(Termite(idTermite, colonie.getId(), pos));
      idTermite++;
    }
  }

  for (int i = 0; i < TAILLE_GRILLE; i++) {
    for (int j = 0; j < TAILLE_GRILLE; j++) {
      Coord c(i, j);
      if (grille.estVide(c) && rand() % 100 < (int)(densiteBrindilles * 100)) {
        grille.poseBrindille(c, -1);
      }
    }
  }
}

void Jeu::etapeSuivante() {
  std::vector<int> ordreDePassage(termites.size());
  for (int i = 0; i < (int)termites.size(); i++) {
    ordreDePassage[i] = i;
  }

  melanger(ordreDePassage);

  for (int id : ordreDePassage) {
    termites[id].vieTermite(grille);
  }

  verifieIntegrite();
  numeroEtape++;
}

std::ostream &Jeu::afficheJeu(std::ostream &out) const {
  out << Console::CYAN << "Étape " << Console::GRAS << numeroEtape << " :\n"
      << Console::RESET;

  for (int i = 0; i < TAILLE_GRILLE; i++) {
    for (int j = 0; j < TAILLE_GRILLE; j++) {
      Coord posCourante(i, j);
      int idTermite = grille.numéroTermite(posCourante);
      int idColonie = grille.proprietaireCase(posCourante);

      if (idTermite != -1) {
        Termite t = termites.at(idTermite);
        out << colonies[t.getIdColonie()].getCouleur() << t.toString()
            << Console::RESET << " ";

      } else if (grille.contientBrindille(posCourante)) {
        out << ((idColonie == -1) ? Console::RESET
                                  : colonies[idColonie].getCouleur())
            << "* " << Console::RESET;

      } else if (grille.contientNid(posCourante)) {
        out << Console::GRAS << colonies[idColonie].getCouleur() << "#"
            << Console::RESET << " ";

      } else {
        out << "  ";
      }
    }
    out << "\n";
  }
  return out;
}

std::ostream &operator<<(std::ostream &out, const Jeu &j) {
  return j.afficheJeu(out);
}

void Jeu::verifieIntegrite() const {

  for (int i = 0; i < (int)termites.size(); i++) {
    const Termite &t = termites[i];
    int id = t.getId();

    if (id != (int)i) {
      throw std::logic_error(
          "Termite stocké au mauvais indice ! Index: " + std::to_string(i) +
          " mais ID: " + std::to_string(id));
    }

    Coord pos = t.getPosition();
    if (grille.numéroTermite(pos) != id) {
      throw std::logic_error("Incohérence entre la position du termite " +
                             std::to_string(id) + " et la grille");

      int idCol = t.getIdColonie();
      if (idCol < 0 || idCol >= (int)colonies.size()) {
        throw std::logic_error("Termite " + std::to_string(id) +
                               " a une colonie avec un id " +
                               std::to_string(idCol) + " invalide");
      }
    }
  }

  for (int i = 0; i < (int)colonies.size(); i++) {
    const Colonie &c = colonies[i];
    int id = c.getId();

    if (id != (int)i) {
      throw std::logic_error(
          "Colonie stockée au mauvais indice ! Index: " + std::to_string(i) +
          " mais ID: " + std::to_string(id));
    }

    Coord pos = c.getPosition();
    if (!grille.contientNid(pos)) {
      throw std::logic_error("Incohérence entre la position de la colonie " +
                             std::to_string(id) + " et la grille");
    }
  }

  for (int i = 0; i < TAILLE_GRILLE; i++) {
    for (int j = 0; j < TAILLE_GRILLE; j++) {
      Coord pos(i, j);
      int idTermite = grille.numéroTermite(pos);
      if (idTermite != -1) {
        if (idTermite < 0 || idTermite >= (int)termites.size()) {
          throw std::logic_error(
              "Grille contient un termite avec un id invalide : " +
              std::to_string(idTermite));
        }
        if (termites.at(idTermite).getPosition() != pos) {
          throw std::logic_error("Incohérence entre la position du termite " +
                                 std::to_string(idTermite) + " et la grille");

          if (grille.contientNid(pos)) {
            int idColonie = grille.proprietaireCase(pos);
            if (idColonie < 0 || idColonie >= (int)colonies.size()) {
              throw std::logic_error(
                  "Grille contient un nid avec un id de colonie invalide : " +
                  std::to_string(idColonie));
            }
            if (colonies.at(idColonie).getPosition() != pos) {
              throw std::logic_error(
                  "Incohérence entre la position du nid de la colonie " +
                  std::to_string(idColonie) + " et la grille");
            }
          }
        }
      }
    }
  }
}

void Jeu::calculerScores() {
  for (Colonie &colonie : colonies) {
    colonie.reinitialiserScore();
  }

  for (int i = 0; i < TAILLE_GRILLE; i++) {
    for (int j = 0; j < TAILLE_GRILLE; j++) {
      Coord c(i, j);

      if (grille.contientBrindille(c)) {
        if (grille.proprietaireCase(c) != 1) {
          colonies[grille.proprietaireCase(c)].incrementerScore();
        }
      }
    }
  }
}
