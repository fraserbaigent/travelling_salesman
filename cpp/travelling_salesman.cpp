#include <iostream>
#include <array>
#include <vector>
#include <fstream>
#include <cmath>

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
    c_d.cities[i].set_id(i);
  };
  return c_d;
};

float get_city_dr(std::array<City,2> cities) {
  float dr;
  float x_i[2], x_j[2];
  std::array<float,2>  p;
  for (int8_t i = 0;i < 2; i++){
    p = cities[i].get_x();
    x_i[i] = p[0];
    x_j[i] = p[1];
  }
  dr = std::hypot((x_i[0] - x_i[1]), (x_j[0]-x_j[1]));
  return dr;
};

std::vector<std::vector<float>> gen_lookup_table(City_Data* city_data) {
  std::vector<std::vector<float>> lookup_table ;
  int  n_cities = city_data->city_ids.size();
  lookup_table.assign(n_cities, std::vector<float>());
  for ( int8_t i = 0; i < n_cities; i++) {
    lookup_table[i].assign(n_cities, float());
  };
  
  float dr;
  std::array<City,2> c;
  
  for ( int8_t i = 0; i < n_cities; i++) {
    lookup_table[i][i] = 0;
    c[0] = (*city_data).cities[i];
    for (int8_t j = i+1; j < n_cities; j++) {
      c[1] = (*city_data).cities[j];
      dr = get_city_dr(c);
      lookup_table[i][j] = dr;
      lookup_table[j][i] = dr;
    };
  };
  return lookup_table;
};

int main(){
  City_Data c_d = init_cities("cities.txt");
  int8_t n_cities;
  n_cities = c_d.city_ids.size();
  //for (int i = 0 ; (i < n_cities) ; ++i) {
  //  std::cout << c_d.city_ids[i] << " " << c_d.cities[i].get_id() << " " << c_d.cities[i].get_x()[0] <<
  //    " " << c_d.cities[i].get_x()[1] <<std::endl;
  //}
  std::vector<std::vector<float>> l_t;
  City_Data *c_2;
  c_2 = new City_Data(c_d);
  l_t = gen_lookup_table(c_2);
  for (int8_t i = 0 ; i < n_cities; i++){
    for (int8_t j = 0 ; j < n_cities; j++){
      std::cout << l_t[i][j]<< " ";
    };
    std::cout<<"\n";
  };
  
  delete c_2;
  return 0;
}
