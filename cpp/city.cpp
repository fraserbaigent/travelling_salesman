#include <iostream>
#include "city.h"
#include <array>




City::City() {};
City::~City() {};
//Getters
std::array<float, 2> City::get_x() {
  return x;
};

int City::get_id() {
  return c_id;
};
//Setters
void City::set_id(const int input) {
  c_id = input;
};

void City::set_x(const float x_0, const float x_1) {
  x = {x_0, x_1};
};
