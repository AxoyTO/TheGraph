#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
constexpr int MAX_ELEMENT=18;
struct Data{
    int from;
    int edge;
    int to;
};
class VecToString{
    public:
        std::string getVectors(){
            return vectors;
        }
        std::string getEdges(){
            return edges;
        }
        void setVectors(std::array<Data,MAX_ELEMENT>data){
            vectors="";
            int maxIndex= getMaxVecIndex(data);
            for(int i=0;i<=maxIndex;i++){
                vectors+="\t\t{\"id\":"+std::to_string(i)+",";
                vectors+="\"edge_ids\": [";
                for(const auto it:data){
                    if (it.from==i||it.to==i){
                        vectors+=std::to_string(it.edge)+",";
                    }
                }
                vectors.pop_back();
                vectors+="]},\n";
            }
            vectors.pop_back();
            vectors.pop_back();
            vectors+="\n";
        }
        void setEdges(std::array<Data,MAX_ELEMENT>data){
            edges="";
            for(const auto it:data){
                edges+="\t\t{\"id\": "+std::to_string(it.edge);
                edges+=",\"vertex_ids\": ["+std::to_string(it.from)+","+std::to_string(it.to)+"]},\n";
            }
            edges.pop_back();
            edges.pop_back();
            edges+="\n";
        }
        int getMaxVecIndex(std::array<Data,MAX_ELEMENT>data){
            int maxIndex=0;
            for(const auto it:data){
                if (it.from>maxIndex)
                    maxIndex=it.from;
                if(it.to>maxIndex)
                    maxIndex=it.to;
            }
            return maxIndex;
        }
    private:
        std::string vectors;
        std::string edges;
};
class WriteTOFile{
public:
    void openFile(){
        try {
            graph.open("Graphic.json",std::ios::out);
            if (!graph.is_open()){
                throw "can not open file";
            }
        }catch (std::string e){
            std::cerr<<e<<std::endl;
            std::exit(1);
        }
    }
    void write(std::string vectors,std::string edges){
        openFile();
        graph<<"{\n\t\"vertices\": ["<<std::endl;
        graph<<vectors;
        graph<<"\t],\n";
        graph<<"\t\"edges\": ["<<std::endl;
        graph<<edges;
        graph<<"\t]\n}\n";
        graph.close();
    }
private:
    std::ofstream graph;
};
int main(){
    constexpr std::array<Data, MAX_ELEMENT> netWork = {{{0, 0, 1},
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
    VecToString vec;
    vec.setEdges(netWork);
    vec.setVectors(netWork);
    std::cout<<"String generated..."<<std::endl;
    WriteTOFile writeToFile;
    writeToFile.write(vec.getVectors(),vec.getEdges());
    std::cout<<"successfully write to json"<<std::endl;
    return 0;
}