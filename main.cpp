#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "util.hpp"
#include "graph.hpp"

using namespace std;

Graph generate_random_graph(int n, int m)
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

void read_graph_data(Graph &g, string fname, int nodes)
{
  vector<Pair> es;
  ifstream ifs(fname);
  string s;
  g.set_n(nodes);

  for(int i = 0; i < 4; ++i) // ignore comments
    getline(ifs, s);

  int src, dst;
  int cnt = 0;
  while(ifs >> src >> dst) {
    es.push_back(make_pair(src, dst));
    cnt++;
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
  const int TEST_NUM = 10;
  Graph g;

  read_graph_data(g, "/home/udon/workspace/SNAP/web-Google.txt", 920000);
  //read_graph_data(g, "/home/udon/workspace/SNAP/amazon0312.txt", 400000);

  double time1 = 0.0, time2 = 0.0;

  for (int i = 0; i < TEST_NUM; ++i) {
    double t1, t2, t3;
    Graph g1 = g;
    t1 = get_dtime();
    g1.path_sampler(TIMES);
    t2 = get_dtime();
    g1.centered_sampler(TIMES);
    t3 = get_dtime();
    time1 += t2 - t1;
    time2 += t3 - t2;
  }
  time1 /= TEST_NUM;
  time2 /= TEST_NUM;

  printf("3path-sampler(k=%d): %lfs\n", TIMES, time1);
  printf("centered-sampler(k=%d): %lf s\n", TIMES, time2);

}
