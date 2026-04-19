#ifndef COORD_HPP
#define COORD_HPP

#include <ostream>
#include <vector>

class Coord {
private:
  int lig;
  int col;

public:
  Coord(int lig, int col);

  int getLig() const { return lig; }
  int getCol() const { return col; }

  std::ostream &print(std::ostream &out) const;

  bool operator==(const Coord &c) const { return lig == c.lig && col == c.col; }
  bool operator!=(const Coord &c) const { return !(*this == c); }
};

// Surcharge de l'opérateur de flux pour Coord
std::ostream &operator<<(std::ostream &out, const Coord &c);

// Direction

enum class Direction {
  NORD_OUEST,
  NORD,
  NORD_EST,
  EST,
  SUD_EST,
  SUD,
  SUD_OUEST,
  OUEST
};

std::ostream &operator<<(std::ostream &out, const Direction &d);

Direction aGauche(Direction d);
Direction aDroite(Direction d);

Coord devantCoord(const Coord &c, Direction d);

// EnsCoord
class EnsCoord {
private:
  std::vector<Coord> coords;

  int position(const Coord &c) const;

public:
  EnsCoord(std::vector<Coord> coords) : coords(coords) {}

  bool contient(const Coord &c) const { return position(c) != -1; }
  bool estVide() const { return coords.empty(); }
  int taille() const { return coords.size(); }
  Coord ieme(int i) const;
  Coord choixHasard() const;

  void ajoute(Coord c);
  void supprime(const Coord &c);
};

EnsCoord voisines(const Coord &c);

#endif
