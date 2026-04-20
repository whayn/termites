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

#endif
