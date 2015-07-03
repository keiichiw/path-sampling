#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <tuple>
#include <set>
#include <unordered_set>
#include <cassert>
#include <random>

using namespace std;
typedef pair<int, int> Pair;
typedef long long ll;

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

struct pair_hash {
  inline std::size_t operator()(const Pair & v) const {
    return v.first*100000+v.second;
  }
};


typedef vector<Edge> Edges;
typedef vector<Pair> Adjs; // (degree, id) list
typedef tuple<int, int, int, int> path_t;
typedef Adjs::iterator EIter;
typedef discrete_distribution<int> disc_dist_t;

class Graph {

private:
  int N;
  vector<Adjs> adj;
  Edges edges;
  std::unordered_set<Pair, pair_hash> has_edge;
  ll W;
  disc_dist_t tau_dist;
  ll L;
  disc_dist_t lambda_dist;

  vector<pair<EIter, EIter>> uppers;

  void add_edge(Edge );

  Motif judge_induced(path_t );
  bool is_centered(path_t );

public:
  string name;
  mt19937 mt_engine;
  Graph(int );
  Graph() {};
  void preprocess_3path();
  void preprocess_centered();

  void set_n(int );
  void add_edges(vector<Pair> );
  void show(string );

  path_t sample();
  void path_sampler(int );

  path_t sample_centered();
  void centered_sampler(int );
};

#endif
