#include <iostream>
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

void Graph::add_edges(vector<Pair> es)
{
  int e_id = edges.size();
  for(auto p: es) {
    int a = p.first, b = p.second;
    if (a > b)
      swap(a, b);
    assert(a != b && 0 <= a && b < N);
    Edge e = Edge(e_id++, a, b);
    add_edge(e);
  }
  tau_set = false;
}

void Graph::add_edge(Edge e)
{
  e.show();
  adj[e.u].push_back(e);
  adj[e.v].push_back(e);
  edges.push_back(e);
  has_edge.insert(make_pair(e.u, e.v));
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

Motif Graph::judge_induced(path3 p)
{
  const vector<int> MOTIFS[6] = {
    {1, 1, 1, 3}, {1, 1, 2, 2},{1, 2, 2, 3},
    {2, 2, 2, 2}, {2, 2, 3, 3},{3, 3, 3, 3}
  };

  Edge tu = edges[get<0>(p)];
  Edge uv = edges[get<1>(p)];
  Edge vw = edges[get<2>(p)];
  int vs[4];
  vs[1] = uv.u;  // u
  vs[2] = uv.v;  // v
  vs[0] = tu.u != vs[1] ? tu.u : tu.v; // t
  vs[3] = vw.u != vs[2] ? vw.u : vw.v; // w
  if (vs[0] == vs[3]) {
    return Triangle;
  }
  vector<int> degs = vector<int>(4, 0);

  for (int i = 0; i < 3; ++i) {
    for (int j = i+1; j < 4; ++j) {
      int a = min(vs[i], vs[j]);
      int b = max(vs[i], vs[j]);
      if (has_edge.find(make_pair(a, b)) != has_edge.end()) {
        degs[i] += 1;
        degs[j] += 1;
      }
    }
  }

  sort(degs.begin(), degs.end());

  for (int i = 0; i < 6; ++i) {
    if (degs == MOTIFS[i]) {
      return (Motif) i;
    }
  }

  assert(false);

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

void Graph::path_sampler(int k) {
  const int A2[6] = {0, 1, 2, 4, 6, 12};
  int count[6] = {0};

  for (int i = 0; i < k; ++i) {
    auto path = sample();
    Motif m = judge_induced(path);
    if (m == Triangle) {
      continue;
    }
    count[(int)m] += 1;
  }

  double c[6] = {0};
  for (int i = 1; i < 6; ++i) {
    c[i] = ((double)count[i] / (double)k) * ((double) W / (double) A2[i]);
  }
  int n1 = 0;

  for (unsigned i = 0; i < adj.size(); ++i) {
    int d = adj[i].size();
    n1 += (d * (d-1) * (d-2)) / 6;
  }
  c[0] = (double)n1 - c[2] - 2*c[4] - 4*c[5];

  for(int i = 0; i < 6; ++i) {
    cout << c[i] << " ";
  }
  cout<<endl;

}
