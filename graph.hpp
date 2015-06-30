#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <tuple>
#include <set>
#include <unordered_set>
#include <cassert>
#include <boost/functional/hash.hpp>
#include "util.hpp"
using namespace std;

enum Motif {
  Triangle = -1,
  Star = 0, Path, Tailed,
  Cycle, Chordal, Clique
};

struct Edge {
  int id;
  int u, v;
  Edge(int id, int u, int v);
  Edge(){}
  void show();
};


typedef vector<Edge> Edges;
typedef vector<Pair> Adjs; // (degree, id) list
typedef tuple<int, int, int, int> path;
typedef Adjs::iterator EIter;

struct Graph {
private:
  int N;
  void add_edge(Edge );
  void preprocess();
public:
  vector<Adjs> adj;
  Edges edges;
  std::unordered_set<Pair, boost::hash< std::pair<int, int>>> has_edge;

  ll W;
  vector<ll> acc_tau;
  ll L;
  vector<ll> acc_lambda;

  /*
    edges[i] = (u, v), uppers[i] = (p1, p2)ならば、
    *p1 はadj[u]の要素のうち、v < *p1を満たす最小のもの
    *p2 はadj[v]の要素のうち、u < *p2を満たす最小のもの
   */
  vector<pair<EIter, EIter>> uppers;

  Graph(int );
  Graph() {};
  void set_n(int );
  int get_size();
  void add_edges(vector<Pair> );

  void show(string );
  Motif judge_induced(path );
  void sample_debug_test(int );

  path sample();
  void path_sampler(int );

  path sample_centered();
  bool is_centered(path );
  void centered_sampler(int );
};

#endif
