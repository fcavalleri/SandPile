#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "matrix.hpp"
#include "udcs.h"

static constexpr size_t L = 1000;
static constexpr size_t T = 10000000;

int main() {

  mtx::matrix<uint32_t, L, L> M;
  mtx::udcs<L * L, uint32_t> overloaded;

  std::fill(M.flat().begin(), M.flat().end(), 0);

  for (size_t t = 0; t < T; ++t) {

    //print the progress bar
    {
      static constexpr size_t print_every = 1000;
      static size_t ip = 0;
      ++ip;
      if (ip >= print_every) {
        fprintf(stderr, "\r %03.3f%%", float(t+1) * 100 / T);
        ip = 0;
      }
    }

    M[L / 2][L / 2]++; // increment central cell
    if(M[L/2][L/2]>3)overloaded.insert((L/2)*L + L/2);

    while (!overloaded.empty()) {
      size_t yx = *overloaded.begin();
      // while there is an overloaded, share load with neighbours
      auto[y, x] = M.index_of(&M.flat()[yx]);
      M[y][x] -= 4;
      if (M.flat()[yx] < 4)overloaded.erase(yx);
      if (y) {
        M[y - 1][x]++;
        if (M.flat()[yx - L] > 3)overloaded.insert(yx - L);
      }
      if (y < L - 1) {
        M[y + 1][x]++;
        if (M.flat()[yx + L] > 3)overloaded.insert(yx + L);
      }
      if (x) {
        M[y][x - 1]++;
        if (M.flat()[yx - 1] > 3)overloaded.insert(yx - 1);
      }
      if (x < L - 1) {
        M[y][x + 1]++;
        if (M.flat()[yx + 1] > 3)overloaded.insert(yx + 1);
      }
    }

  }
  fprintf(stderr, "\r%lu epochs completed\n", T);

//  std::ofstream output("M2.ascii");
//  for (int k = 0; k < (L * L); k++) {
//    output << M[k] << " "; // behaves like cout - cout is also a stream
//    if ((k + 1) % L == 0)output << "\t" << std::endl;
//  }
  for (size_t y = 0; y < L; ++y) {
    for (size_t x = 0; x < L; ++x) {
      printf("%d ", M[y][x]);
    }
    printf("\t\n");
  }

  return 0;
}
