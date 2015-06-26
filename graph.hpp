#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <tuple>
#include <set>
#include <cassert>
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
typedef std::tuple<int, int, int> path3;


struct Graph {
private:
  int N;
  bool tau_set;
  void add_edge(Edge );
  void calcProb();
public:
  vector<Edges> adj;
  Edges edges;
  std::set<Pair> has_edge;

  int W;
  vector<int> acc_tau;

  Graph(int );
  int get_size();
  void add_edges(vector<Pair> );

  void show(string );
  path3 sample();
  Motif judge_induced(path3 );
  void sample_debug_test(int );
  void path_sampler(int );
};

#endif
