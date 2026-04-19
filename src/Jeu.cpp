#include "Jeu.hpp"
#include "Couleurs.hpp"
#include "Grille.hpp"
#include "parametres.hpp"
#include <doctest.h>
#include <stdexcept>
#include <vector>

Jeu::Jeu(int nbTermites, float densiteBrindilles)
    : grille(TAILLE_GRILLE), numeroEtape(0) {

  for (int i = 0; i < TAILLE_GRILLE; i++) {
    for (int j = 0; j < TAILLE_GRILLE; j++) {
      if (rand() % 100 < (int)(densiteBrindilles * 100)) {
        grille.poseBrindille(Coord(i, j));
      }
    }
  }

  for (int i = 0; i < nbTermites; i++) {
    Coord pos(0, 0);
    do {
      pos = Coord(rand() % TAILLE_GRILLE, rand() % TAILLE_GRILLE);
    } while (!grille.estVide(pos));
    grille.poseTermite(pos, i);
    termites.push_back(Termite(pos, i));
  }
}

void Jeu::etapeSuivante() {
  std::vector<int> ordreDePassage(termites.size());
  for (int i = 0; i < (int)termites.size(); i++) {
    ordreDePassage[i] = i;
  }

  // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
  for (int i = 0; i < (int)ordreDePassage.size() - 1; i++) {
    int j = i + rand() % (ordreDePassage.size() - i);
    std::swap(ordreDePassage[i], ordreDePassage[j]);
  }

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

      if (idTermite != -1) {
        out << Console::ROUGE << termites.at(idTermite).toString()
            << Console::RESET << " ";
      } else if (grille.contientBrindille(posCourante)) {
        out << Console::JAUNE << "* " << Console::RESET;
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

  for (size_t i = 0; i < termites.size(); i++) {
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
        }
      }
    }
  }
}
