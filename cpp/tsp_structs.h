#ifndef STRUCTS_HEADER
#define STRUCTS_HEADER

#include "city.h"

#include <iostream>
#include <vector>

struct City_Data{
  std::vector<int> city_ids;
  std::vector<City> cities;
};

struct Point{
  float x_0;
  float x_1;
};

#endif
