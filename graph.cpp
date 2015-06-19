#include <cstdio>
#include <algorithm>
#include "util.hpp"
#include "graph.hpp"
using namespace std;

#define D(...) fprintf(stderr, __VA_ARGS__)

Edge::Edge(int id, int u, int v) : id(id), u(u), v(v)
{
  assert(u < v);
}
void Edge::show()
{
  printf("e%d = (%d, %d)\n", id, u, v);
}

Graph::Graph(int n) : N(n)
{
  adj = vector<Edges>(N);
  tau_set = false;
}

void Graph::add_edge(int a, int b)
{
  if (a > b)
    swap(a, b);
  assert(b < N);
  assert(a != b);
  Edge e = Edge(edges.size(), a, b);
  e.show();
  adj[e.u].push_back(e);
  adj[e.v].push_back(e);
  edges.push_back(e);
  tau_set = false;
}
int Graph::get_size() {return N;}

void Graph::show(string name)
{
  printf("graph %s {\n", name.c_str());
  for (auto e: edges) {
    printf("  v%d -- v%d;\n", e.u, e.v);
  }
  printf("}\n");
}

void Graph::calcProb()
{
  W = 0;
  acc_tau = vector<int>(edges.size());
  for (unsigned i = 0; i < edges.size(); ++i) {
    int d1 = adj[edges[i].u].size();
    int d2 = adj[edges[i].v].size();
    int tau = (d1 - 1) * (d2 - 1);
    acc_tau[i] = W;
    W += tau;
  }
  tau_set = true;
}

path3 Graph::sample()
{
  if (!tau_set) {
    calcProb();
  }
  int r = rand_n(W);
  int uv_i = upper_bound(acc_tau.begin(), acc_tau.end(), r) - acc_tau.begin() - 1;

  assert(uv_i < (int)edges.size());
  Edge uv = edges[uv_i];
  assert(adj[uv.u].size() > 1);
  assert(adj[uv.v].size() > 1);
  Edge tu, vw;
  while(1) {
    r = rand_n(adj[uv.u].size());
    tu = adj[uv.u][r];
    if(tu.v != uv.v) {// tu != uv
      break;
    }
  }

  while(1) {
    r = rand_n(adj[uv.v].size());
    vw = adj[uv.v][r];
    if(uv.u != vw.u) {
      break;
    }
  }
  assert(tu.u == uv.u || tu.v == uv.u);
  assert(vw.u == uv.v || vw.v == uv.v);

  return path3(tu.id, uv.id, vw.id);
}

void Graph::sample_debug_test(int n) {

  vector<int> test(edges.size(), 0);

  for(int i = 0; i < n; ++i) {
    int idx = get<1>(sample());
    test[idx] += 1;
  }

  assert(edges.size() == acc_tau.size());

  double maxd = -1;

  for (unsigned i = 0; i < edges.size(); ++i) {
    int tau = (i==edges.size()-1) ? W-acc_tau[i] : acc_tau[i+1] - acc_tau[i];

    double exp = (double) tau / (double) W;
    double real = (double) test[i] / (double) n;

    D("%d: exp %lf %%, real %lf %%\n", i, exp, real);
    maxd = max(maxd, abs(exp -real));
  }
  D("max d = %.10lf\n", maxd);

}
