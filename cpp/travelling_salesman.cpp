#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <cmath>
//#include <chrono>
#include "tsp_structs.h"
#include "city.h"
#include "solver.h"


std::vector<Point> cit_list(std::string filename) {
  std::vector<Point> x;
  std::ifstream infile(filename);
  float x_0, x_1;
  while (infile >> x_0 >> x_1) {
    x.push_back(Point{x_0,x_1});
  };
  return x;
};

City_Data init_cities(std::string filename) {
  std::vector<Point> x = cit_list(filename);
  int n_cities = x.size();
  City_Data c_d;
  c_d.cities.assign(n_cities, City());
  for (int i = 0 ; (i < n_cities) ; ++i) {
    c_d.city_ids.push_back(i);
    c_d.cities[i].set_x(x[i].x_0, x[i].x_1);
    c_d.cities[i].set_id(i);
  };
  return c_d;
};

float get_city_dr(std::array<City,2> cities) {
  float dr;
  float x_i[2], x_j[2];
  std::array<float,2>  p;
  for (int i = 0;i < 2; i++){
    p = cities[i].get_x();
    x_i[i] = p[0];
    x_j[i] = p[1];
  }
  dr = std::hypot((x_i[0] - x_i[1]), (x_j[0]-x_j[1]));
  return dr;
};

LUTab gen_lookup_table(City_Data* city_data) {
  LUTab lookup_table ;
  int  n_cities = city_data->city_ids.size();
  lookup_table.assign(n_cities, std::vector<float>());
  for ( int i = 0; i < n_cities; i++) {
    lookup_table[i].assign(n_cities, float());
  };
  
  float dr;
  std::array<City,2> c;
  
  for ( int i = 0; i < n_cities; i++) {
    lookup_table[i][i] = 0;
    c[0] = (*city_data).cities[i];
    for (int j = i+1; j < n_cities; j++) {
      c[1] = (*city_data).cities[j];
      dr = get_city_dr(c);
      lookup_table[i][j] = dr;
      lookup_table[j][i] = dr;
    };
  };
  return lookup_table;
};

int main(){
  // Load cities and make structure
  City_Data c_d = init_cities("cities.txt");
  // Generate lookup table 
  LUTab l_t;
  l_t = gen_lookup_table(&c_d);
  // Run optimiser for TSP
  Kwargs kwargs;
  kwargs.beta = 1.0;
  kwargs.beta_x = 1.5;
  kwargs.n_outer = 1;
  kwargs.n_inner = 10000;
  Solver solver(&c_d, &l_t, &kwargs);
  solver.optimise();
  
  //for (int i = 0 ; i < n_cities; i++){
  //  for (int j = 0 ; j < n_cities; j++){
  //    std::cout << l_t[i][j]<< " ";
  //  };
  //  std::cout<<"\n";
  //};
  return 0;
}
