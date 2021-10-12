#include <array>
#include <fstream>
#include <iostream>
#include <string>
constexpr int MAX_ELEMENT=18;
class StrGenerater{
    public:
        std::string getVectors(){
            return vectors;
        }
        std::string getEdges(){
            return edges;
        }
        void setVectors(std::array<std::array<int,3>,MAX_ELEMENT>data){
            vectors="";
            int maxIndex= getMaxVecIndex(data);
            for(int i=0;i<=maxIndex;i++){
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
        int getMaxVecIndex(std::array<std::array<int,3>,MAX_ELEMENT>data){
            int maxIndex=0;
            for(const auto it:data){
                if (it.at(0)>maxIndex)
                    maxIndex=it.at(0);
                if(it.at(2)>maxIndex)
                    maxIndex=it.at(2);
            }
            return maxIndex;
        }
    private:
        std::string vectors;
        std::string edges;
};
class FileWriter{
public:
    void write(std::string vectors,std::string edges){
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
    std::ofstream  writePT;
};
int main(){
    const std::array<std::array<int,3>, MAX_ELEMENT> netWork = {{{0, 0, 1},
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
    StrGenerater vec;
    vec.setEdges(netWork);
    vec.setVectors(netWork);
    std::cout<<"String generated..."<<std::endl;
    FileWriter writeToFile;
    writeToFile.write(vec.getVectors(),vec.getEdges());
    std::cout<<"successfully write to json"<<std::endl;
    return 0;
}