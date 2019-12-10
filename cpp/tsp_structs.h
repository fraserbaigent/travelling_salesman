#ifndef STRUCTS_HEADER
#define STRUCTS_HEADER

#include "city.h"
#include <iostream>
#include <vector>

typedef std::vector<std::vector<float>> LUTab;

struct City_Data{
  std::vector<int> city_ids;
  std::vector<City> cities;
};

struct Point{
  float x_0;
  float x_1;
};

struct Kwargs{
  double beta;
  double beta_x;
  int n_outer;
  int n_inner;
  int n_runs;
};
#endif
