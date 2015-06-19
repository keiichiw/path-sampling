#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <tuple>
#include <cassert>
using namespace std;

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

  void calcProb();
public:
  vector<Edges> adj;
  Edges edges;

  int W;
  vector<int> acc_tau;

  Graph(int );
  int get_size();
  void add_edge(int , int );

  void show(string );
  path3 sample();
  void sample_debug_test(int );
};

#endif
