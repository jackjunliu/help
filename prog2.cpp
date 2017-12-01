#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_NODE 100
#define edge pair<int,int>

class Graph {
private:
    vector<pair<int, edge> > G; // The graph
    vector<pair<int, edge> > T; // The MST
    int *parent;
    int V; // The number of nodes in the graph
    vector<int> nodes;
    vector<int> weights;
public:
    Graph(int V);
    void AddWeightedEdge(int u, int v, int w);
    int find_edge(int i);
    void union_edge(int u, int v);
    void kruskalMST(bool print_cost);
    void print_MST();
    void NumberOfNodes();
    void CheckForCycle();
    void ShortestPath(int u, int v, bool first);
};

Graph::Graph(int V) {
    parent = new int[V];

    for (int i = 0; i < V; i++) {
	parent[i] = i;
    }

    G.clear();
    T.clear();
}

void Graph::AddWeightedEdge(int u, int v, int w) {
    if (u < v) {
	G.push_back(make_pair(w, edge(u, v)));
    } else {
 	G.push_back(make_pair(w, edge(v, u)));
    }
    nodes.push_back(u);
    if (v != u) {
	nodes.push_back(v);
    }
    weights.push_back(w);
}

int Graph::find_edge(int i) {
    if (i == parent[i]) {
	return i;
    } else {
      return find_edge(parent[i+1]);
    }
}

void Graph::union_edge(int u, int v) {
    parent[u] = parent[v];
}

void Graph::NumberOfNodes() {
    sort (nodes.begin(), nodes.end());
    nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
    cout << nodes.size() << endl;
}

void Graph::CheckForCycle() {
    sort (nodes.begin(), nodes.end());
    nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
    if (nodes.size() > weights.size()) {
	cout << "NO" << endl;
    } else {
    	cout << "YES" << endl;
    }
}

void Graph::kruskalMST(bool print_cost) {
    int uRep, vRep;
    size_t i;
    int cost, minCost = 0;
    sort(G.begin(), G.end()); // Sort by weight;
    if (T.size() != 0) {
    	for (int j = 0; j < MAX_NODE; j++) {
	    parent[j] = j;
	}
	T.clear();
    }
    for (i = 0; i < G.size(); i++) {
	uRep = find_edge(G[i].second.first);
    	vRep = find_edge(G[i].second.second);
	cost = G[i].first;
	if (uRep != vRep) {
	    minCost += cost;
	    T.push_back(G[i]); 
	    union_edge(uRep, vRep);
	}
    }
    if (print_cost) {
	cout << minCost << endl;
    }
}

void Graph::ShortestPath(int u, int v, bool first) {
    if (first) {
	kruskalMST(false);
 	cout << u << " ";
    }
    int temp;
    temp = u;
    for (size_t i = 0; i < T.size(); i++) { //ASDF
    	if (T[i].second.first == temp) {
	    temp = T[i].second.second;
	    cout << temp << " ";
    	}
	if (temp == v) {
	    cout << endl;
	    break;
  	}
    }
    if (temp != v) {
	ShortestPath(temp, v, false);
    }
}

void Graph::print_MST() {
    kruskalMST(false);
    for (size_t i = 0; i < T.size(); i++) {
	cout << "(" << T[i].second.first << "," << T[i].second.second << ")" << " ";
    }
    cout << endl;
}

vector<string> parse_line(const string& target, const string& delimiter)
{
    vector<string> result;
    string::const_iterator substart = target.begin(), subend;

    while (true) {
        subend = search(substart, target.end(), delimiter.begin(), delimiter.end());
        string temp(substart, subend);
        if (!temp.empty()) {
	    result.push_back(temp);
        }
	if (subend == target.end()) {
	    break;
        }
	substart = subend + delimiter.size();
    }
    return result;
}

void print_menu() {
    cout << endl;
    cout << "The following commands are available: " << endl;
    cout << "  BID node_number node_number cost" << endl;
    cout << "  MANDATORY node_number node_number cost" << endl;
    cout << "  COST?" << endl;
    cout << "  COMPONENTS?" << endl;
    cout << "  CYCLE?" << endl;
    cout << "  LIST?" << endl;
    cout << "  SHORTEST_PATH? node_number node_number" << endl;
    cout << "  END" << endl;
}

int main() {
    Graph g(MAX_NODE);
    string line;
    vector<string> cmd_line_split_str;
    int node_1, node_2, cost;
    size_t i;

    print_menu();
    do {
 	getline(std::cin, line);
     	cmd_line_split_str = parse_line(line, " ");
	if ((cmd_line_split_str[0] == "BID") || (cmd_line_split_str[0] == "MANDATORY")) {
	    node_1 = atoi(cmd_line_split_str[1].c_str());
	    node_2 = atoi(cmd_line_split_str[2].c_str());
	    cost = atoi(cmd_line_split_str[3].c_str());
	    g.AddWeightedEdge(node_1, node_2, cost);
	} else if (cmd_line_split_str[0] == "COST?") {
	    g.kruskalMST(true);
        } else if (cmd_line_split_str[0] == "COMPONENTS?") {
	    g.NumberOfNodes();
	} else if (cmd_line_split_str[0] == "CYCLE?") {
	    g.CheckForCycle();
	} else if (cmd_line_split_str[0] == "LIST?") {
	    g.print_MST();
	} else if (cmd_line_split_str[0] == "SHORTEST_PATH?") {
	    node_1 = atoi(cmd_line_split_str[1].c_str());
	    node_2 = atoi(cmd_line_split_str[2].c_str());
	    cost = atoi(cmd_line_split_str[3].c_str());
	    g.ShortestPath(node_1, node_2, true);
	} else if (cmd_line_split_str[0] == "END") {
	    // Do nothing here.
	} else {
	    cout << line << " is invalid." << endl;
	    print_menu();
	}
    } while (cmd_line_split_str[0] != "END");
    return 0;
}
