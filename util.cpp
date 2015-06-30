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

ll rand_n(ll n)
{
  assert(n > 0LL);
  static mt19937 rng;
  uniform_int_distribution<ll> dist(0, n-1);
  return dist(rng);
}
