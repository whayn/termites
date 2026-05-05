#ifndef UTILITAIRES_HPP
#define UTILITAIRES_HPP

#include "raylib.h"
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

inline Color LerpColor(Color c1, Color c2, float t) {
  return {(unsigned char)(c1.r + (c2.r - c1.r) * t),
          (unsigned char)(c1.g + (c2.g - c1.g) * t),
          (unsigned char)(c1.b + (c2.b - c1.b) * t),
          (unsigned char)(c1.a + (c2.a - c1.a) * t)};
}

#endif
