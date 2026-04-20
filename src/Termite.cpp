#include "Termite.hpp"
#include "Coord.hpp"
#include "parametres.hpp"
#include <cstdlib>
#include <doctest.h>
#include <stdexcept>

Termite::Termite(int id, int idColonie, Coord position, int dureeSablier,
                 float probaTourner)
    : id(id), idColonie(idColonie), position(position),
      cap(static_cast<Direction>(rand() % 8)), avecBrindille(false), sablier(0),
      tourneSurPlace(false), dureeSablier(dureeSablier),
      probaTourner(probaTourner) {};

Direction Termite::directionTermite() const { return cap; };

int Termite::getSablier() const { return sablier; }

std::string Termite::toString() const {
  switch (cap) {
  case Direction::NORD_OUEST:
    return "\\";
  case Direction::NORD:
    return "|";
  case Direction::NORD_EST:
    return "/";
  case Direction::EST:
    return "-";
  case Direction::SUD_EST:
    return "\\";
  case Direction::SUD:
    return "|";
  case Direction::SUD_OUEST:
    return "/";
  case Direction::OUEST:
    return "-";
  }
  return "|";
}

Coord Termite::devant() const { return devantCoord(position, cap); }

bool Termite::porteBrindille() const { return avecBrindille; }

void Termite::tourneADroite() { cap = aDroite(cap); }
void Termite::tourneAGauche() { cap = aGauche(cap); }
void Termite::tourneAleat() {
  cap = rand() % 2 == 0 ? aDroite(cap) : aGauche(cap);
}

bool Termite::laVoieEstLibre(const Grille &grille) const {
  try {
    return grille.estVide(devant());
  } catch (std::out_of_range) {
    return false;
  }
}

bool Termite::brindilleEnFace(const Grille &grille) const {
  try {
    return grille.contientBrindille(devant());
  } catch (std::out_of_range) {
    return false;
  }
}

int Termite::voisinsLibre(const Grille &grille) const {
  int voisins = 0;
  for (int i = std::max(position.getLig() - 1, 0);
       i <= std::min(position.getLig() + 1, TAILLE_GRILLE - 1); i++) {
    for (int j = std::max(position.getCol() - 1, 0);
         j <= std::min(position.getCol() + 1, TAILLE_GRILLE - 1); j++) {
      if (i != position.getLig() || j != position.getCol()) {
        if (grille.estVide(Coord(i, j))) {
          voisins++;
        }
      }
    }
  }
  return voisins;
}

void Termite::avance(Grille &grille) {
  if (!laVoieEstLibre(grille))
    return;

  grille.enleveTermite(position);
  position = devant();
  grille.poseTermite(position, id);
}

void Termite::marcheAleatoire(Grille &grille) {
  if (!laVoieEstLibre(grille)) {
    tourneAleat();
  } else if (rand() % 100 < (int)(getProbaTourner() * 100)) {
    tourneAleat();
  }
  avance(grille);
}

void Termite::chargeBrindille(Grille &grille) {
  if (!brindilleEnFace(grille) || avecBrindille || sablier != 0)
    return;

  grille.enleveBrindille(devant());
  avecBrindille = true;
  sablier = getDureeSablier();
}

void Termite::dechargeBrindille(Grille &grille) {
  if (!laVoieEstLibre(grille) || !avecBrindille || sablier != 0)
    return;

  grille.poseBrindille(devant());
  avecBrindille = false;
  sablier = getDureeSablier();
}

void Termite::vieTermite(Grille &grille) {
  if (sablier > 0)
    sablier--;
  if (!avecBrindille) {
    if (brindilleEnFace(grille) && sablier == 0) {
      chargeBrindille(grille);
    } else {
      marcheAleatoire(grille);
    }
  } else if (avecBrindille) {
    if (!tourneSurPlace) {
      if (brindilleEnFace(grille) && sablier == 0 && voisinsLibre(grille) > 1) {
        tourneSurPlace = true;
        tourneADroite();
      } else {
        marcheAleatoire(grille);
      }
    } else if (tourneSurPlace) {
      if (laVoieEstLibre(grille)) {
        dechargeBrindille(grille);
        tourneSurPlace = false;
      } else {
        tourneADroite();
      }
    }
  }
}

TEST_CASE("Test de la classe Termite") {
  Grille g(10);
  Termite t(1, 0, Coord(5, 5));
  g.poseTermite(Coord(5, 5), 1);

  Coord cible = t.devant();

  SUBCASE("Constructeur") { CHECK(t.porteBrindille() == false); }

  SUBCASE("Rotations") {
    Direction dirInitiale = t.directionTermite();

    t.tourneADroite();
    CHECK(t.directionTermite() != dirInitiale);
    t.tourneAGauche();
    CHECK(t.directionTermite() == dirInitiale);
  }

  SUBCASE("Voie Libre par défaut") {
    CHECK(t.laVoieEstLibre(g) == true);
    CHECK(t.brindilleEnFace(g) == false);
  }

  SUBCASE("Obstacles") {
    Coord devant = t.devant();

    g.poseBrindille(devant);
    CHECK_FALSE(t.laVoieEstLibre(g));
    CHECK(t.brindilleEnFace(g));

    g.enleveBrindille(devant);
    g.poseTermite(devant, 2);
    CHECK_FALSE(t.laVoieEstLibre(g));
    CHECK_FALSE(t.brindilleEnFace(g));
  }

  SUBCASE("Chargement/Déchargement brindilles") {
    g.poseBrindille(cible);
    CHECK(g.contientBrindille(cible) == true);

    t.avance(g);
    Coord nouvelleCible = t.devant();

    t.dechargeBrindille(g);

    CHECK(t.porteBrindille() == false);
    CHECK(g.contientBrindille(nouvelleCible) == true);
  }

  SUBCASE("Avancer") {
    Coord depart = Coord(5, 5);

    t.avance(g);
    CHECK((t.devant() == depart) == false);
    CHECK(g.estVide(depart) == true);
    CHECK(g.estVide(cible) == false);
  }

  SUBCASE("Sablier") {
    Coord devant = t.devant();
    g.poseBrindille(devant);

    CHECK(t.getSablier() == 0);

    t.chargeBrindille(g);
    CHECK(t.porteBrindille() == true);
    CHECK(t.getSablier() == t.getDureeSablier());

    t.dechargeBrindille(g);
    CHECK(t.porteBrindille() == true);
    CHECK(g.estVide(devant) == true);

    for (int i = 0; i < t.getDureeSablier(); i++) {
      CHECK(t.getSablier() == t.getDureeSablier() - i);
      t.vieTermite(g);
    }
    CHECK(t.getSablier() == 0);

    t.dechargeBrindille(g);
    CHECK(t.porteBrindille() == false);
    CHECK(g.contientBrindille(devant) == true);
    CHECK(t.getSablier() == t.getDureeSablier());
  }

  SUBCASE("Voisins Libres") {
    CHECK(t.voisinsLibre(g) == 8);

    Coord nw(4, 4);
    g.poseBrindille(nw);
    CHECK(t.voisinsLibre(g) == 7);
    g.enleveBrindille(nw);

    Coord n(4, 5);
    g.poseTermite(n, 2);
    CHECK(t.voisinsLibre(g) == 7);
    g.enleveTermite(n);

    Coord voisinsAll[8] = {Coord(4, 4), Coord(4, 5), Coord(4, 6), Coord(5, 4),
                           Coord(5, 6), Coord(6, 4), Coord(6, 5), Coord(6, 6)};

    for (int k = 0; k < 8; ++k) {
      if (k % 2 == 0)
        g.poseBrindille(voisinsAll[k]);
      else
        g.poseTermite(voisinsAll[k], 10 + k);
    }
    CHECK(t.voisinsLibre(g) == 0);

    for (int k = 0; k < 8; ++k) {
      if (k % 2 == 0)
        g.enleveBrindille(voisinsAll[k]);
      else
        g.enleveTermite(voisinsAll[k]);
    }

    Termite tc(3, 0, Coord(0, 0));
    g.poseTermite(Coord(0, 0), 3);
    CHECK(tc.voisinsLibre(g) == 3);

    Coord right(0, 1);
    g.poseBrindille(right);
    CHECK(tc.voisinsLibre(g) == 2);
    g.enleveBrindille(right);
    g.enleveTermite(Coord(0, 0));

    Termite te(4, 0, Coord(0, 5));
    g.poseTermite(Coord(0, 5), 4);
    CHECK(te.voisinsLibre(g) == 5);
    g.enleveTermite(Coord(0, 5));
  }
}
