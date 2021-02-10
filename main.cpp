#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "matrix.hpp"
#include "udcs.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

static constexpr size_t L = 512;
static constexpr size_t T = 1000000;

void display_grid(const mtx::matrix<uint8_t, L, L> &M, const std::array<sf::Color, 4> &color_map) {
  sf::RenderWindow window(sf::VideoMode(L, L, 32), "Grid");

  sf::Image image;
  image.create(L, L, sf::Color::Black);
  for (int y = 0; y < L; ++y)
    for (int x = 0; x < L; ++x)
      image.setPixel(x, y, color_map[M[y][x]]);
  //save to file
  {
    std::string filename("output_L");
    filename.append(std::to_string(L)).append("_T")
        .append(std::to_string(T)).append(".png");

    image.saveToFile(filename);
    fprintf(stderr, "   saved in image: %s\n", filename.c_str());
  }
  sf::Sprite sprite;
  sf::Texture texture;
  texture.loadFromImage(image);
  sprite.setTexture(texture);
  while (window.isOpen()) {
    for (sf::Event event; window.pollEvent(event);) {
      switch (event.type) {
        case sf::Event::Closed: {
          window.close();
          break;
        }
        default:;
      }
    }

    window.draw(sprite);
    window.display();
    sf::sleep(sf::milliseconds(10));
  }
}

int main() {

  mtx::matrix<uint8_t, L, L> M; // keeps the content of every cell
  mtx::udcs<L * L, uint32_t> overloaded; // keeps the set of cell where content > 3

  std::fill(M.flat().begin(), M.flat().end(), 0);
  size_t traveled_distance = 0;
  for (size_t t = 0; t < T; ++t) {

    //print the progress bar
    {
      static constexpr size_t print_every = 100;
      static size_t ip = 0;
      ++ip;
      if (ip >= print_every) {
        fprintf(stderr, "\r %03.3f%%", float(t + 1) * 100 / T);
        ip = 0;
      }
    }

    M[L / 2][L / 2]++; // increment central cell
    if (M[L / 2][L / 2] > 3)overloaded.insert((L / 2) * L + L / 2);

    while (!overloaded.empty()) {
      size_t yx = *overloaded.begin();
      // while there is an overloaded, share load with neighbours
      auto[y, x] = M.index_of(&M.flat()[yx]);
      M[y][x] -= 4;
      if (M.flat()[yx] < 4)overloaded.erase(yx);
      if (y) {
        M[y - 1][x]++;
        ++traveled_distance;
        if (M.flat()[yx - L] > 3)overloaded.insert(yx - L);
      }
      if (y < L - 1) {
        M[y + 1][x]++;
        ++traveled_distance;
        if (M.flat()[yx + L] > 3)overloaded.insert(yx + L);
      }
      if (x) {
        M[y][x - 1]++;
        ++traveled_distance;
        if (M.flat()[yx - 1] > 3)overloaded.insert(yx - 1);
      }
      if (x < L - 1) {
        M[y][x + 1]++;
        ++traveled_distance;
        if (M.flat()[yx + 1] > 3)overloaded.insert(yx + 1);
      }
    }

  }

  //print stats
  fprintf(stderr, "\r%lu epochs completed\n", T);
  fprintf(stderr, "  particles added: %lu\n", T);
  fprintf(stderr, "distance traveled: %lu\n", traveled_distance);
  fprintf(stderr, "   lost particles: %lu\n", T - std::accumulate(M.flat().begin(), M.flat().end(), size_t(0)));

  //save to file
  {
    std::string filename("output_L");
    filename.append(std::to_string(L)).append("_T")
        .append(std::to_string(T)).append(".txt");
    std::ofstream output(filename);
    for (size_t y = 0; y < L; ++y) {
      for (size_t x = 0; x < L; ++x) {
        output << M[y][x] << " ";
      }
      output << "\t\n";
    }
    fprintf(stderr, "    saved in file: %s\n", filename.c_str());
  }

  //display grid
  display_grid(M, {sf::Color::Black, sf::Color::Magenta, sf::Color::Red, sf::Color::Blue});

  return 0;
}
