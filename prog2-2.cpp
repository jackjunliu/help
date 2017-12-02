#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_NODE 100
#define edge pair<int,int>

class Graph {
private:
  vector<pair<int, edge> > G; //graph x,y
  vector<pair<int, edge> > MST; //minimum tree x,y
  int *parent;
  int *rank;
  int nodenum;
  vector<int> nodes;
  vector<int> weights;
  vector<int> m_weights;
  size_t current;
  int minCost;
public:
  Graph(int nodenum);
  void AddWeightedEdge(int u, int v, int w, bool m);
  int find_edge(int i);
  void union_edge(int u, int v);
  void kruskalMST(bool print_cost);
  void print_MST();
  void NumberOfNodes();
  void CheckForCycle();
  void ShortestPath(int u, int v, bool first);
  void link(int x, int y);
};

Graph::Graph(int nodenum) {
    parent = new int[nodenum];
    rank = new int [nodenum];

    for (int i = 0; i < nodenum; i++) {
	parent[i] = i;
    	rank[i] = 0;
    }

    G.clear();
    MST.clear();
    current = 0;
    minCost = 0;
}

void Graph::AddWeightedEdge(int u, int v, int w, bool m) {
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
    if (m) {
	m_weights.push_back(w);
    }
}

int Graph::find_edge(int i) {
    if (i == parent[i]) {
	return i;
    } else {
	return find_edge(parent[i]);
    }
}

void Graph::link(int x, int y) {
    if (x != y) {
    	if (rank[x] < rank[y]) {
	    rank[y] += rank[x];
	    parent[x] = y;
	} else {
	    rank[x] += rank[y];
	    parent[y] = x;
	}
    }
}

void Graph::union_edge(int u, int v) {
    link(find_edge(u), find_edge(v));
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
    int cost;
    
    sort(G.begin()+current, G.end()); //Take weight, push, pop
    
    for (i = current; i < G.size(); i++) {
	uRep = find_edge(G[i].second.first);
    	vRep = find_edge(G[i].second.second);
	cost = G[i].first;
	if (std::find(m_weights.begin(), m_weights.end(), cost) != m_weights.end()) {
	    minCost += cost;
	    MST.push_back(G[i]);
	    union_edge(uRep, vRep);
        } else {
	    if (uRep != vRep) {
	        minCost += cost;
	        MST.push_back(G[i]); 
	        union_edge(uRep, vRep);
	    }
 	}
    }
    if (print_cost) {
	cout << minCost << endl;
    }
    current = weights.size();
}

void Graph::ShortestPath(int u, int v, bool first) {
    if (first) {
	if (current != weights.size()) {
	    kruskalMST(false);
	}
 	cout << u << " ";
    }
    int temp;
    temp = u;
    for (size_t i = 0; i < MST.size(); i++) {
    	if (MST[i].second.first == temp) {
	    temp = MST[i].second.second;
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
    if (current != weights.size()) {
	kruskalMST(false);
    }
    for (size_t i = 0; i < MST.size(); i++) {
	cout << "(" << MST[i].second.first << "," << MST[i].second.second << ")" << " ";
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

int main() {
    Graph g(MAX_NODE);
    string line;
    vector<string> cmd_line_split_str;
    int node_1, node_2, cost;
    size_t i;

    do {
 	getline(std::cin, line);
     	cmd_line_split_str = parse_line(line, " ");
	if (cmd_line_split_str[0] == "BID") { //difference between bid and mandatory is one has weight, other does not
	    node_1 = atoi(cmd_line_split_str[1].c_str());
	    node_2 = atoi(cmd_line_split_str[2].c_str());
	    cost = atoi(cmd_line_split_str[3].c_str());
	    g.AddWeightedEdge(node_1, node_2, cost, false);
        } else if (cmd_line_split_str[0] == "MANDATORY") { //adds weight and stores it
    	    node_1 = atoi(cmd_line_split_str[1].c_str());
	    node_2 = atoi(cmd_line_split_str[2].c_str());
	    cost = atoi(cmd_line_split_str[3].c_str());
	    g.AddWeightedEdge(node_1, node_2, cost, true);
	} else if (cmd_line_split_str[0] == "COST?") {
	    g.kruskalMST(true);
        } else if (cmd_line_split_str[0] == "COMPONENTS?") {
	    g.NumberOfNodes();
	} else if (cmd_line_split_str[0] == "CYCLE?") {
	    g.CheckForCycle();
	} else if (cmd_line_split_str[0] == "LIST?") {
	    g.print_MST(); // pull up the search tree
	} else if (cmd_line_split_str[0] == "SHORTEST_PATH?") {
	    node_1 = atoi(cmd_line_split_str[1].c_str());
	    node_2 = atoi(cmd_line_split_str[2].c_str());
	    cost = atoi(cmd_line_split_str[3].c_str());
	    g.ShortestPath(node_1, node_2, true);
	} else if (cmd_line_split_str[0] == "END") {
	  //call just to end, no need to do anything
	} else {
	    cout << line << " is invalid." << endl;
	}
    } while (cmd_line_split_str[0] != "END");
    return 0;
}
