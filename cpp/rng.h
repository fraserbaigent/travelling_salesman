#ifndef RNG_SEEDER
#define RNG_SEEDER

unsigned long long rdtsc(){
  unsigned int lo,hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return ((unsigned long long)hi << 32) | lo;
}

#endif
