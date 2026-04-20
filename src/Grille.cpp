#include "Grille.hpp"
#include "doctest.h"

#include "parametres.hpp"

bool Grille::contientBrindille(Coord c) const {
  return cases[c.getLig()][c.getCol()].type == TypeCase::BRINDILLE;
};

void Grille::poseBrindille(Coord c, int idColonie) {
  if (contientBrindille(c)) {
    throw std::logic_error("Il y a déjà une brindille à cette position");
  }
  if (cases[c.getLig()][c.getCol()].idTermite != -1) {
    throw std::logic_error("Il y a déjà une termite à cette position");
  }
  cases[c.getLig()][c.getCol()].type = TypeCase::BRINDILLE;
  cases[c.getLig()][c.getCol()].idColonie = idColonie;
};

void Grille::enleveBrindille(Coord c) {
  if (!contientBrindille(c)) {
    throw std::logic_error("Il n'y a pas de brindille à cette position");
  }
  cases[c.getLig()][c.getCol()].type = TypeCase::VIDE;
  cases[c.getLig()][c.getCol()].idColonie = -1;
}

void Grille::poseTermite(Coord c, int idT) {
  if (cases[c.getLig()][c.getCol()].idTermite != -1) {
    throw std::logic_error("Il y a déjà une termite à cette position");
  }
  if (contientBrindille(c)) {
    throw std::logic_error("Il y a déjà une brindille à cette position");
  }
  cases[c.getLig()][c.getCol()].idTermite = idT;
}

void Grille::enleveTermite(Coord c) {
  if (cases[c.getLig()][c.getCol()].idTermite == -1) {
    throw std::logic_error("Il n'y a pas de termite à cette position");
  }
  cases[c.getLig()][c.getCol()].idTermite = -1;
}

int Grille::numéroTermite(Coord c) const {
  return cases[c.getLig()][c.getCol()].idTermite;
}

bool Grille::contientNid(Coord c) const {
  return cases[c.getLig()][c.getCol()].type == TypeCase::NID;
}
void Grille::poseNid(Coord c, int idColonie) {
  if (!estVide(c)) {
    throw std::logic_error("Il y a déjà quelque chose à cette position");
  }
  cases[c.getLig()][c.getCol()].type = TypeCase::NID;
  cases[c.getLig()][c.getCol()].idColonie = idColonie;
}

int Grille::proprietaireCase(Coord c) const {
  return cases[c.getLig()][c.getCol()].idColonie;
}

bool Grille::estVide(Coord c) const {
  return cases[c.getLig()][c.getCol()].idTermite == -1 &&
         !(cases[c.getLig()][c.getCol()].type == TypeCase::BRINDILLE);
}

std::ostream &Grille::print(std::ostream &out) const {
  for (int i = 0; i < (int)cases.size(); i++) {
    for (int j = 0; j < (int)cases[i].size(); j++) {
      if (cases[i][j].idTermite != -1) {
        out << "T ";
      } else if (contientBrindille(Coord(i, j))) {
        out << "* ";
      } else {
        out << "  ";
      }
    }
    out << "\n";
  }
  return out;
}

std::ostream &operator<<(std::ostream &out, const Grille &g) {
  return g.print(out);
}

// Tests

TEST_CASE("Test de la classe Grille") {
  Grille g(TAILLE_GRILLE);

  SUBCASE("Test de poseBrindille et contientBrindille") {
    Coord c(5, 5);
    CHECK(g.contientBrindille(c) == false);
    g.poseBrindille(c);
    CHECK(g.contientBrindille(c) == true);
    CHECK_THROWS_AS(g.poseBrindille(c), std::logic_error);
  }

  SUBCASE("Test de enleveBrindille") {
    Coord c(5, 5);
    g.poseBrindille(c);
    CHECK(g.contientBrindille(c) == true);
    g.enleveBrindille(c);
    CHECK(g.contientBrindille(c) == false);
    CHECK_THROWS_AS(g.enleveBrindille(c), std::logic_error);
  }

  SUBCASE("Test de poseTermite et numéroTermite") {
    Coord c(5, 5);
    Coord c_vide(6, 6);
    CHECK(g.estVide(c) == true);
    g.poseTermite(c, 1);
    CHECK(g.numéroTermite(c) == 1);
    CHECK(g.numéroTermite(c_vide) == -1);
    CHECK_THROWS_AS(g.poseTermite(c, 2), std::logic_error);
  }

  SUBCASE("Test de enleveTermite") {
    Coord c(5, 5);
    g.poseTermite(c, 1);
    CHECK(g.numéroTermite(c) == 1);
    g.enleveTermite(c);
    CHECK(g.estVide(c) == true);
    CHECK_THROWS_AS(g.enleveTermite(c), std::logic_error);
  }

  SUBCASE("Test de l'exclusion mutuelle (Brindille vs Termite)") {
    Coord c(10, 10);
    g.poseBrindille(c);

    // On ne doit pas pouvoir poser un termite sur une brindille
    CHECK_THROWS_AS(g.poseTermite(c, 1), std::logic_error);

    g.enleveBrindille(c);
    g.poseTermite(c, 1);

    // On ne doit pas pouvoir poser une brindille sur un termite
    CHECK_THROWS_AS(g.poseBrindille(c), std::logic_error);
  }

  SUBCASE("Test de estVide") {
    Coord c(1, 1);
    CHECK(g.estVide(c) == true);

    g.poseBrindille(c);
    CHECK(g.estVide(c) == false);

    g.enleveBrindille(c);
    g.poseTermite(c, 99);
    CHECK(g.estVide(c) == false);
  }
}
