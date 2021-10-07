#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::endl;
using std::ofstream;
using std::vector;

int i = 0, j = 0;

struct E {
    int id;
    int v_s;
    int v_e;
};
struct V {
    int id;
    vector <int> e_id;
};

class GRAPH {
public:
    GRAPH(int length, int max) : count_e(length), count_v(max) {
        e_g = new E[count_e];
        v_g = new V[count_v];
    }


    void create_mass_edges(int(*t)[2]);
    void print_mass_edges();
    void create_mass_vertices();
    void print_mass_vertices();

    friend void input_file_json(GRAPH &graph);

    ~GRAPH() {
        delete[] v_g; v_g = 0;
        delete[] e_g; e_g = 0;
    }

private:
    E *e_g;
    V *v_g;
    int count_e;
    int count_v;    
};

void GRAPH::create_mass_edges(int(*t)[2]) {
    for (i = 0; i < count_e; i++) {
        e_g[i].id = i;
        e_g[i].v_s = t[i][0];
        e_g[i].v_e = t[i][1];
    }
}

void GRAPH::print_mass_edges() {
    cout<<"EDGES: "<<endl;
    for (i = 0; i < count_e; i++) {
        cout<<e_g[i].id<<" : "<<e_g[i].v_s<<" - "<<e_g[i].v_e<<endl;
    }
}

void GRAPH::create_mass_vertices() {
    for (i = 0; i < count_v; i++) {
        v_g[i].id = i;
        for (j = 0; j < count_e; j++) {
            if ((e_g[j].v_s == i) or (e_g[j].v_e == i)) {
                v_g[i].e_id.push_back(j);
            }
        }
    }
}

void GRAPH::print_mass_vertices() {
    cout<<endl<<"VERTICES: "<<endl;
    for (i = 0; i < count_v; i++) {
        cout << v_g[i].id <<" : ";
        for (j = 0; j < v_g[i].e_id.size(); j++) {
            cout << v_g[i].e_id[j] << " ";
        }
        cout<<endl;
    }
}

void input_file_json(GRAPH &graph) {
    ofstream outf("graph_oop.json");
    // vertices
	outf <<'{'<<"\n\t"<<'"'<<"vertices"<<'"'<<':'<<" "<<'['<<"\n";
    for (i = 0; i < graph.count_v-1; i++) {
        outf<<"\t\t"<<'{'<<'"'<<"id"<<'"'<<':'<<graph.v_g[i].id
        <<","<<'"'<<"edge_ids"<<'"'<<':'<<'[';
        for (j = 0; j < graph.v_g[i].e_id.size()-1; j++) {
            outf<<graph.v_g[i].e_id[j]<< ",";
        }
        outf<<graph.v_g[i].e_id[graph.v_g[i].e_id.size()-1]<<']'<<'}'<<",\n";
    }
    outf<<"\t\t"<<'{'<<'"'<<"id"<<'"'<<':'<<graph.v_g[graph.count_v-1].id
        <<","<<'"'<<"edge_ids"<<'"'<<':'<<'[';
    for (j = 0; j < graph.v_g[graph.count_v-1].e_id.size()-1; j++) {
        outf<<graph.v_g[graph.count_v-1].e_id[j]<< ",";
    }
    // edges
    outf<<graph.v_g[graph.count_v-1].e_id[graph.v_g[graph.count_v-1].e_id.size()-1]<<']'<<'}'<<"\n\t"<<']'<<",\n\t";
    outf <<'"'<<"edges"<<'"'<<':'<<" "<<'['<<"\n";
    for (i = 0; i < graph.count_e-1; i++) {
        outf<<"\t\t"<<'{'<<'"'<<"id"<<'"'<<':'<<graph.e_g[i].id
        <<","<<'"'<<"vertex_ids"<<'"'<<':'<<'[';
        outf<<graph.e_g[i].v_s<< ",";
        outf<<graph.e_g[i].v_e<<']'<<'}'<<",\n";
    }
    outf<<"\t\t"<<'{'<<'"'<<"id"<<'"'<<':'<<graph.v_g[graph.count_e-1].id
        <<","<<'"'<<"vertex_ids"<<'"'<<':'<<'[';
    outf<<graph.e_g[graph.count_e-1].v_s<< ",";
    outf<<graph.e_g[graph.count_e-1].v_e<<']'<<'}'<<"\n";
    outf<<"\t"<<']'<<"\n";
	outf << "}";
}


int main() {

    std::cout << "Test" << std::endl;

    // GRAPH
    int t[][2] = 
    { {0,1},{0,2},{0,3},
      {1,4},{1,5},{1,6},
      {2,7},{2,8},
      {3,9},
      {4,10},
      {5,10},
      {6,10},
      {7,11},
      {8,11},
      {9,12},
      {10,13},
      {11,13},
      {12,13},
    };

    // NUMBER OF GRAPH EDGES
    int length = sizeof(t) / sizeof(t[0]);
    
    // NUMBER OF GRAPH VERTICES
    int max = t[0][0];
    for (i = 0; i < length; i++) {
        for (j = 0; j < 2; j++) {
            if (t[i][j] > max) 
                max = t[i][j];
        }
    }
    max++;
    
    // cout<<"graph"<<endl;
    GRAPH graph(length,max);
    graph.create_mass_edges(t);
    // cout<<"graph"<<endl;
    graph.create_mass_vertices();
    graph.print_mass_edges();
    graph.print_mass_vertices();
    input_file_json(graph);
    

    return 0;
}
