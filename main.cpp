#include <set>
#include <string>
#include "util.hpp"
#include "graph.hpp"

using namespace std;

Graph generate_graph(int n, int m)
{
  Graph g = Graph(n);
  set <Pair> st;

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
    g.add_edge(a, b);
    st.insert(make_pair(a, b));
  }

  return g;
}


Graph generate_example ()
{
  Graph g = Graph(6);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  g.add_edge(1, 2);
  g.add_edge(1, 4);
  g.add_edge(2, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 5);
  return g;
}


int main ()
{
  Graph g = generate_graph(50, 500);
  //Graph g = generate_example();
  g.sample_debug_test(1000000);
  //g.show("Sample");

}
