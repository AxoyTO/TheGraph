#include <memory>
#include <cstdlib>
#include <iostream>
#include <fstream>

struct edge {
    static int edge_id_max;
    int edge_id;
    int to_vertice;
    std::shared_ptr<struct vertice> ptr;

    edge() {}

    edge(int to_vertice_, bool again) {
        if (again) {
            edge_id = edge_id_max - 1;
        }
        else {
            edge_id = edge_id_max;
            edge_id_max++;
        }
        to_vertice = to_vertice_;
    }
};

int edge::edge_id_max = 0;

struct vertice {
    static int vertice_id_max;
    int id;
    int depth;
    std::unique_ptr<struct edge[]> edges_from_ptr;
    std::unique_ptr<struct edge> parent_edge_ptr;
    int edges_from_num;

    //How many new edges to generate from current vertice? 0..3
    int num_generate() {
        int res;
        if (this->depth == 1) return 3;
        int x = std::rand()/((RAND_MAX + 1u)/10);
        if ((this->depth == 2) && (x < 2)) return x + 2;
        if (x <= this->depth) return 0;
        int y = std::rand()/((RAND_MAX + 1u)/3);
        if (y == 2) res = 3;
        else if (y == 1) res = 2;
        else res = 1;
        return res;
    }
    
    vertice(int depth_, int parent_id) {
        id = vertice_id_max;
        vertice_id_max++;
        depth = depth_;

        if (depth == 1) {
            parent_edge_ptr.reset(nullptr);
        }
        else {
            parent_edge_ptr.reset(new struct edge(parent_id, true));
        }
        edges_from_num = num_generate();

        if (edges_from_num == 0) {
            edges_from_ptr.reset(nullptr);
        }
        else {
            edges_from_ptr.reset(new struct edge[edges_from_num]);
            for (int i=0; i < edges_from_num; i++) {
                edges_from_ptr[i] = edge(vertice_id_max, false);
                std::shared_ptr<struct vertice> new_vertice(
                        new struct vertice(depth + 1, id)
                    );
                edges_from_ptr[i].ptr = new_vertice;
            }
        }
    }

    void to_json_vert(std::ofstream& file) {
        file <<  "{\n\t\t\t\"id\": " << this->id << ",\n\t\t\t\"edge_ids\": [";
        if (this->edges_from_num > 1) {
            for (int i=0; i < this->edges_from_num - 1; i++) {
                file << this->edges_from_ptr[i].edge_id << ", ";
            }
            file << this->edges_from_ptr[this->edges_from_num - 1].edge_id;
        }
        else if (this->edges_from_num == 1) {
            file << this->edges_from_ptr[0].edge_id;
        }
        file << "]\n\t\t}";

        if (this->edges_from_num > 0) {
            file << ", ";
            if (this->edges_from_num > 1) {
                for (int i=0; i < this->edges_from_num - 1; i++) {
                    (*this->edges_from_ptr[i].ptr).to_json_vert(file);
                    file << ", ";
                }
                (*this->edges_from_ptr[this->edges_from_num - 1].ptr).to_json_vert(file);
            }
            else (*this->edges_from_ptr[0].ptr).to_json_vert(file);
        }
    }

    void to_json_edge(std::ofstream& file) {
        if (this->edges_from_num > 0) {
            if (this->edges_from_num > 1) {
                for (int i=0; i < this->edges_from_num - 1; i++) {
                    file << "{\n\t\t\t\"id\": " << this->edges_from_ptr[i].edge_id;
                    file << ",\n\t\t\t\"vertex_ids\": [" << this->id << ", ";
                    file << this->edges_from_ptr[i].to_vertice << "]\n\t\t}, ";
                }
            }
            file << "{\n\t\t\t\"id\": " << this->edges_from_ptr[this->edges_from_num - 1].edge_id;
            file << ",\n\t\t\t\"vertex_ids\": [" << this->id << ", ";
            file << this->edges_from_ptr[this->edges_from_num - 1].to_vertice << "]\n\t\t}";

            if (this->edges_from_num > 0) {
                bool has_grandkids = false;
                for (int i=0; i < this->edges_from_num; i++) {
                    if ((*this->edges_from_ptr[i].ptr).edges_from_num > 0) {
                        has_grandkids = true;
                    }
                }
                if (has_grandkids) {
                    file << ", ";
                }
            }

            if (this->edges_from_num > 1) {
                for (int i=0; i < this->edges_from_num - 1; i++) {
    
                    if ((*this->edges_from_ptr[i].ptr).edges_from_num > 0) {
                        (*this->edges_from_ptr[i].ptr).to_json_edge(file);
                        bool other_has_grandkids = false;
                        for (int j=i+1; j < this->edges_from_num; j++) {
                            if ((*this->edges_from_ptr[j].ptr).edges_from_num > 0) {
                                other_has_grandkids = true;
                            }
                        }
                        if (other_has_grandkids) {
                            file << ", ";
                        }
                    }
                }
                if ((*this->edges_from_ptr[this->edges_from_num - 1].ptr).edges_from_num > 0) {
                    (*this->edges_from_ptr[this->edges_from_num - 1].ptr).to_json_edge(file);
                }
            }
            else if ((*this->edges_from_ptr[0].ptr).edges_from_num > 0) {
                (*this->edges_from_ptr[0].ptr).to_json_edge(file);
            }
        }
    }

    void to_json(std::ofstream& file) {        
        file << "{\n\t \"vertices\": [\n\t\t";
        this->to_json_vert(file);
        file << "\n\t],\n\t\"edges\": [\n\t\t";
        this->to_json_edge(file);
        file << "\n\t]\n}";
    }
};

int vertice::vertice_id_max = 0;

int main() {
    std::shared_ptr<struct vertice> graph_root_ptr(new struct vertice(1, -1));

    std::ofstream myfile;
    myfile.open("graph.json");
    (*graph_root_ptr).to_json(myfile);
    myfile.close();

    return 0;
}
