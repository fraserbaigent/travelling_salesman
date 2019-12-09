#ifndef CLASS_SOLVER
#define CLASS_SOLVER

#include <iostream>
#include <random>
#include <vector>
#include "tsp_structs.h"

class Solver {
 public:
  //constructors
  Solver(City_Data*, LUTab*, Kwargs*);
  //destructors
  ~Solver();
  //functions
  void optimise();
  void swap_cities(const int);
  void load_vectors(std::vector<int>* , std::vector<int>*);
  void calculate_d();
  //setters
  void set_rng_seed(const int);
 private:
  static std::mt19937 gen;
  static std::uniform_int_distribution<int> el_dist;
  static std::uniform_real_distribution<double> qa_dist;
  // Kwargs class from tsp_structs.h, includes sim input data
  Kwargs *kwargs;
  // City_Data class from tsp_structs.h, includes answer city list and id list
  City_Data *c_dat;
  // Lookup table with calculated distances between cities (i,j)
  std::vector<std::vector<float>> *lookup_table;
  //values
  std::vector<int> test_list;
  double d_opt, d_new, beta;
  int n_cities, rn[2], max_swaps = 1, test_e[2];
  std::vector<int> placeholder;
};


#endif
