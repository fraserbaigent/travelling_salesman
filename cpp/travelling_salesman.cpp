#include <iostream>
#include <array>
#include <vector>
#include <fstream>

#include "city.h"
#include "tsp_structs.h"

std::vector<Point> city_list(std::string filename) {
  std::vector<Point> x;
  std::ifstream infile(filename);
  float x_0, x_1;
  while (infile >> x_0 >> x_1) {
    x.push_back(Point{x_0,x_1});
  }
  return x;
};

City_Data init_cities(std::string filename) {
  std::vector<Point> x = city_list(filename);
  int n_cities = x.size();
  City_Data c_d;
  c_d.cities.assign(n_cities, City());
  for (int i = 0 ; (i < n_cities) ; ++i) {
    c_d.city_ids.push_back(i);
    c_d.cities[i].set_x(x[i].x_0, x[i].x_1);
  };
  return c_d;
};


int main(){
  City_Data c_d = init_cities("cities.txt");
  std::cout <<c_d.city_ids[0] << " " << c_d.cities[0].get_x()[0]<<std::endl;
  City a;
  std::array<float,2> p = a.get_x();
  std::cout << p[0] << " " << p[1] << std::endl;
  a.set_x(0.1,1.0);
  p = a.get_x();
  std::cout << p[0] << " " << p[1] << std::endl;
  return 0;
}
