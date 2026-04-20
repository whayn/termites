#include "Colonie.hpp"
#include "doctest.h"

TEST_CASE("Test de la classe Colonie") {
  Coord pos(10, 10);
  Colonie c(1, pos, 6, 0.1f, "rouge");

  SUBCASE("Test des getters") {
    CHECK(c.getId() == 1);
    CHECK(c.getPosition().getLig() == 10);
    CHECK(c.getPosition().getCol() == 10);
    CHECK(c.getDureeSablier() == 6);
    CHECK(c.getProbaTourner() == 0.1f);
    CHECK(c.getCouleur() == "rouge");
    CHECK(c.getScore() == 0);
  }

  SUBCASE("Test de la gestion du score") {
    CHECK(c.getScore() == 0);

    c.incrementerScore();
    CHECK(c.getScore() == 1);

    c.incrementerScore();
    CHECK(c.getScore() == 2);

    c.decrementerScore();
    CHECK(c.getScore() == 1);

    c.reinitialiserScore();
    CHECK(c.getScore() == 0);
  }
}
