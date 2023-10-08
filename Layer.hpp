#ifndef LAYER
#define LAYER

#include "Node.hpp"
#include <vector>

class Layer{
    private:
        Layer* next;
        int layer_length;
        std::vector<Node*> nodes;
    public:
        Layer(int,int,std::string);
        void display();
        ~Layer(){
            for(int i=layer_length-1;i>=0;i--){
                Node *temp = nodes[i];
                delete temp;
                nodes.pop_back();
            }
        }
};

Layer::Layer(int no_nodes, int input_to_node, std::string acn){
    layer_length = no_nodes;
    for(int i=0;i<acn.length();i++){
        acn[i] = (char)tolower(acn[i]);
    }
    int i=0;
    std::string fn_set[] = {"identity","step","sigmoid","tanh","arctan","relu"};
    for(;i<6;i++){
        if(acn == fn_set[i]){
            i = -1;
            break;
        }
    }
    if(i!=-1){
        throw -1;
    }
    else{
        for(int i=0;i<layer_length;i++){
            nodes.emplace_back(new Node(input_to_node, acn));
        }
    }
    next = NULL;
}

void Layer::display(){
    std::cout<<"Next Node: "<<next<<std::endl;
    std::cout<<"Number of nodes: "<<layer_length<<std::endl;
    for(int i(0);i<layer_length;i++){
        nodes[i]->record_weights();
    }
    return;
}

#endif