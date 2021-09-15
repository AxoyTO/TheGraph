#include<iostream>
#include<fstream>
#include<cstdlib>
const int maxDots = 14;
const int maxVecPerDot = 4;
const int maxEdges=18;
typedef struct liner{
    int maindot;
    int net[maxVecPerDot][2];
}liner;

class vecSetUp{
    public:
    void setUp(){
        for(int i=0; i<maxDots;i++) {
        netWork[i].maindot=i;
        for(int j=0; j<maxVecPerDot;j++) {
            netWork[i].net[j][0]=-1;
            netWork[i].net[j][1]=-1;
            }
        }
        std::cout<<"Setup Tree Completed..."<<std::endl;
    }
    bool addFork(int aparm[3]){ //Aparm-> from way to 
        for(int i=0;i<maxDots;i++) {
            if(netWork[aparm[0]].net[i][0]<0){
                netWork[aparm[0]].net[i][0]=aparm[1];
                netWork[aparm[0]].net[i][1]=aparm[2];
                break;
            }
        }
        return true;
    }
    bool printJS(){
        std::ofstream graphics;
        graphics.open("Graphic.json",std::ios::out);
        if(!graphics.is_open()) {
            std::cerr<<"open file or create error"<<std::endl;
        }
        bool beginFlag=true;
        //JS begin
        graphics<<"{\n\t\"vertices\": [\n";
        for(int i=0; i<maxDots;i++) {
            graphics<<"{\n\t\t\t\"id\":"<<netWork[i].maindot<<","<<"\n";
            graphics<<"\t\t\t\"edge_ids\":[";
            beginFlag=true;
            for(int j=0;j<maxVecPerDot;j++){
                if(netWork[i].net[j][0]>=0) {
                    if(beginFlag==true){
                        graphics<<netWork[i].net[j][1];
                        beginFlag=false;
                    }else{
                        graphics<<", "<<netWork[i].net[j][1];
                    }
                }
            }
            graphics<<"]\n";
            if(i==(maxDots-1)){
                graphics<<"\t\t}\n";
            }else{
                graphics<<"\t\t}, ";
            }
        }
        //Edges
        beginFlag=true;
        graphics<<"\t],\n";
        graphics<<"\t\"edges\": [\n";
        for(int i=0; i<maxEdges;i++) {
            for(int j=0; j<maxDots ;j++) {
                for(int k=0; k<maxVecPerDot;k++) {
                    if(i==netWork[j].net[k][0]) { //get Edge ID
                        if(beginFlag==false){
                            graphics<<",";
                        }
                        graphics<<"{\n\t\t\t\"id\": "<<i<<",\n";
                        graphics<<"\t\t\t\"vertex_ids\": ["<<netWork[j].maindot<<","<<netWork[j].net[k][1]<<"]\n\t\t}";
                        beginFlag=false;
                    }
                    
                }
                
            }
            
        }
        graphics<<"\t]";
        //JS end
        graphics<<"\n}";
        return true;
    }
    private:
    liner netWork[maxDots];
};
int main(int argc,char* argv[]){
    vecSetUp vec;
    vec.setUp();
    int vecs[18][3]={{0,0,1},{0,1,2},{0,2,3},{1,3,4},{1,4,5},{1,5,6},{2,6,7},{2,7,8},{3,8,9},{4,9,10},{5,10,10},{6,11,10},{7,12,11},{8,13,11},{9,14,12},{10,15,13},{11,16,13},{12,17,13}};
    for(int i=0; i<18;i++) {
        if(!vec.addFork(vecs[i])){
            std::cerr<<"Error in data: "<<i<<std::endl;
        }
        if(!vec.printJS()){
             std::cerr<<"Error in print"<<i<<std::endl;
        }
    }
    
}