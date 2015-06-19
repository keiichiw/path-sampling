#include <random>
#include <cassert>
#include "util.hpp"
using namespace std;

double rand_r()
{
  static mt19937 rng;
  static uniform_real_distribution<double> dist(0, 1.0);
  return dist(rng);
}

int rand_n(int n)
{
  assert(n > 1);
  static mt19937 rng;
  uniform_int_distribution<int> dist(0, n-1);
  return dist(rng);
}
