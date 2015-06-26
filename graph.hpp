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


typedef std::vector<Edge> Edges;
typedef std::vector<Pair> Adjs; // (degree, id) list
typedef std::tuple<int, int, int, int> path;


struct Graph {
private:
  int N;
  bool tau_set;
  void add_edge(Edge );
  void calcProb();
public:
  vector<Adjs> adj;
  Edges edges;
  std::unordered_set<Pair, boost::hash< std::pair<int, int>>> has_edge;

  ll W;
  vector<ll> acc_tau;

  Graph(int );
  Graph() {};
  void set_n(int );
  int get_size();
  void add_edges(vector<Pair> );

  void show(string );
  path sample();
  Motif judge_induced(path );
  void sample_debug_test(int );
  void path_sampler(int );
};

#endif
