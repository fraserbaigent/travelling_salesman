#include <iostream>
#include <random>
#include <vector>
#include <math.h>
#include "solver.h"
#include "tsp_structs.h"

#include <algorithm>

std::mt19937 Solver::gen;
std::uniform_int_distribution<int> Solver::el_dist(1,20);
std::uniform_real_distribution<double> Solver::qa_dist(0.0,1.0);



Solver::Solver(City_Data *city_data,
	       LUTab *l_tab,
	       Kwargs *kwargs_input) {
  //assign pointers and get number of cities
  c_dat = city_data;
  kwargs = kwargs_input;
  lookup_table = l_tab;
  n_cities = (city_data->city_ids).size();
  //Load test city list
  for (std::vector<int>::iterator it = (c_dat->city_ids).begin() ;
       it != (c_dat->city_ids).end(); ++it) {
    test_list.push_back((c_dat->city_ids)[*it]);
  };
  //initialise rng dist with number of cities
  decltype(el_dist.param()) rng_cits (0, n_cities-1);
  el_dist.param(rng_cits);
  //initialise d with the highest possible journey length
  init_d();
};

Solver::~Solver() {};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//setters

void Solver::set_rng_seed(const int input){
  gen.seed(input);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//getters

double Solver::get_d() const {
  return d_opt;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//secondary functions

void Solver::init_d() {
  double d;
  d_opt = n_cities * (*lookup_table)[0][0];
  for (int i = 0 ; i < n_cities; i++){
    for (int j = i ; j < n_cities; j++) {
      d = n_cities * (*lookup_table)[i][j];
      if (d > d_opt){
	d_opt = d;
      };
    };
  };
};

void Solver::calculate_d(std::vector<int> *t_l, double *d) {
  *d = (*lookup_table)[(*t_l)[0]][t_l->back()];
  for (int i = 0; i < n_cities-1 ; i++) {
    *d += (*lookup_table)[(*t_l)[i]][(*t_l)[i+1]];
  };
};

void Solver::swap_cities(const int n_swaps) {
  for (int i=0; i < n_swaps; i++) {
    rn[0] = el_dist(gen);
    // Make sure that we get two different cities
    do {
      rn[1] = el_dist(gen);
    }
    while (rn[0] == rn[1]);
    std::swap(test_list[rn[0]], test_list[rn[1]]);
  };
};

//loops through cities to load one vector in to another.
//need to make a deep copy so that the data outside the class changes
void Solver::load_vectors(std::vector<int> *source,
			  std::vector<int> *destin){  
  for (int c_l = 0 ; c_l < n_cities; ++c_l) {
    (*destin)[c_l] = (*source)[c_l];
  };
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//primary optimiser
void Solver::optimise() {
  int i, j, k=0;
  double global_d;
  std::vector<int> internal_opt_list(test_list);
  calculate_d(&test_list, &global_d);
  do {
    i = 0;
    beta= kwargs->beta;
    std::random_shuffle(test_list.begin(), test_list.end());
    load_vectors(&test_list, &internal_opt_list);
    calculate_d(&test_list, &d_new);
    d_opt = d_new;
    do {
      j = 0;
      do {
	swap_cities(2);
	calculate_d(&test_list, &d_new);
	if (d_new <= d_opt) {
	  load_vectors(&test_list, &internal_opt_list);
	  d_opt = d_new;
	}
	else if ( log ( qa_dist(gen)) >= (- beta * (d_new - d_opt) ) ){
	  load_vectors(&internal_opt_list ,&test_list);
	};
	j++;
	;}
      while (j < kwargs->n_inner);
      beta *= kwargs->beta_x;
      ++i;
      load_vectors(&internal_opt_list ,&test_list);
      calculate_d(&test_list, &d_new);
    }
    while (i < kwargs->n_outer);
    k++;
    calculate_d(&internal_opt_list, &d_opt);
    if (d_opt < global_d){
      load_vectors(&internal_opt_list , &(c_dat->city_ids));
      calculate_d(&internal_opt_list, &global_d);
      //std::cout<< "New best d: "<< global_d << "\n";
    };
  }
  while (k < kwargs->n_runs);
  d_opt = global_d;
};
