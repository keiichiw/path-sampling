#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "graph.hpp"

using namespace std;

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
    if (src == dst)
      continue;
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

const string data_sets[] = {
  "./data/amazon0312.txt",
  "./data/web-Google.txt",
  "./data/cit-Patents.txt"
};

const int node_count[] = {
  410000, 920000, 10000000
};


int main()
{

  const int SAMPLE_K = 200000;
  const int TEST_NUM = 10;
  Graph g;
  int data_id = 0; // change

  read_graph_data(g, data_sets[data_id], node_count[data_id]);

  printf("%s\n", data_sets[data_id].c_str());


  double time1 = 0.0, time2 = 0.0;

  for (int i = 0; i < TEST_NUM; ++i) {
    double t0, t1, t2;
    Graph g1 = g;
    random_device seed_gen;
    g1.mt_engine = mt19937(seed_gen());
    printf("Test %d------------\n", i);
    t0 = get_dtime();

    // 3path-sampler
    g1.preprocess_3path();
    g1.path_sampler(SAMPLE_K);

    t1 = get_dtime();

    // centered-sampler
    g1.preprocess_centered();
    g1.centered_sampler(SAMPLE_K);

    t2 = get_dtime();

    time1 += t1 - t0;
    time2 += t2 - t1;
  }
  time1 /= TEST_NUM;
  time2 /= TEST_NUM;
  printf("-----------------\n");
  printf("Data: %s (k = %d)\n", data_sets[data_id].c_str(), SAMPLE_K);
  printf("3path-sampler:    %lfs\n", time1);
  printf("centered-sampler: %lfs\n", time2);

}
