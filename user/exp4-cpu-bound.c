#include "kernel/types.h"
#include "user/user.h"
#include "user/random.h"

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
graph_t * random_graph() {
  graph_t *g = malloc(sizeof (graph_t));

  g->n = random(100, 200);
  g->m = random(50, 400);

  g->ef = malloc(g->m * sizeof (int));
  g->es = malloc(g->m * sizeof (int));

  g->deg = malloc(g->n * sizeof (int));
  for (int i = 0; i < (g->n); ++i) {
    g->deg[i] = 0;
  }

  for (int i = 0; i < g->m; ++i) {
    g->ef[i] = random(0, g->n - 1);
    g->es[i] = random(0, g->n - 1);
    ++g->deg[g->ef[i]];
  }

  g->adj = malloc(g->n * sizeof (int *));
  for (int i = 0; i < (g->n); ++i) {
    g->adj[i] = malloc(g->deg[i] * sizeof (int));
  }

  for (int i = 0; i < (g->n); ++i) {
    g->deg[i] = 0;
  }

  for (int i = 0; i < (g->m); ++i) {
    g->adj[g->ef[i]][g->deg[g->ef[i]]++] = g->es[i];
  }

  return g;
}

void destroy_graph(graph_t *g) {
  free(g->ef);
  free(g->es);
  free(g->deg);
  for (int i = 0; i < (g->n); ++i) {
    free(g->adj[i]);
  }
  free(g->adj);
}

/*
  Returns the distance from s to t in the graph g,
  or -1 if it is not possible to reach t from s.
 */
int shortest_path(graph_t *g, int s, int t) {
  int *d = malloc(g->n * sizeof (int));
  int *p = malloc(g->n * sizeof (int));
  for (int i = 0; i < (g->n); ++i) {
    d[i] = -1;
    p[i] = -1;
  }
  int *q = malloc(g->n * sizeof (int));
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
  free(d);
  free(p);
  free(q);
  return ans;
}

int main() {
  for (int i = 0; i < 1000; ++i) {
    graph_t *g = random_graph();
    shortest_path(g, 0, g->n - 1);
    destroy_graph(g);
  }
  return 0;
}
