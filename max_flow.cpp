/***
* INPUT files:
* 1) railroad.in: number M of edges in the directed railroad graph, followed by description of each edge: LABEL1 LABEL2 CAPACITY
* 2) constructions.in: number N of construction sites followed by description of each site: LABEL1 LABEL2 REDUCTION_COEFFICIENT DATE1 DATE2
* 3) coordinates.in: number C of coordinates followed by description for each coordinate: LABEL COORDINATE1 COORDINATE 2
* 4) user.in: DATE to be queried, followed by maximum allowed THRESHOLD
*
* OUTPUT files:
* output.txt: list of bottleneck stations for that day: LABEL1 COORDINATE11 COORDINATE12 LABEL2 COORDINATE21 COORDINATE22 INITIAL_CAPACITY FINAL_CAPACITY
*/

#include <bits/stdc++.h>

using namespace std;

const int LIM = 10; // denotes the maximum sample size for the determination of the bottlenecks (if sample size is less than LIM, additional random points are added)
const int LOCALITY = 75; // denotes the "locality" of the max-flow algorithm (the distance of the farthest node that should be considered, starting from the source)
const int RADIUS = 4; // denotes the greatest distance we consider when picking "relevant nodes" in a neighborhood

struct Bottleneck { // struct that stores the bottleneck as two stations (from, to), initial capacity of the road between the stations (directed)
  int from, to, initial, reduced; // and the new, possibly reduced, capacity after the construction sites
  Bottleneck (int f, int t, int i = 0, int r = 0) : from(f), to(t), initial(i), reduced(r) {}
};

struct GraphEdge { // struct that stores the edges of the railroad by source, destination and capacity
  string from, to;
  int cap;
  GraphEdge (string f, string t, int c) : from(f), to(t), cap(c) {}
};

struct Construction { // struct that stores the position of a construction site with endpoints, capacity-reduction-coefficient, start date and end date
  string from, to;
  double reduction;
  int dateStart, dateEnd;
};

int day; // day queried by the user
double REDUCED_LIM; // precision desired by the user
map < string, int > codes; // maps the names of the stations (e.g. "ALL", "KGO") to an integer ID (for easy of representation)
vector < int > father; // stores the fathers in the flow network tree for each augmenting path iteration
vector < pair < double, double > > coord; // associates for each ID a pair of coordinates
vector < bool > seen; // vector that stores if a node has been visited or not during the augmenting path BFS
vector < string > nodes; // stores the original strings for each ID (for conversion back to the original labels)
vector < GraphEdge > listEdges; // array of the graph edges
vector < Construction > constr; // array of the construction sites
vector < vector < int > > graph, capacity, flow; // 2D arrays for adjacency list, capacity and flow adjacency matrix

void readGraph(ifstream& fin) { // reading the graph out of the specific input
  int numEdg;
  fin >> numEdg;
  for (int i = 0; i < numEdg; ++i) { // read in edges
    string a, b;
    int c;
    fin >> a >> b >> c;
    listEdges.emplace_back(a, b, c);
    if (codes.find(a) == codes.end()) { // determine IDs for the nodes
      codes[a] = (int) nodes.size();
      nodes.push_back(a);
    }
    if (codes.find(b) == codes.end()) {
      codes[b] = (int) nodes.size();
      nodes.push_back(b);
    }
  }

  father.resize(nodes.size()); // resize arrays to match size of input
  seen.resize(nodes.size());
  graph.resize(nodes.size());
  capacity.resize(nodes.size());
  flow.resize(nodes.size());
  for (int i = 0; i < (int) nodes.size(); ++i) {
    capacity[i].resize(nodes.size());
    flow[i].resize(nodes.size());
  }

  for (auto edg : listEdges) { // construct graph adjacency list
    int x = codes[edg.from];
    int y = codes[edg.to];
    capacity[x][y] += edg.cap; // modify flow network
    if (find(graph[x].begin(), graph[x].end(), y) == graph[x].end()) { // check that edges aren't added multiple times
      graph[x].push_back(y);
    }
    if (find(graph[y].begin(), graph[y].end(), x) == graph[y].end()) {
      graph[y].push_back(x);
    }
  }
}

void readConstructions(ifstream& fin) { // reading the construction sites out of the specific input
  int num;
  fin >> num;
  constr.resize(num); // resizing accordingly
  for (int i = 0; i < (int) constr.size(); ++i) {
    fin >> constr[i].from >> constr[i].to >> constr[i].reduction >> constr[i].dateStart >> constr[i].dateEnd;
  }
}

void readCoordinates(ifstream& fin) { // reading the construction sites out of the specific input
  int num;
  fin >> num;
  coord.resize(nodes.size());
  coord.assign(nodes.size(), make_pair(0.00000, 0.00000)); // assign a default value for points that have undefined coordinates
  for (int i = 0; i < num; ++i) {
    string s;
    fin >> s;
    fin >> coord[codes[s]].first >> coord[codes[s]].second;
  }
}

void readUserInput(ifstream& fin) { // reading the user input out of the specific input
  fin >> day >> REDUCED_LIM; // day that is queried and the desired threshold
}

bool BFS(int source, int sink, vector < vector < int > >& cap, int lim = 0) { // BFS that finds an augmenting path for the flow network
  vector < int > dist(nodes.size());
  seen.assign(nodes.size(), false);
  queue < int > q;
  q.push(source);
  dist[source] = 0;
  seen[source] = true;
  if (lim == 0)
    lim = (int) nodes.size();

  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    if (curr != sink && dist[curr] <= lim) {
      for (auto ngh : graph[curr]) {
        if (!seen[ngh] && flow[curr][ngh] < cap[curr][ngh]) {
          seen[ngh] = true;
          q.push(ngh);
          father[ngh] = curr;
          dist[ngh] = dist[curr] + 1;
        }
      }
    }
  }

  return seen[sink];
}

int maxflow(int source, int sink, vector < vector < int > >& cap, int lim = 0) { // function computing the max-flow given a specific network and
  int mxflow = 0; // a 2D array of the capacities of the edges
  // this is a modified version of the classical max-flow because it only takes into account the nodes in the neighborhood of the source node
  for (int i = 0; i < (int) nodes.size(); ++i) { // (more precisely: the neighbors at distance at most lim)
    flow[i].assign(nodes.size(), 0);
  }

  while (BFS(source, sink, cap, lim)) {
    for (auto sink_ngh : graph[sink]) {
      if (seen[sink_ngh]) {
        father[sink] = sink_ngh;
        int fl = INT_MAX;

        for (int curr = sink; curr != source; curr = father[curr]) {
          fl = min(fl, cap[father[curr]][curr] - flow[father[curr]][curr]);
        }
        mxflow += fl;

        for (int curr = sink; curr != source; curr = father[curr]) {
          flow[father[curr]][curr] += fl;
          flow[curr][father[curr]] -= fl;
        }
      }
    }
  }
  return mxflow;
}

int relevantNeighbor(int node, int radius = RADIUS) { // function that gets a node in the neighborhood of a given node (optional radius parameter)
  int prev = -1;
  for (int i = 0; i < radius; ++i) { // iterate randomly through the neighbors
    int idx = rand() % graph[node].size();
    if (graph[node][idx] == prev) // try to not go back
      idx = (idx + 1) % graph[node].size();
    prev = node;
    node = graph[node][idx];
  }
  return node;
}

vector < Bottleneck > computeBottleneck(int date) { // function returning an array of potential bottleneck stations
  vector < Bottleneck > ans;
  vector < pair < int, int > > crit;
  vector < vector < int > > newCap = capacity;

  for (auto it : constr) {
    if (it.dateStart <= date && date <= it.dateEnd) { // check if construction site is active in the queried day
      int a = codes[it.from], b = codes[it.to];
      if (a == b) {
        for (auto ngh : graph[a]) {
          if (capacity[a][ngh] == 0) { // if construction sites is located on node
            newCap[ngh][a] = (1.0 - it.reduction / graph[a].size()) * capacity[ngh][a]; // modify the capacity of each ingoing edge
          } else {
            newCap[a][ngh] = (1.0 - it.reduction / graph[a].size()) * capacity[a][ngh]; // modify the capacity of each outgoing edge
          }
        }
      } else { // else, construction site happens on an edge
        newCap[a][b] = (1.0 - it.reduction) * capacity[a][b]; // modify the capacity
      }

      a = relevantNeighbor(a); // get nodes in the neighborhood of the affected edges so that we can extract a sample of how bad the
      b = relevantNeighbor(b); // flow in the area was affected
      if (a == b) {
        a = relevantNeighbor(a, 1);
      }
      crit.push_back(make_pair(a, b));
    }
  }

  for (int i = (int) crit.size(); i < LIM; ++i) { // if sample size is too small, add random nodes from the map to cover eventual butterfly effects
    int node = rand() % nodes.size();
    int a = relevantNeighbor(node), b = relevantNeighbor(node);
    if (a == b) {
      a = relevantNeighbor(a, 1);
    }
    crit.push_back(make_pair(a, b));
  }

  for (int i = 0; i < (int) crit.size(); ++i) { // compute new max-flow (on the graph with the modified edges) to see what impact the construction
    int a = crit[i].first, b = crit[i].second; // sites had for the general capacity of the network
    ans.emplace_back(a, b, maxflow(a, b, capacity, LOCALITY), maxflow(a, b, newCap, LOCALITY));
  }
  return ans;
}

void solve_input(ofstream& fout) { // given the user input, compute the desired result for the query
  vector < Bottleneck > ans = computeBottleneck(day);

  for (auto it : ans) {
    if ((1.0 * it.initial - it.reduced) / it.initial > REDUCED_LIM) { // check the threshold
      fout << nodes[it.from] << "," << setprecision(11) << fixed << coord[it.from].first << "," << setprecision(10) << fixed << coord[it.from].second << ",";
      fout << nodes[it.to] << "," << setprecision(11) << fixed << coord[it.to].first << "," << setprecision(10) << fixed << coord[it.to].second << ",";
      fout << it.initial << "," << it.reduced << endl;
    }
  }
}

int main()
{
    srand(time(nullptr));

    ifstream finR("railroad.in");
    ifstream finC("constructions.in");
    ifstream finK("coordinates.in");
    ifstream finU("user.in");
    ofstream fout("output.txt");

    readGraph(finR); // reading input and precomputation
    readConstructions(finC);
    readCoordinates(finK);
    readUserInput(finU);

    solve_input(fout); // solving query and printing output

    finR.close();
    finC.close();
    finU.close();
    fout.close();
    return 0;
}
