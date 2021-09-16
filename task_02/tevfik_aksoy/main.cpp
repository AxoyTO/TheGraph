#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <set>
#include <fstream>

using std::cout;
using std::endl;
using std::set;
using std::tuple;
using std::vector;

//Structure of nodes(vertices)
struct Node
{
	int node_id = 0;
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
	Node src_node = {};
	Node dest_node = {};
	int edge_val = 0;
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

			graphVec[src_id].push_back(std::make_tuple(dest_id, edge_val));
		}
	};

	~Graph()
	{
		//Destructor
	}
};

//Opens a json file for writing and writes on it info from the graph
void writeJson(const Graph &graph, int node_c, const vector<Edge> &edges)
{
	std::ofstream ofile("graph.json", std::ios::out);
	if (!ofile.is_open())
	{
#line 151 "graph.json"
		std::cerr << "Error while opening the file " << __FILE__ << endl;
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
					ofile << std::get<1>(graph.graphVec[i][j]) << ",";
				else
					ofile << std::get<1>(graph.graphVec[i][j]) << "]\n";
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
