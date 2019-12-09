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
  c_dat = new City_Data(*city_data);
  kwargs = new Kwargs(*kwargs_input);
  lookup_table = new LUTab(*l_tab);
  n_cities = (*city_data).city_ids.size();

  //initialise rng dist with number of cities
  decltype(el_dist.param()) rng_cits (0, n_cities);
  el_dist.param(rng_cits);
  //initialise d with the highest possible journey length
  double d_test;
  d_test = n_cities * (*lookup_table)[0][0];
  d_opt = d_test;
  for (int i = 0 ; i < n_cities; i++){
    test_list.push_back((*c_dat).city_ids[i]);
    for (int j = i ; j < n_cities; j++) {
      d_test = n_cities * (*lookup_table)[i][j];
      if (d_test > d_opt){
	d_opt = d_test;
      };
    };
  };
};

Solver::~Solver() {
  // Delete the pointers
  delete c_dat;
  delete kwargs;
  delete lookup_table;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//setters

void Solver::set_rng_seed(const int input){
  gen.seed(input);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//secondary functions

void Solver::calculate_d() {
  d_new = (*lookup_table)[test_list[0]][test_list.back()];
  for (int i = 0; i < n_cities-1 ; i++) {
    d_new += (*lookup_table)[test_list[i]][test_list[i+1]];
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
    //test_e[0] = test_list[rn[0]];
    //test_e[1] = test_list[rn[1]];
    //test_list[rn[1]] = test_e[0];
    //test_list[rn[0]] = test_e[1];
    if (test_list[rn[0]] == test_list[rn[1]]){
      std::cout<<"They are the same before\n";
      std::cout<< rn[0] << " "<<rn[1]<< " "<< test_list[rn[0]] <<" "<< test_list[rn[1]] << "\n";};
    std::swap(test_list[rn[0]], test_list[rn[1]]);
    if (test_list[rn[0]] == test_list[rn[1]]){
      std::cout<<"They are the same after\n";
      exit(1);};
  };
};

//loops through cities to load one vector in to another.
//need to make a deep copy so that the data outside the class changes
void Solver::load_vectors(std::vector<int> *source,
			  std::vector<int> *destin){  
  for (int c_l = 0 ; c_l < n_cities; ++c_l) {
    std::cout<< c_l<<" " << (*destin)[c_l] << " "<< (*source)[c_l] <<"\n";
    (*destin)[c_l] = (*source)[c_l];
  };
  std::cout<<"\n";
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//primary optimiser
void Solver::optimise() {
  int i=0,j=0;
  beta= kwargs->beta;
  do {
    do {
      swap_cities(1);
      calculate_d();
      std::cout << i << " " << j << "\n";
      if (d_new <= d_opt) {
	//load_vectors(&test_list, &((*c_dat).city_ids));
	d_opt = d_new;
      }
      else if ( log ( qa_dist(gen)) <= (- beta * (d_new - d_opt) ) ){
      	d_opt = d_new;
      };
      //else{
      	//load_vectors(&((*c_dat).city_ids) ,&test_list);
	
      //};
      ++j;}
    while (j < kwargs->n_inner);
    beta *= kwargs -> beta_x;
    ++i;
    load_vectors(&((*c_dat).city_ids) ,&test_list);
    calculate_d();
    d_opt = d_new;
  }
  while (i < kwargs->n_outer);
}; 
