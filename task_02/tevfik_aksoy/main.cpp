#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <set>
#include <fstream>

using namespace std;
//Structure of nodes(vertices)
struct Node
{
	int node_id;
	Node()
	{
		node_id = 0;
	}
	Node(int node_id)
	{
		this->node_id = node_id;
	}
};

//Tuple of <int,int> for realization of vector of vectors of <int,int>
typedef tuple<int, int> DoubleTuple;

//Structure of the edge including the source and destination nodes in it
struct Edge
{
	Node src_node, dest_node;
	int edge_val;
	Edge(Node src_node, Node dest_node, int edge_val)
	{
		this->src_node = src_node;
		this->dest_node = dest_node;
		this->edge_val = edge_val;
	}
};

class Graph
{
public:
	vector<vector<DoubleTuple>> graphVec;
	Graph(vector<Edge> &edges, int node_amount)
	{
		graphVec.resize(node_amount);
		for (int i = 0; i < edges.size(); i++)
		{
			int src_id = edges[i].src_node.node_id;
			int dest_id = edges[i].dest_node.node_id;
			int edge_val = edges[i].edge_val;

			graphVec[src_id].push_back(make_tuple(dest_id, edge_val));
		}
	};

	void insert_edge(const Edge &e);
	void remove_edge(const Edge &e, set<int> nodes_s, vector<Edge> &edges);

	~Graph()
	{
		//Destructor
	}
};

//Insert an edge to the graph (Это я просто для практики написал)
void Graph::insert_edge(const Edge &e)
{
	graphVec[e.src_node.node_id].push_back(make_tuple(e.dest_node.node_id, e.edge_val));
}

//Remove an edge from the graph(Это тоже просто для практики написал)
void Graph::remove_edge(const Edge &e, set<int> nodes_s, vector<Edge> &edges)
{
	int src_index = -1, dest_index = -1, val;
	try
	{
		for (auto &i : nodes_s)
		{
			if (e.src_node.node_id == i)
				src_index = i;
		}
		if (src_index == -1)
			throw 1;
		else
		{
			for (int i = 0; i < graphVec[src_index].size(); i++)
			{
				if (e.dest_node.node_id == get<0>(graphVec[src_index][i]))
					dest_index = i;
			}
			if (dest_index == -1)
				throw 2;
			else
			{
				if (e.edge_val == get<1>(graphVec[src_index][dest_index]))
				{
					val = e.edge_val;
					graphVec[src_index].erase(graphVec[src_index].begin() + dest_index);
					for (int i = 0; i < edges.size(); i++)
					{
						if (edges[i].src_node.node_id == e.src_node.node_id && edges[i].dest_node.node_id == e.dest_node.node_id && edges[i].edge_val == e.edge_val)
							edges.erase(edges.begin() + i);
					}
				}
				else
					throw 3;
			}
		}
	}
	catch (int a)
	{
		switch (a)
		{
		case 1:
			cout << "No such source node to remove." << endl;
			break;
		case 2:
			cout << "No such destination node to remove." << endl;
			break;
		case 3:
			cout << "No such edge value to remove." << endl;
			break;
		default:
			break;
		}
	}
}

//To display graph
void displayGraph(const Graph &graph)
{
	//vector<vector<DoubleTuple>> gVec = graph.graphVec;
	for (int i = 0; i < graph.graphVec.size(); i++)
	{
		for (int j = 0; j < graph.graphVec[i].size(); j++)
		{
			cout << "(" << i << ", " << get<0>(graph.graphVec[i][j]) << ", " << get<1>(graph.graphVec[i][j]) << ")";
		}
		cout << endl;
	}
}

//Opens a json file for writing and writes on it info from the graph
void writeJson(const Graph &graph, int node_c, const vector<Edge> &edges)
{
	ofstream ofile("graph.json", ios::out);
	if (!ofile.is_open())
	{
#line 151 "graph.json"
		cerr << "Error while opening the file " << __FILE__ << endl;
	}
	else
	{
		ofile << "{\n  \"vertices\": [\n    {\n";
		for (int i = 0; i < node_c; i++)
		{
			ofile << "      \"id\": " << i << "," << endl;
			ofile << "      \"edge_ids\": [";
			for (int j = 0; j < graph.graphVec[i].size(); j++)
			{
				if (graph.graphVec[i].size() != j + 1)
					ofile << get<1>(graph.graphVec[i][j]) << ",";
				else
					ofile << get<1>(graph.graphVec[i][j]) << "]\n";
			}
			if (node_c != i + 1)
				ofile << "	}, {\n";
			else
				ofile << "    }\n  ],\n";
		}
		ofile << "  \"edges\": [\n    {\n";
		for (int i = 0; i < edges.size(); i++)
		{
			ofile << "      \"id\": " << i << "," << endl;
			ofile << "      \"vertex_ids\": [" << edges[i].src_node.node_id << "," << edges[i].dest_node.node_id << "]";
			if (edges.size() != i + 1)
				ofile << "\n	}, {\n";
			else
				ofile << "\n	}\n  ]\n";
		}
		ofile << "}" << endl;
	}
	ofile.close();
}

int main()
{
	vector<Edge> edges =
		{
			{0, 1, 0}, {0, 2, 1}, {0, 3, 2}, {1, 4, 3}, {1, 5, 4}, {1, 6, 5}, {2, 7, 6}, {2, 8, 7}, {3, 9, 8}, {4, 10, 9}, {5, 10, 10}, {6, 10, 11}, {7, 11, 12}, {8, 11, 13}, {9, 12, 14}, {10, 13, 15}, {11, 13, 16}, {12, 13, 17}};

	//using set to exclude the repeating source nodes(the ones with more than 1 edge connected)
	set<int> nodes_set;
	for (int i = 0; i < edges.size(); i++)
	{
		nodes_set.insert(edges[i].src_node.node_id);
	}
	int node_count = nodes_set.size();
	Graph graph(edges, node_count);
	Edge k(12, 13, 17);
	writeJson(graph, node_count, edges);

	return 0;
}