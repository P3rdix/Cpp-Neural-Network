#ifndef LAYER
#define LAYER

#include "Node.hpp"
#include <vector>
#include <exception>

class Unknown_Activation_Function_Exception: public std::exception{
    public:
        Unknown_Activation_Function_Exception(std::string s){
            std::cerr<<"Error Occured"<<std::endl;
            std::cerr<<"Unknown activation function "<<s<< " found.\n";
            std::cerr<<"Please input a valid activation function.\n";
            std::cerr<<"Valid functions include:\n"<<"identity\n"<<"step\n"<<"sigmoid\n"<<"tanh\n"<<"arctan\n"<<"relu\n";
            std::abort();
        }
};

class Layer{
    private:
        Layer* next;
        std::vector<Node*> nodes;
        int no_inputs;
    public:
        int layer_length;
        Layer(int,int,std::string);
        void record_layer();
        ~Layer(){
            for(int i=layer_length-1;i>=0;i--){
                Node *temp = nodes[i];
                delete temp;
                nodes.pop_back();
            }
        }
        float* forward_propogate(float**);
        float* back_propogate(float*);
};

Layer::Layer(int no_nodes, int input_to_node, std::string acn){
    layer_length = no_nodes;
    no_inputs = input_to_node;
    next = NULL;
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
        throw Unknown_Activation_Function_Exception(acn);
    }
    else{
        for(int i=0;i<layer_length;i++){
            nodes.emplace_back(new Node(input_to_node, acn));
        }
    }
}

void Layer::record_layer(){
    std::cout<<"Next Layer: "<<next<<std::endl;
    std::cout<<"Number of nodes: "<<layer_length<<std::endl;
    for(int i(0);i<layer_length;i++){
        nodes[i]->record_node();
    }
    return;
}

float* Layer::forward_propogate(float** data){
    float* output = new float[layer_length];
    for(int i=0;i<layer_length;i++){
        nodes[i]->forward_propogate(*(data+i));
        *(output+i) = nodes[i]->output;
    }
    return output;
}

float* Layer::back_propogate(float* error){
    float* n_err = new float[no_inputs];
    for(int i=0;i<no_inputs;i++){ *(n_err+i) = 0;};
    for(int i=0;i<layer_length;i++){
        float* temp = nodes[i]->calculate_err(*(error+i));
        nodes[i]->update_weights();
        for(int j=0;j<no_inputs;j++){
            *(n_err+j) = *(temp+j);
        }
    }
    return n_err;
}

#endif