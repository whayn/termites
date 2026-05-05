#ifndef GRILLE_HPP
#define GRILLE_HPP
#include "Coord.hpp"
#include <ostream>
#include <vector>

enum class TypeCase { VIDE, BRINDILLE, NID };

struct Case {
  TypeCase type;
  int idColonie; // -1 si pas de propriétaire, sinon id de la colonie
  int idTermite; // -1 si aucune termite
  std::vector<float> pheromones; // Un tableau de phéromones, une par colonie

  Case() : type(TypeCase::VIDE), idTermite(-1) {}
};

class Grille {
private:
  std::vector<std::vector<Case>> cases;
  int taille;

public:
  Grille(int taille)
      : cases(taille, std::vector<Case>(taille)), taille(taille) {}

  void poseBrindille(
      Coord c,
      int idColonie = -1); // un appel par défaut pour les brindilles neutres
  void enleveBrindille(Coord c);
  bool contientBrindille(Coord c) const;

  void poseTermite(Coord c, int idT);
  void enleveTermite(Coord c);
  int numéroTermite(Coord c) const;

  bool contientNid(Coord c) const;
  void poseNid(Coord c, int idColonie);

  int proprietaireCase(Coord c) const;
  int getTaille() const { return taille; }

  bool estVide(Coord c) const;

  // Gestion des phéromones
  void initialiserPheromones(int nbColonies);
  void deposerPheromone(Coord c, int idColonie, float quantite);
  float getIntensitePheromone(Coord c, int idColonie) const;
  void evaporerPheromones(float taux);

  // Plus tard on réalisera une fonction afficheJeu
  std::ostream &print(std::ostream &out) const;
};

std::ostream &operator<<(std::ostream &out, const Grille &g);

#endif
