#include <set>
#include <string>
#include "util.hpp"
#include "graph.hpp"

using namespace std;

Graph generate_graph(int n, int m)
{
  Graph g = Graph(n);
  set <Pair> st;
  vector<Pair> edges;

  for (int i = 0; i < m; ++i) {
    int a = rand_n(n);
    int b = rand_n(n);
    if (a > b) {
      swap(a, b);
    }
    if (a == b ||
        st.find(make_pair(a, b)) != st.end()) {
      --i;
      continue;
    }
    edges.push_back(make_pair(a, b));
    st.insert(make_pair(a, b));
  }

  g.add_edges(edges);

  return g;
}


Graph generate_example ()
{
  Graph g = Graph(6);
  vector<Pair> edges = {
    make_pair(0, 1),
    make_pair(0, 2),
    make_pair(0, 3),
    make_pair(1, 2),
    make_pair(1, 4),
    make_pair(2, 3),
    make_pair(3, 4),
    make_pair(4, 5)
  };
  g.add_edges(edges);

  return g;
}


int main ()
{
  //Graph g = generate_graph(50, 500);
  Graph g = generate_example();
  g.path_sampler(1000000);
}
