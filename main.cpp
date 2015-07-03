#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <cstring>
#include "graph.hpp"

using namespace std;

void read_dimacs_data(Graph &g) {
  vector<Pair> es;
  ifstream ifs(g.name);

  if (!ifs.good()) {
    cerr << g.name << " not found" << endl;
    exit(1);
  }
  string c;
  bool flg = false;
  while (ifs >> c) {
    if (c == "c") {
      string t;
      getline(ifs, t);
    } else if (c == "p") {
      int v, e;
      ifs >> c >> v >> e;
      g.set_n(v+1);
      flg = true;
    } else if (c == "a") {
      int src, dst, d;
      if (!flg) {
        cerr << "size is not set yet" << endl;
        exit(1);
      }
      ifs >> src >> dst >> d;
      if (src == dst)
        continue;
      es.push_back(make_pair(src, dst));
    } else {
      cerr << "dimacs input error : " << c << endl;
    }
  }
  g.add_edges(es);
}

double get_dtime(){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((double)(tv.tv_sec) + (double)(tv.tv_usec) * 0.001 * 0.001);
}


void time_test(Graph &g) {

  const int SAMPLE_K = 200000;
  const int TEST_NUM = 10;

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
  printf("Data: %s (k = %d)\n", g.name.c_str(), SAMPLE_K);
  printf("3path-sampler:    %lfs\n", time1);
  printf("centered-sampler: %lfs\n", time2);
}

void run(Graph &g) {
  const int SAMPLE_K = 200000;
  g.preprocess_3path();
  g.path_sampler(SAMPLE_K);

  // centered-sampler
  g.preprocess_centered();
  g.centered_sampler(SAMPLE_K);

}

int main(int argc,char *argv[])
{
  if (!(argc == 2 ||
        (argc == 3 && strcmp(argv[1], "-t") == 0))) {
    cerr << "Usage: ./main [-t] <DIMACS format file>" << endl;
    return 0;
  }
  Graph g;
  bool time_flg = false;
  if (argc == 2) {
    g.name = string(argv[1]);
  } else {
    g.name = string(argv[2]);
    time_flg = true;
  }

  int n_sz = g.name.size();
  if (n_sz < 3 || g.name.substr(n_sz - 3) != ".gr") {
    cerr << "filename extension must be '.gr'" << endl;
    return 0;
  }

  read_dimacs_data(g);

  if (time_flg) {
    time_test(g);
  } else {
    run(g);
  }


}
