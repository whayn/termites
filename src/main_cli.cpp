#include "Jeu.hpp"
#include "parametres.hpp"
#include <string>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <doctest.h>
#include <iostream>

int main() {
  srand(time(NULL));

  int nbTermites = 5;
  Jeu jeu(AppConfig(20, 0.08f, 2,
                    std::vector<ColonieConfig>{
                        ColonieConfig{nbTermites, 6, 0.1f},
                        ColonieConfig{nbTermites, 6, 0.1f},
                    }));
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
      jeu.etapeSuivante(LaboConfig());
    }
  }

  return 0;
}
