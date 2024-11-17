#include "kernel/types.h"
#include "user/user.h"
#include "user/random.h"
#include "user/exp4-cpu-bound.h"

typedef struct _graph_t {
  int n; // number of vertices
  int m; // number of edges
  int *ef, *es; // edges (ef_i, es_i)
  int *deg; // vertices out degree
  int **adj; // adjacency lists
} graph_t;

/*
  Returns a random directed graph with 100-200 vertices and 50-400 edges.
*/
graph_t * random_graph(int *mem_time) {
  int time = uptime();
  graph_t *g = malloc(sizeof (graph_t));
  *mem_time += uptime() - time;

  g->n = random(100, 200);
  g->m = random(50, 400);

  time = uptime();
  g->ef = malloc(g->m * sizeof (int));
  g->es = malloc(g->m * sizeof (int));
  g->deg = malloc(g->n * sizeof (int));
  *mem_time += uptime() - time;

  for (int i = 0; i < (g->n); ++i) {
    g->deg[i] = 0;
  }

  for (int i = 0; i < g->m; ++i) {
    g->ef[i] = random(0, g->n - 1);
    g->es[i] = random(0, g->n - 1);
    ++g->deg[g->ef[i]];
  }

  time = uptime();
  g->adj = malloc(g->n * sizeof (int *));
  *mem_time += uptime() - time;
  for (int i = 0; i < (g->n); ++i) {
    time = uptime();
    g->adj[i] = malloc(g->deg[i] * sizeof (int));
    *mem_time += uptime() - time;
  }

  for (int i = 0; i < (g->n); ++i) {
    g->deg[i] = 0;
  }

  for (int i = 0; i < (g->m); ++i) {
    g->adj[g->ef[i]][g->deg[g->ef[i]]++] = g->es[i];
  }

  return g;
}

void destroy_graph(graph_t *g, int *mem_time) {
  for (int i = 0; i < (g->n); ++i) {
    int time = uptime();
    free(g->adj[i]);
    *mem_time += uptime() - time;
  }
  int time = uptime();
  free(g->ef);
  free(g->es);
  free(g->deg);
  free(g->adj);
  *mem_time += uptime() - time;
}

/*
  Returns the distance from s to t in the graph g,
  or -1 if it is not possible to reach t from s.
 */
int shortest_path(graph_t *g, int s, int t, int *mem_time) {
  int time = uptime();
  int *d = malloc(g->n * sizeof (int));
  int *p = malloc(g->n * sizeof (int));
  int *q = malloc(g->n * sizeof (int));
  *mem_time += uptime() - time;
  for (int i = 0; i < (g->n); ++i) {
    d[i] = -1;
    p[i] = -1;
  }
  int sz = 0;
  q[sz++] = s;
  d[s] = 0;
  for (int it = 0; it < sz; ++it) {
    int u = q[it];
    for (int i = 0; i < (g->deg[u]); ++i) {
      int v = g->adj[u][i];
      if (d[v] == -1) {
        d[v] = d[u] + 1;
        p[v] = u;
        q[sz++] = v;
      }
    }
  }
  int ans = d[t];
  time = uptime();
  free(d);
  free(p);
  free(q);
  *mem_time += uptime() - time;
  return ans;
}

void cpu_bound(int *mem_time) {
  for (int i = 0; i < 1000; ++i) {
    graph_t *g = random_graph(mem_time);
    shortest_path(g, 0, g->n - 1, mem_time);
    destroy_graph(g, mem_time);
  }
}
