#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "matrix.hpp"

static constexpr size_t L = 1000;
static constexpr size_t T = 1000;

int main() {

  mtx::matrix<uint32_t, L, L> M;

  std::fill(M.flat().begin(), M.flat().end(), 0);

  auto overloaded_it =
      [&M]() { return std::find_if(M.flat().begin(), M.flat().end(), [](uint32_t x) { return x > 3; }); };
  for (size_t t = 0; t < T; ++t) {

    fprintf(stderr, "\r %03.3f%%", float(t)*100 / T);
    M[L / 2][L / 2]++; // increment central cell

    for (uint32_t *it = overloaded_it(); it != M.flat().end(); it = overloaded_it()) {
      // while there is an overloaded, share load with neighbours
      auto[y, x] = M.index_of(it);
      M[y][x] -= 4;
      if (y)M[y - 1][x]++;
      if (y < L - 1)M[y + 1][x]++;
      if (x)M[y][x - 1]++;
      if (x < L - 1)M[y][x + 1]++;
    }

  }
  fprintf(stderr, "\r%lu epochs completed\n", T);

//  std::ofstream output("M2.ascii");
//  for (int k = 0; k < (L * L); k++) {
//    output << M[k] << " "; // behaves like cout - cout is also a stream
//    if ((k + 1) % L == 0)output << "\t" << std::endl;
//  }

  return 0;
}
