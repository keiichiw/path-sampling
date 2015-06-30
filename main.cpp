#include <sys/time.h>
#include <iostream>
#include <fstream>
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


Graph generate_example()
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

Graph generate_clique()
{
  Graph g = Graph(6);
  vector<Pair> edges;

  for (int i = 0; i < 3; ++i) {
    for (int j = i + 1; j < 4; ++j) {
      edges.push_back(make_pair(i, j));
    }
  }

  g.add_edges(edges);

  return g;
}

void read_graph_data(Graph &g)
{
  //const int nodes = 400727;
  const int nodes = 916428;
  vector<Pair> es;
  //ifstream ifs("/home/udon/workspace/SNAP/amazon0312.txt");
  ifstream ifs("/home/udon/workspace/SNAP/web-Google.txt");
  string s;
  g.set_n(nodes);
  for(int i = 0; i < 4; ++i)
    getline(ifs, s);

  int src, dst;
  while(ifs >> src >> dst) {
    es.push_back(make_pair(src, dst));
  }
  g.add_edges(es);
}

double get_dtime(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((double)(tv.tv_sec) + (double)(tv.tv_usec) * 0.001 * 0.001);
}

int main()
{

  const int TIMES = 200000;
  Graph g;
  double t1, t2;
  read_graph_data(g);
  //g = generate_graph(400000, 3000000);
  //g = generate_example();
  t1 = get_dtime();
  g.path_sampler(TIMES);
  g.centered_sampler(TIMES);
  t2 = get_dtime();
  printf("%.10lf s\n", t2-t1);

}
