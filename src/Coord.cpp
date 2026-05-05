#include "Coord.hpp"
#include "doctest.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

// Coord
Coord::Coord(int lig, int col) : lig(lig), col(col) {
  if (lig < 0 || col < 0) {
    throw std::out_of_range("Coordonnées " + std::to_string(lig) + ", " +
                            std::to_string(col) + " hors de la grille !");
  }
}

double Coord::distance(const Coord &c) const {
  int dLig = lig - c.lig;
  int dCol = col - c.col;
  return std::sqrt(dLig * dLig + dCol * dCol);
}

std::ostream &Coord::print(std::ostream &out) const {
  out << "(" << lig << ", " << col << ")";
  return out;
}

std::ostream &operator<<(std::ostream &out, const Coord &c) {
  return c.print(out);
}

// Direction

std::ostream &operator<<(std::ostream &out, const Direction &d) {
  switch (d) {
  case Direction::NORD_OUEST:
    out << "Nord Ouest";
    break;
  case Direction::NORD:
    out << "Nord";
    break;
  case Direction::NORD_EST:
    out << "Nord Est";
    break;
  case Direction::EST:
    out << "Est";
    break;
  case Direction::SUD_EST:
    out << "Sud Est";
    break;
  case Direction::SUD:
    out << "Sud";
    break;
  case Direction::SUD_OUEST:
    out << "Sud Ouest";
    break;
  case Direction::OUEST:
    out << "Ouest";
    break;
  }
  return out;
}

Direction aGauche(Direction d) {
  return static_cast<Direction>((static_cast<int>(d) + 7) % 8);
}

Direction aDroite(Direction d) {
  return static_cast<Direction>((static_cast<int>(d) + 1) % 8);
}

Coord devantCoord(const Coord &c, Direction d) {
  int lig = c.getLig();
  int col = c.getCol();

  switch (d) {
  case Direction::NORD_OUEST:
    return Coord(lig - 1, col - 1);
  case Direction::NORD:
    return Coord(lig - 1, col);
  case Direction::NORD_EST:
    return Coord(lig - 1, col + 1);
  case Direction::EST:
    return Coord(lig, col + 1);
  case Direction::SUD_EST:
    return Coord(lig + 1, col + 1);
  case Direction::SUD:
    return Coord(lig + 1, col);
  case Direction::SUD_OUEST:
    return Coord(lig + 1, col - 1);
  case Direction::OUEST:
    return Coord(lig, col - 1);
  }
}

// Tests Coord

TEST_CASE("Test de la classe Coord") {
  SUBCASE("Test de la méthode print de Coord") {
    Coord c(3, 4);
    std::ostringstream oss;
    c.print(oss);
    CHECK(oss.str() == "(3, 4)");
  }

  SUBCASE("Test de l'opérateur de flux de Coord") {
    Coord c(1, 2);
    std::ostringstream oss;
    oss << c;
    CHECK(oss.str() == "(1, 2)");
  }

  SUBCASE("Test du constructeur et des Getters de Coord") {
    Coord c(5, 10);
    CHECK(c.getLig() == 5);
    CHECK(c.getCol() == 10);
  }

  SUBCASE("Test de l'opérateur d'égalité et de différence de Coord") {
    Coord c1(3, 4);
    Coord c2(3, 4);
    Coord c3(5, 6);

    CHECK(c1 == c2);
    CHECK(c1 != c3);
  }

  SUBCASE("Test de l'opérateur de flux de Coord") {
    Coord c(2, 7);
    std::ostringstream oss;
    oss << c;
    CHECK(oss.str() == "(2, 7)");
  }

  SUBCASE("Test de l'exception pour des coordonnées hors de la grille") {
    CHECK_THROWS_AS(Coord(-1, 5), std::out_of_range);
    CHECK_THROWS_AS(Coord(5, -1), std::out_of_range);
  }
}

// Tests Direction

TEST_CASE("Test de la classe Direction") {
  SUBCASE("Test de l'opérateur de flux de Direction") {
    CHECK((std::ostringstream() << Direction::NORD_OUEST).str() ==
          "Nord Ouest");
    CHECK((std::ostringstream() << Direction::NORD).str() == "Nord");
    CHECK((std::ostringstream() << Direction::NORD_EST).str() == "Nord Est");
    CHECK((std::ostringstream() << Direction::EST).str() == "Est");
    CHECK((std::ostringstream() << Direction::SUD_EST).str() == "Sud Est");
    CHECK((std::ostringstream() << Direction::SUD).str() == "Sud");
    CHECK((std::ostringstream() << Direction::SUD_OUEST).str() == "Sud Ouest");
    CHECK((std::ostringstream() << Direction::OUEST).str() == "Ouest");
  }

  SUBCASE("Test de l'opérateur de flux de Direction") {
    CHECK((std::ostringstream() << Direction::NORD_OUEST).str() ==
          "Nord Ouest");
    CHECK((std::ostringstream() << Direction::NORD).str() == "Nord");
    CHECK((std::ostringstream() << Direction::NORD_EST).str() == "Nord Est");
    CHECK((std::ostringstream() << Direction::EST).str() == "Est");
    CHECK((std::ostringstream() << Direction::SUD_EST).str() == "Sud Est");
    CHECK((std::ostringstream() << Direction::SUD).str() == "Sud");
    CHECK((std::ostringstream() << Direction::SUD_OUEST).str() == "Sud Ouest");
    CHECK((std::ostringstream() << Direction::OUEST).str() == "Ouest");
  }

  SUBCASE("Test de la fonction aGauche") {
    CHECK(aGauche(Direction::NORD_OUEST) == Direction::OUEST);
    CHECK(aGauche(Direction::NORD) == Direction::NORD_OUEST);
    CHECK(aGauche(Direction::NORD_EST) == Direction::NORD);
    CHECK(aGauche(Direction::EST) == Direction::NORD_EST);
    CHECK(aGauche(Direction::SUD_EST) == Direction::EST);
    CHECK(aGauche(Direction::SUD) == Direction::SUD_EST);
    CHECK(aGauche(Direction::SUD_OUEST) == Direction::SUD);
    CHECK(aGauche(Direction::OUEST) == Direction::SUD_OUEST);
  }

  SUBCASE("Test de la fonction aDroite") {
    CHECK(aDroite(Direction::NORD_OUEST) == Direction::NORD);
    CHECK(aDroite(Direction::NORD) == Direction::NORD_EST);
    CHECK(aDroite(Direction::NORD_EST) == Direction::EST);
    CHECK(aDroite(Direction::EST) == Direction::SUD_EST);
    CHECK(aDroite(Direction::SUD_EST) == Direction::SUD);
    CHECK(aDroite(Direction::SUD) == Direction::SUD_OUEST);
    CHECK(aDroite(Direction::SUD_OUEST) == Direction::OUEST);
    CHECK(aDroite(Direction::OUEST) == Direction::NORD_OUEST);
  }

  SUBCASE("Test du cycle de aDroite et aGauche") {
    CHECK(aDroite(aGauche(Direction::NORD)) == Direction::NORD);
    CHECK(aGauche(aDroite(Direction::EST)) == Direction::EST);

    Direction d = Direction::NORD;
    for (int i = 0; i < 8; i++) {
      d = aDroite(d);
    }
    CHECK(d == Direction::NORD);
    d = Direction::EST;
    for (int i = 0; i < 8; i++) {
      d = aGauche(d);
    }
    CHECK(d == Direction::EST);
  }

  SUBCASE("Test de la fonction devantCoord") {
    Coord c(5, 5);
    CHECK(devantCoord(c, Direction::NORD_OUEST) == Coord(4, 4));
    CHECK(devantCoord(c, Direction::NORD) == Coord(4, 5));
    CHECK(devantCoord(c, Direction::NORD_EST) == Coord(4, 6));
    CHECK(devantCoord(c, Direction::EST) == Coord(5, 6));
    CHECK(devantCoord(c, Direction::SUD_EST) == Coord(6, 6));
    CHECK(devantCoord(c, Direction::SUD) == Coord(6, 5));
    CHECK(devantCoord(c, Direction::SUD_OUEST) == Coord(6, 4));
    CHECK(devantCoord(c, Direction::OUEST) == Coord(5, 4));

    // Un seul test car on sait que les autres directions fonctionnent
    // correctement d'après les tests précédents
    // Coin
    CHECK_THROWS_AS(devantCoord(Coord(0, 0), Direction::NORD),
                    std::out_of_range);
  }

  SUBCASE("Test du retour au départ") {
    Coord c(5, 5);
    for (int i = 0; i < 8; i++) {
      Direction d = static_cast<Direction>(i);
      c = devantCoord(c, d);
      for (int j = 0; j < 4; j++) {
        d = aDroite(d);
      }
      c = devantCoord(c, d);
    }
    CHECK(c == Coord(5, 5));
  }
}
