#include "Coord.hpp"
#include "doctest.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

// Coord
Coord::Coord(int lig, int col) {
  if (lig < 0 || lig >= TAILLE_GRILLE || col < 0 || col >= TAILLE_GRILLE) {
    throw std::out_of_range("Coordonnées " + std::to_string(lig) + ", " +
                            std::to_string(col) + " hors de la grille !");
  }
  this->lig = lig;
  this->col = col;
}

std::ostream &Coord::print(std::ostream &out) const {
  out << "(" << lig << ", " << col << ")";
  return out;
}

std::ostream &operator<<(std::ostream &out, const Coord &c) {
  return c.print(out);
}

// EnsCoord
int EnsCoord::position(const Coord &c) const {
  for (int i = 0; i < taille(); i++) {
    if (coords[i] == c) {
      return i;
    }
  }
  return -1;
}

void EnsCoord::ajoute(Coord c) {
  if (!contient(c)) {
    coords.push_back(c);
  }
}

void EnsCoord::supprime(const Coord &c) {
  int pos = position(c);
  if (pos != -1) {
    coords.erase(coords.begin() + pos);
  } else {
    throw std::invalid_argument("Coordonnée " + std::to_string(c.getLigne()) +
                                ", " + std::to_string(c.getColonne()) +
                                " non trouvée dans l'ensemble !");
  }
}

Coord EnsCoord::ieme(int i) const {
  if (i < 0 || i >= taille()) {
    throw std::out_of_range("Index " + std::to_string(i) +
                            " hors de l'ensemble !");
  }
  return coords[i];
}

Coord EnsCoord::choixHasard() const {
  if (estVide()) {
    throw std::runtime_error(
        "Ensemble vide, impossible de choisir une coordonnée !");
  }
  int index = rand() % taille();
  return coords[index];
}

// Fonctions

EnsCoord voisines(const Coord &c) {
  EnsCoord ens({});
  for (int i = std::max(c.getLigne() - 1, 0);
       i <= std::min(c.getLigne() + 1, TAILLE_GRILLE - 1); i++) {
    for (int j = std::max(c.getColonne() - 1, 0);
         j <= std::min(c.getColonne() + 1, TAILLE_GRILLE - 1); j++) {
      if (i != c.getLigne() || j != c.getColonne()) {
        ens.ajoute(Coord(i, j));
      }
    }
  }
  return ens;
}

// Tests Coord

TEST_CASE("Test du constructeur et des Getters de Coord") {
  Coord c(5, 10);
  CHECK(c.getLigne() == 5);
  CHECK(c.getColonne() == 10);
}

TEST_CASE("Test de l'opérateur d'égalité et de différence de Coord") {
  Coord c1(3, 4);
  Coord c2(3, 4);
  Coord c3(5, 6);

  CHECK(c1 == c2);
  CHECK(c1 != c3);
}

TEST_CASE("Test de l'opérateur de flux de Coord") {
  Coord c(2, 7);
  std::ostringstream oss;
  oss << c;
  CHECK(oss.str() == "(2, 7)");
}

TEST_CASE("Test de l'exception pour des coordonnées hors de la grille") {
  CHECK_THROWS_AS(Coord(-1, 5), std::out_of_range);
  CHECK_THROWS_AS(Coord(5, -1), std::out_of_range);
  CHECK_THROWS_AS(Coord(TAILLE_GRILLE, 5), std::out_of_range);
  CHECK_THROWS_AS(Coord(5, TAILLE_GRILLE), std::out_of_range);
}

// Tests EnsCoord
TEST_CASE("Test de la méthode contient de EnsCoord") {
  EnsCoord ens({Coord(1, 2), Coord(3, 4), Coord(5, 6)});

  CHECK(ens.contient(Coord(1, 2)) == true);
  CHECK(ens.contient(Coord(3, 4)) == true);
  CHECK(ens.contient(Coord(5, 6)) == true);
  CHECK(ens.contient(Coord(7, 8)) == false);
  CHECK(ens.contient(Coord(0, 0)) == false);
}

TEST_CASE("Test de la méthode ajoute de EnsCoord") {
  EnsCoord ens({Coord(1, 2), Coord(3, 4)});

  ens.ajoute(Coord(5, 6));
  CHECK(ens.contient(Coord(5, 6)) == true);

  ens.ajoute(Coord(1, 2));
  CHECK(ens.contient(Coord(1, 2)) == true);
}

TEST_CASE("Test de la méthode supprime de EnsCoord") {
  EnsCoord ens({Coord(1, 2), Coord(3, 4), Coord(5, 6)});

  ens.supprime(Coord(3, 4));
  CHECK(ens.contient(Coord(3, 4)) == false);

  CHECK_THROWS_AS(ens.supprime(Coord(7, 8)), std::invalid_argument);
}

TEST_CASE("Test de la méthode estVide de EnsCoord") {
  EnsCoord ens1({});
  EnsCoord ens2({Coord(1, 2)});
  EnsCoord ens3({Coord(0, 0)});
  EnsCoord ens4({Coord(19, 19)});

  CHECK(ens1.estVide() == true);
  CHECK(ens2.estVide() == false);
  CHECK(ens3.estVide() == false);
  CHECK(ens4.estVide() == false);
}

TEST_CASE("Test de la méthode taille de EnsCoord") {
  EnsCoord ens1({});
  EnsCoord ens2({Coord(1, 2)});
  EnsCoord ens3({Coord(0, 0), Coord(1, 1), Coord(2, 2)});

  CHECK(ens1.taille() == 0);
  CHECK(ens2.taille() == 1);
  CHECK(ens3.taille() == 3);
}

TEST_CASE("Test de la méthode ieme de EnsCoord") {
  EnsCoord ens({Coord(1, 2), Coord(3, 4), Coord(5, 6)});

  CHECK(ens.ieme(0) == Coord(1, 2));
  CHECK(ens.ieme(1) == Coord(3, 4));
  CHECK(ens.ieme(2) == Coord(5, 6));

  CHECK_THROWS_AS(ens.ieme(-1), std::out_of_range);
  CHECK_THROWS_AS(ens.ieme(3), std::out_of_range);
}

TEST_CASE("Test de la méthode choixHasard de EnsCoord") {
  EnsCoord ens({});
  CHECK_THROWS_AS(ens.choixHasard(), std::runtime_error);

  EnsCoord ens2({Coord(1, 2)});
  CHECK(ens2.choixHasard() == Coord(1, 2));
  CHECK(ens2.choixHasard() == Coord(1, 2));

  EnsCoord ens3({Coord(0, 0), Coord(1, 1), Coord(2, 2)});
  for (int i = 0; i < 10; i++) {
    Coord c = ens3.choixHasard();
    CHECK(ens3.contient(c) == true);
  }
}

// Test de la fonction voisines
TEST_CASE("Test de la fonction voisines (Les 9 cas limites sur la grille)") {

  SUBCASE("Milieu") {
    Coord c(5, 5);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 8);
    CHECK(v.contient(Coord(4, 4))); // Haut-Gauche
    CHECK(v.contient(Coord(6, 6))); // Bas-Droit
    CHECK(v.contient(Coord(5, 5)) == false);
  }

  SUBCASE("Coin Haut-Gauche") {
    Coord c(0, 0);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 3);
    CHECK(v.contient(Coord(0, 1))); // Droite
    CHECK(v.contient(Coord(1, 0))); // Bas
    CHECK(v.contient(Coord(1, 1))); // Bas-Droite
    CHECK(v.contient(Coord(0, 0)) == false);
  }

  SUBCASE("Coin Bas-Droit") {
    Coord c(TAILLE_GRILLE - 1, TAILLE_GRILLE - 1);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 3);
    CHECK(v.contient(Coord(TAILLE_GRILLE - 2, TAILLE_GRILLE - 1))); // Haut
    CHECK(v.contient(Coord(TAILLE_GRILLE - 1, TAILLE_GRILLE - 2))); // Gauche
    CHECK(
        v.contient(Coord(TAILLE_GRILLE - 2, TAILLE_GRILLE - 2))); // Haut-Gauche
    CHECK(v.contient(Coord(TAILLE_GRILLE - 1, TAILLE_GRILLE - 1)) == false);
  }

  SUBCASE("Coin Haut-Droit") {
    Coord c(0, TAILLE_GRILLE - 1);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 3);
    CHECK(v.contient(Coord(0, TAILLE_GRILLE - 2))); // Gauche
    CHECK(v.contient(Coord(1, TAILLE_GRILLE - 1))); // Bas
    CHECK(v.contient(Coord(1, TAILLE_GRILLE - 2))); // Bas-Gauche
    CHECK(v.contient(Coord(0, TAILLE_GRILLE - 1)) == false);
  }

  SUBCASE("Coin Bas-Gauche") {
    Coord c(TAILLE_GRILLE - 1, 0);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 3);
    CHECK(v.contient(Coord(TAILLE_GRILLE - 2, 0))); // Haut
    CHECK(v.contient(Coord(TAILLE_GRILLE - 1, 1))); // Droite
    CHECK(v.contient(Coord(TAILLE_GRILLE - 2, 1))); // Haut-Droite
    CHECK(v.contient(Coord(TAILLE_GRILLE - 1, 0)) == false);
  }

  SUBCASE("Bord Haut") {
    Coord c(0, 5);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 5);
    CHECK(v.contient(Coord(0, 4))); // Gauche
    CHECK(v.contient(Coord(0, 6))); // Droite
    CHECK(v.contient(Coord(1, 4))); // Bas-Gauche
    CHECK(v.contient(Coord(1, 5))); // Bas
    CHECK(v.contient(Coord(1, 6))); // Bas-Droite
    CHECK(v.contient(Coord(0, 5)) == false);
  }

  SUBCASE("Bord Bas") {
    Coord c(TAILLE_GRILLE - 1, 5);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 5);
    CHECK(v.contient(Coord(TAILLE_GRILLE - 1, 4))); // Gauche
    CHECK(v.contient(Coord(TAILLE_GRILLE - 1, 6))); // Droite
    CHECK(v.contient(Coord(TAILLE_GRILLE - 2, 4))); // Haut-Gauche
    CHECK(v.contient(Coord(TAILLE_GRILLE - 2, 5))); // Haut
    CHECK(v.contient(Coord(TAILLE_GRILLE - 2, 6))); // Haut-Droite
    CHECK(v.contient(Coord(TAILLE_GRILLE - 1, 5)) == false);
  }

  SUBCASE("Bord Gauche") {
    Coord c(5, 0);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 5);
    CHECK(v.contient(Coord(4, 0))); // Haut
    CHECK(v.contient(Coord(6, 0))); // Bas
    CHECK(v.contient(Coord(4, 1))); // Haut-Droite
    CHECK(v.contient(Coord(5, 1))); // Droite
    CHECK(v.contient(Coord(6, 1))); // Bas-Droite
    CHECK(v.contient(Coord(5, 0)) == false);
  }

  SUBCASE("Bord Droit") {
    Coord c(5, TAILLE_GRILLE - 1);
    EnsCoord v = voisines(c);
    CHECK(v.taille() == 5);
    CHECK(v.contient(Coord(4, TAILLE_GRILLE - 1))); // Haut
    CHECK(v.contient(Coord(6, TAILLE_GRILLE - 1))); // Bas
    CHECK(v.contient(Coord(4, TAILLE_GRILLE - 2))); // Haut-Gauche
    CHECK(v.contient(Coord(5, TAILLE_GRILLE - 2))); // Gauche
    CHECK(v.contient(Coord(6, TAILLE_GRILLE - 2))); // Bas-Gauche
    CHECK(v.contient(Coord(5, TAILLE_GRILLE - 1)) == false);
  }
}
