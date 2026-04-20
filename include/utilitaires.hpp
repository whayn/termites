#ifndef UTILITAIRES_HPP
#define UTILITAIRES_HPP

#include <cstdlib> // Pour rand()
#include <utility> // Pour std::swap
#include <vector>

// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
template <typename T> void melanger(std::vector<T> &vec) {
  if (vec.empty())
    return;

  for (int i = 0; i < (int)vec.size() - 1; i++) {
    int j = i + rand() % (vec.size() - i);
    std::swap(vec[i], vec[j]);
  }
}

#endif
