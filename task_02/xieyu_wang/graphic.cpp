//Tow Idea of mine IDK which one is IPO so need your help....
/**
 * solution image 1 :
 * class graph:
 *      func 1. read arrays and Classification
 *      func 2. make it to string
 *          possible trouble:
 *              different string "edge_ids" and "vertex_ids"
 *                  give it param when called it ?
 *      func 3. write it to file
 */
 //----------------------------------------------------------
 /**
  * solution image 2:(my Present code)
  * class Graph:
  *     func 1. read and trans vectors to string
  *     func 2. read and trans edges to string
  *     func 3. write to file
  */
 //--------------------------------------------------------
 /**
  * questions in comment:
  * 1)why vector func is 1d?
  * 2)what the main idea of the two func?(I means IO)
  */
// class Vertex {
// public:
//     const int id = 0;
//
//     Vertex(int _id) : id(_id) {}
//
//     void add_edge_id(int id) {
//         // validation that id has not been added already
//         edge_ids_.push_back(id);
//     }
//
// private:
//     std::vector<int> edge_ids_;
// };
//class Edge {
//public:
//    const int id = 0;
//    const int from_vertex_id = 0;
//    const int from_edge_id = 0;
//
//    Edge(int _id, int _from_vertex_id, int _to_vertex_id) : id(_id), from_vertex_id(_from_vertex_id), to_vertex_id(_to_vertex_id) {}
//};

#include <array>
#include <fstream>
#include <iostream>
#include <string>
constexpr int MAX_ELEMENT=18;
constexpr int MAX_VECTOR=13;
class Graph{
public:
        void setVectors(std::array<std::array<int,3>,MAX_ELEMENT>data){
            vectors="";
            for(int i=0;i<=MAX_VECTOR;i++){
                vectors+="\t\t{\"id\":"+std::to_string(i)+",";
                vectors+="\"edge_ids\": [";
                for(const auto it:data){
                    if (it.at(0)==i||it.at(2)==i){
                        vectors+=std::to_string(it.at(1))+",";
                    }
                }
                vectors.pop_back();
                vectors+="]},\n";
            }
            vectors.pop_back();
            vectors.pop_back();
            vectors+="\n";
        }
        void setEdges(std::array<std::array<int,3>,MAX_ELEMENT>data){
            edges="";
            for(const auto it:data){
                edges+="\t\t{\"id\": "+std::to_string(it.at(1));
                edges+=",\"vertex_ids\": ["+std::to_string(it.at(0))+","+std::to_string(it.at(2))+"]},\n";
            }
            edges.pop_back();
            edges.pop_back();
            edges+="\n";
        }
    void write(){
        std::ofstream  writePT;
        writePT.open("Graphic.json",std::ios::out);
        writePT<<"{\n\t\"vertices\": ["<<std::endl;
        writePT<<vectors;
        writePT<<"\t],\n";
        writePT<<"\t\"edges\": ["<<std::endl;
        writePT<<edges;
        writePT<<"\t]\n}\n";
        writePT.close();
        }
    private:
        std::string vectors;
        std::string edges;
};
int main(){
    const std::array<std::array<int,3>, MAX_ELEMENT> netWork = {{
                                                            {0, 0, 1},
                                                           {0, 1, 2},
                                                           {0, 2, 3},
                                                           {1, 3, 4},
                                                           {1, 4, 5},
                                                           {1, 5, 6},
                                                           {2, 6, 7},
                                                           {2, 7, 8},
                                                           {3, 8, 9},
                                                           {4, 9, 10},
                                                           {5, 10, 10},
                                                           {6, 11, 10},
                                                           {7, 12, 11},
                                                           {8, 13, 11},
                                                           {9, 14, 12},
                                                           {10, 15, 13},
                                                           {11, 16, 13},
                                                           {12, 17, 13}}};
    Graph vec;
    vec.setEdges(netWork);
    vec.setVectors(netWork);
    std::cout<<"String generated..."<<std::endl;
    vec.write();
    std::cout<<"successfully write to json"<<std::endl;
    return 0;
}