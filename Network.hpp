#ifndef NETWORK
#define NETWORK

#include "Layer.hpp"
#include <exception>

struct data{
    float* Output_Values;
    int size;
};

class Network{
    private:
        int no_layers;
        std::vector<Layer*> layers;
        int forward_propogate(float**, int, int, struct data*);
        int back_propogate();
    public:
        int Add_Layer(int, std::string);
        int Add_Layer(int, int, int*, std::string);
        int Add_Input_Layer(int, int*);
        int train(float**, float**, int, int, int); 
        int test(float**, float**, std::string);
        Network();
};

Network::Network(){
    no_layers = 0;
}

int Network::Add_Layer(int num_of_Nodes, std::string activation_fn){
    if(!no_layers){
        return 1;
    }
    else{
        try{
            layers.emplace_back(new Layer(num_of_Nodes,layers[no_layers-1]->layer_length,activation_fn));
        } catch(Unknown_Activation_Function_Exception e){
            return 1;
        }
    }
    return 0;
}

int Network::Add_Input_Layer(int input_dim, int* input_size){
    if(input_dim == 1){
        layers.emplace_back(new Layer(*(input_size),1,"identity"));
    }
    else if(input_dim == 2){
        layers.emplace_back(new Layer(*(input_size), *(input_size+1),"identity"));
    }
    else{
        return 1;
    }
    return 0;
}

int Network::Add_Layer(int no_nodes, int input_dim, int* input_size, std::string activation_function){
    if(no_layers){
        int r = this->Add_Layer(no_nodes, activation_function);
        return r;
    }
    int a = this->Add_Input_Layer(input_dim, input_size);
    if(a){
        a = this->Add_Layer(no_nodes, activation_function);
        return a;
    }

}

int Network::train(float** x, float** y, int length, int epochs, int batch_size){
    if(no_layers == 0){
        std::cerr<<"The network is incomplete. Please Add layers to it before running\n";
        return -1;
    }
    for(int i=0;i<length/batch_size;i++){
        struct data *output = (struct data*)malloc(sizeof(struct data));
        forward_propogate(x, batch_size*i, (i == length/batch_size - 1)?(length%batch_size):(batch_size), output );
        back_propogate();
    }
    
    return 0;
}

int Network::test(float** x, float** y, std::string metric){
    return 0;
}

int Network::forward_propogate(float** data, int base, int batch_size, struct data* out){

    for(int i=base;i<base+batch_size;i++){
        std::cout<<"Hello";
    }
    return 0;
}

int Network::back_propogate(){
    return 0;
}

#endif