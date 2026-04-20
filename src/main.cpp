#include "Jeu.hpp"
#include <string>
#define DOCTEST_CONFIG_IMPLEMENT
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <doctest.h>
#include <iostream>

int main() {
  srand(time(NULL));

  int nbTermites = 20;
  float densiteBrindilles = 0.05;
  Jeu jeu(nbTermites, densiteBrindilles);
  int nbPasse = 1;
  std::string saisie;

  while (true) {
    system("clear");

    std::cout << jeu;
    std::cout << "Vitesse actuelle (itérations par affichage) : " << nbPasse
              << "\n";
    std::cout << "Commandes : [*] Accélérer [/] Ralentir [Entrée] Continuer "
                 "[q] Quitter\n";

    std::getline(std::cin, saisie);

    if (saisie == "q")
      break;
    if (saisie == "*")
      nbPasse *= 2;
    if (saisie == "/")
      nbPasse = std::max(1, nbPasse / 2);

    for (int i = 0; i < nbPasse; i++) {
      jeu.etapeSuivante();
      system("clear");
      std::cout << jeu;
    }
  }

  return 0;
}
