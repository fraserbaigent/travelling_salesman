#ifndef CLASS_CITY
#define CLASS_CITY

#include <iostream>
#include <array>

class City{
 private:
  int c_id;
  std::array<float, 2> x;
 public:
  City();
  ~City();

  void set_x(const float, const float);
  void set_id(const int);
  
  int get_id();
  std::array<float, 2> get_x();
};

#endif
