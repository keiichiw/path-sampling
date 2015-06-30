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
  adj = vector<Adjs>(N);
}

void Graph::set_n(int n)
{
  N = n;
  adj = vector<Adjs>(N);
}

void Graph::add_edges(vector<Pair> es)
{
  int e_id = edges.size();
  for(auto p: es) {
    int a = p.first, b = p.second;
    if (a > b)
      swap(a, b);
    assert(a != b && 0 <= a && b < N);
    if (has_edge.find(make_pair(a, b)) != has_edge.end()) {
      continue;
    }
    Edge e = Edge(e_id++, a, b);
    add_edge(e);
  }
  preprocess();
}

void Graph::add_edge(Edge e)
{
  //e.show();
  adj[e.u].push_back(make_pair(-1, e.v));
  adj[e.v].push_back(make_pair(-1, e.u));
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

void Graph::preprocess()
{

  for (unsigned i = 0; i < adj.size(); ++i) {
    for (unsigned j = 0; j < adj[i].size(); ++j) {
      int to = adj[i][j].second;
      adj[i][j].first = adj[to].size();
    }
    sort(adj[i].begin(), adj[i].end());
  }


  // for 3-path-sample
  W = 0;
  acc_tau = vector<ll>(edges.size());
  for (unsigned i = 0; i < edges.size(); ++i) {
    int d1 = adj[edges[i].u].size();
    int d2 = adj[edges[i].v].size();
    int tau = (d1 - 1) * (d2 - 1);
    acc_tau[i] = W;
    W += tau;
  }
  D("W=%lld\n", W);

  // for centered-3-path-sample
  L = 0;
  acc_lambda = vector<ll>(edges.size());
  uppers = vector<pair<EIter, EIter>>(edges.size());
  for (unsigned i = 0; i < edges.size(); ++i) {
    int u = edges[i].u, v = edges[i].v;
    int d1 = adj[edges[i].u].size(), d2 = adj[edges[i].v].size();
    auto p1 = upper_bound(adj[u].begin(), adj[u].end(), make_pair(d2, v));
    auto p2 = upper_bound(adj[v].begin(), adj[v].end(), make_pair(d1, u));
    int l1 = adj[u].end() - p1;
    int l2 = adj[v].end() - p2;
    acc_lambda[i] = L;
    L += l1 * l2;
    uppers[i] = make_pair(p1, p2);
  }
  D("L=%lld\n", L);

}

path Graph::sample()
{
  ll r = rand_n(W);
  int uv_i = upper_bound(acc_tau.begin(), acc_tau.end(), r) - acc_tau.begin() - 1;
  assert(0 <= uv_i && uv_i < (int)edges.size());
  Edge uv = edges[uv_i];

  assert(adj[uv.u].size() > 1);
  assert(adj[uv.v].size() > 1);
  int t, w;

  while(1) {
    r = rand_n(adj[uv.u].size());
    t = adj[uv.u][r].second;
    if(t != uv.v) {// tu != uv
      break;
    }
  }

  while(1) {
    r = rand_n(adj[uv.v].size());
    w = adj[uv.v][r].second;
    if(uv.u != w) {
      break;
    }
  }

  return path(t, uv.u, uv.v, w);
}

path Graph::sample_centered()
{
  ll r = rand_n(L);
  int uv_i = upper_bound(acc_lambda.begin(), acc_lambda.end(), r) - acc_lambda.begin() - 1;

  assert(0 <= uv_i && uv_i < (int)edges.size());

  Edge uv = edges[uv_i];
  auto p1 = uppers[uv_i].first;
  auto p2 = uppers[uv_i].second;

  int l1 = adj[uv.u].end() - p1;
  int l2 = adj[uv.v].end() - p2;

  assert(l1 >= 1);
  assert(l2 >= 1);

  int t, w;

  while(1) {
    r = rand_n(l1);
    t = (*(p1 + r)).second;
    if(t != uv.v) {// tu != uv
      break;
    }
  }

  while(1) {
    r = rand_n(l2);
    w = (*(p2 + r)).second;
    if(uv.u != w) {
      break;
    }
  }

  return path(t, uv.u, uv.v, w);

}

Motif Graph::judge_induced(path p)
{

  const vector<int> MOTIFS[6] = {
    {1, 1, 1, 3}, {1, 1, 2, 2},{1, 2, 2, 3},
    {2, 2, 2, 2}, {2, 2, 3, 3},{3, 3, 3, 3}
  };

  int vs[4] = {
    get<0>(p), get<1>(p), get<2>(p), get<3>(p)
  };

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


void Graph::sample_debug_test(int n)
{

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

void Graph::path_sampler(int k)
{
  const int A2[6] = {0, 1, 2, 4, 6, 12};
  ll count[6] = {0};

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
  ll n1 = 0;

  for (unsigned i = 0; i < adj.size(); ++i) {
    ll d = adj[i].size();
    n1 += (d * (d-1) * (d-2)) / 6;
  }
  c[0] = (double)n1 - c[2] - 2*c[4] - 4*c[5];

  for(int i = 0; i < 6; ++i) {
    cout << i << ": " << c[i] << ", ";
  }
  cout<<endl;

}

bool Graph::is_centered(path p)
{
  int t = get<0>(p);
  int w = get<3>(p);

  if (t == w)
    return false;

  if (t > w)
    swap(t, w);

  return has_edge.find(make_pair(t, w)) != has_edge.end();

}

void Graph::centered_sampler(int k)
{
  const int B[3] = {1, 1, 3};
  ll count[3] = {0};

  for (int i = 0; i < k; ++i) {
    auto path = sample_centered();

    if (!is_centered(path)) {
      continue;
    }

    Motif m = judge_induced(path);
    assert(3 <= (int)m);
    count[(int)m - 3] += 1;
  }

  double c[3] = {0};
  for (int i = 0; i < 3; ++i) {
    c[i] = ((double)count[i] / (double)k) * ((double) L / (double) B[i]);
  }

  for(int i = 0; i < 3; ++i) {
    cout << i + 3 << ": " << c[i] << ", ";
  }
  cout<<endl;

}
