#include "Grille.hpp"
#include "doctest.h"

bool Grille::contientBrindille(Coord c) const {
  return cases.at(c.getLig()).at(c.getCol()).type == TypeCase::BRINDILLE;
};

void Grille::poseBrindille(Coord c, int idColonie) {
  if (contientBrindille(c)) {
    throw std::logic_error("Il y a déjà une brindille à cette position");
  }
  if (cases.at(c.getLig()).at(c.getCol()).idTermite != -1) {
    throw std::logic_error("Il y a déjà une termite à cette position");
  }
  cases.at(c.getLig()).at(c.getCol()).type = TypeCase::BRINDILLE;
  cases.at(c.getLig()).at(c.getCol()).idColonie = idColonie;
};

void Grille::enleveBrindille(Coord c) {
  if (!contientBrindille(c)) {
    throw std::logic_error("Il n'y a pas de brindille à cette position");
  }
  cases.at(c.getLig()).at(c.getCol()).type = TypeCase::VIDE;
  cases.at(c.getLig()).at(c.getCol()).idColonie = -1;
}

void Grille::poseTermite(Coord c, int idT) {
  if (cases.at(c.getLig()).at(c.getCol()).idTermite != -1) {
    throw std::logic_error("Il y a déjà une termite à cette position");
  }
  if (contientBrindille(c)) {
    throw std::logic_error("Il y a déjà une brindille à cette position");
  }
  cases.at(c.getLig()).at(c.getCol()).idTermite = idT;
}

void Grille::enleveTermite(Coord c) {
  if (cases.at(c.getLig()).at(c.getCol()).idTermite == -1) {
    throw std::logic_error("Il n'y a pas de termite à cette position");
  }
  cases.at(c.getLig()).at(c.getCol()).idTermite = -1;
}

int Grille::numéroTermite(Coord c) const {
  return cases.at(c.getLig()).at(c.getCol()).idTermite;
}

bool Grille::contientNid(Coord c) const {
  return cases.at(c.getLig()).at(c.getCol()).type == TypeCase::NID;
}
void Grille::poseNid(Coord c, int idColonie) {
  int lig = c.getLig();
  int col = c.getCol();

  if (lig + 1 >= taille || col + 1 >= taille) {
    throw std::logic_error("Le nid déborde de la grille !");
  }

  if (!estVide(c) || !estVide(Coord(lig + 1, col)) ||
      !estVide(Coord(lig, col + 1)) || !estVide(Coord(lig + 1, col + 1))) {
    throw std::logic_error(
        "Il n'y a pas assez d'espace (2x2) pour poser le nid");
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      cases.at(lig + i).at(col + j).type = TypeCase::NID;
      cases.at(lig + i).at(col + j).idColonie = idColonie;
    }
  }
}

int Grille::proprietaireCase(Coord c) const {
  return cases.at(c.getLig()).at(c.getCol()).idColonie;
}

bool Grille::estVide(Coord c) const {
  return cases.at(c.getLig()).at(c.getCol()).type == TypeCase::VIDE &&
         cases.at(c.getLig()).at(c.getCol()).idTermite == -1;
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

void Grille::initialiserPheromones(int nbColonies) {
  for (auto &ligne : cases) {
    for (auto &c : ligne) {
      c.pheromones.assign(nbColonies, 0.0f);
    }
  }
}

void Grille::deposerPheromone(Coord c, int idColonie, float quantite) {
  cases.at(c.getLig()).at(c.getCol()).pheromones.at(idColonie) += quantite;
  if (cases.at(c.getLig()).at(c.getCol()).pheromones.at(idColonie) > 1.0f)
    cases.at(c.getLig()).at(c.getCol()).pheromones.at(idColonie) = 1.0f;
};

float Grille::getIntensitePheromone(Coord c, int idColonie) const {
  return cases.at(c.getLig()).at(c.getCol()).pheromones.at(idColonie);
};

void Grille::evaporerPheromones(float taux) {
  for (auto &ligne : cases) {
    for (auto &c : ligne) {
      for (float &p : c.pheromones) {
        if (p > 0)
          p -= taux;
        if (p < 0)
          p = 0;
      }
    }
  }
};

// Tests

TEST_CASE("Test de la classe Grille") {
  Grille g(100);

  SUBCASE("Test des exceptions pour des coordonnées hors de la grille") {
    Coord c_hors_limite_lig(100, 5);
    Coord c_hors_limite_col(5, 100);

    // On s'assure que Grille rejette bien les accès au-delà de sa taille
    // dynamique (100 ici)
    CHECK_THROWS_AS(g.estVide(c_hors_limite_lig), std::out_of_range);
    CHECK_THROWS_AS(g.estVide(c_hors_limite_col), std::out_of_range);

    // Et test sur la pose d'éléments en dehors de la grille
    CHECK_THROWS_AS(g.poseBrindille(c_hors_limite_lig), std::out_of_range);
    CHECK_THROWS_AS(g.poseTermite(c_hors_limite_col, 1), std::out_of_range);

    // Remarque : les coordonnées négatives sont toujours bloquées par Coord
    CHECK_THROWS_AS(Coord(-1, 5), std::out_of_range);
    CHECK_THROWS_AS(Coord(5, -1), std::out_of_range);
  }

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

  SUBCASE("Test de poseNid et contientNid") {
    Coord c(5, 5);
    CHECK(g.contientNid(c) == false);

    g.poseNid(c, 1);
    CHECK(g.contientNid(c) == true);
    CHECK(g.proprietaireCase(c) == 1);
    CHECK(g.estVide(c) == false);

    // On ne doit pas pouvoir poser un nid sur une case déjà occupée
    CHECK_THROWS_AS(g.poseNid(c, 2), std::logic_error);

    Coord c2(6, 6);
    g.poseBrindille(c2);
    CHECK_THROWS_AS(g.poseNid(c2, 1), std::logic_error);

    Coord c3(7, 7);
    g.poseTermite(c3, 1);
    CHECK_THROWS_AS(g.poseNid(c3, 1), std::logic_error);
  }

  SUBCASE("Tests des phéromones") {
    Coord c(5, 5);
    g.initialiserPheromones(3);

    CHECK(g.getIntensitePheromone(c, 0) == 0.0f);
    CHECK(g.getIntensitePheromone(c, 1) == 0.0f);
    CHECK(g.getIntensitePheromone(c, 2) == 0.0f);

    g.deposerPheromone(c, 1, 0.5f);
    CHECK(g.getIntensitePheromone(c, 0) == 0.0f);
    CHECK(g.getIntensitePheromone(c, 1) == 0.5f);
    CHECK(g.getIntensitePheromone(c, 2) == 0.0f);

    g.deposerPheromone(c, 1, 0.6f); // Doit plafonner à 1.0
    CHECK(g.getIntensitePheromone(c, 1) == 1.0f);

    g.evaporerPheromones(0.3f);
    CHECK(g.getIntensitePheromone(c, 1) == 0.7f);

    g.evaporerPheromones(1.0f); // Doit descendre à 0
    CHECK(g.getIntensitePheromone(c, 1) == 0.0f);
  }
}
