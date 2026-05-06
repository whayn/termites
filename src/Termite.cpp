#include "Termite.hpp"
#include "Coord.hpp"
#include "parametres.hpp"

#include <cstdlib>
#include <doctest.h>
#include <stdexcept>

Termite::Termite(int id, int idColonie, Coord position, Coord posNid,
                 int dureeSablier, float probaTourner)
    : id(id), idColonie(idColonie), position(position), posNid(posNid),
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

bool Termite::nidEnFace(const Grille &grille) const {
  try {
    return grille.contientNid(devant());
  } catch (std::out_of_range) {
    return false;
  }
}

int Termite::voisinsLibre(const Grille &grille) const {
  int voisins = 0;
  for (int i = std::max(position.getLig() - 1, 0);
       i <= std::min(position.getLig() + 1, grille.getTaille() - 1); i++) {
    for (int j = std::max(position.getCol() - 1, 0);
         j <= std::min(position.getCol() + 1, grille.getTaille() - 1); j++) {
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

  grille.poseBrindille(devant(), idColonie);
  avecBrindille = false;
  sablier = getDureeSablier();
}

void Termite::vieTermite(Grille &grille, const LaboConfig &laboConfig) {
  if (sablier > 0)
    sablier--;

  // Machines à états
  if (!avecBrindille) {
    // Recherche de brindille
    if (brindilleEnFace(grille) && sablier == 0 &&
        grille.proprietaireCase(devant()) != idColonie) {
      chargeBrindille(grille);
    } else {
      // Gestion des phéromones
      Direction dirF = cap;
      Direction dirG = aGauche(cap);
      Direction dirD = aDroite(cap);

      // Captation des odeurs
      float posF = 0, posG = 0, posD;
      // Pas de phéromones à proximité du nid pour éviter les bloquages
      if (position.distance(posNid) > laboConfig.rayonAntiBouchon) {
        try {
          if (grille.estVide(devantCoord(position, dirF)))
            posF = grille.getIntensitePheromone(devantCoord(position, dirF),
                                                idColonie);
        } catch (...) {
        }
        try {
          if (grille.estVide(devantCoord(position, dirG)))
            posG = grille.getIntensitePheromone(devantCoord(position, dirG),
                                                idColonie);
        } catch (...) {
        }
        try {
          if (grille.estVide(devantCoord(position, dirD)))
            posD = grille.getIntensitePheromone(devantCoord(position, dirD),
                                                idColonie);
        } catch (...) {
        }
      }

      // Décision de la direction à prendre
      if (posF > 0 || posG > 0 || posD > 0) {
        float total = posF + posG + posD;
        float r = (float)rand() / (float)RAND_MAX * total;

        if (r < posF) {
          // Reste dans la même direction
        } else if (r < posF + posG) {
          tourneAGauche();
        } else {
          tourneADroite();
        }
        avance(grille);
      } else {
        marcheAleatoire(grille);
      }
    }
  } else {
    // Retour au nid

    // On pose des phéromones à chaque pas lorsque le termite porte une
    // brindille
    grille.deposerPheromone(position, idColonie, laboConfig.depotPheromone);

    if (!tourneSurPlace) {
      if (((brindilleEnFace(grille) || nidEnFace(grille)) &&
           grille.proprietaireCase(devant()) == idColonie) &&
          sablier == 0 && voisinsLibre(grille) > laboConfig.compaciteNid) {
        tourneSurPlace = true;
        tourneADroite();
      } else {
        float distActuelle = position.distance(posNid);

        // On évalue les distances au nid dans les trois directions possibles
        Direction meilleurCap = cap;
        float meilleureDist = distActuelle;

        Direction caps[3] = {cap, aGauche(cap), aDroite(cap)};
        for (Direction c : caps) {
          try {
            Coord cible = devantCoord(position, c);
            if (grille.estVide(cible)) {
              float dist = cible.distance(posNid);
              if (dist < meilleureDist) {
                meilleureDist = dist;
                meilleurCap = c;
              }
            }
          } catch (...) {
          }
        }

        if (meilleureDist < distActuelle) {
          cap = meilleurCap;
          if (rand() % 100 < laboConfig.bruitGPS)
            tourneAleat();
          avance(grille);
        } else {
          marcheAleatoire(grille);
        }
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
  g.initialiserPheromones(1);
  Termite t(1, 0, Coord(5, 5), Coord(0, 0));
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
      t.vieTermite(g, LaboConfig());
    }
    CHECK(t.getSablier() == 0);

    // On s'assure que la voie est libre
    while (!t.laVoieEstLibre(g)) {
      t.tourneADroite();
    }

    t.dechargeBrindille(g);
    CHECK(t.porteBrindille() == false);
    CHECK(g.contientBrindille(t.devant()) == true);
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

    Termite tc(3, 0, Coord(0, 0), Coord(0, 0));
    g.poseTermite(Coord(0, 0), 3);
    CHECK(tc.voisinsLibre(g) == 3);

    Coord right(0, 1);
    g.poseBrindille(right);
    CHECK(tc.voisinsLibre(g) == 2);
    g.enleveBrindille(right);
    g.enleveTermite(Coord(0, 0));

    Termite te(4, 0, Coord(0, 5), Coord(0, 0));
    g.poseTermite(Coord(0, 5), 4);
    CHECK(te.voisinsLibre(g) == 5);
    g.enleveTermite(Coord(0, 5));
  }
}
