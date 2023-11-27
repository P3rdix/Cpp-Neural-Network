#ifndef NETWORK
#define NETWORK

#include "Layer.hpp"
#include <exception>

/*
Notes:

* Add_layer(4 param) should NOT have an increment on layers since incrementation already occurs in Add_layer(2 param) and Add_Input_Layer
* Add exceptions for 0 nodes in any add layer.
* Add bias variables
* In forward propogation, add a checking value for the number of dims of data to the number of nodes in input
* 
*/


class Network{
    private:
        int no_layers;
        std::vector<Layer*> layers;
        int in_dim;
        int in_size[2]; //first element is number of nodes. Second element is the number of inputs to the node.
        int output_size;
        int back_propogate(float*);
        float* forward_propogate(float**);

    public:
        int Add_Layer(int, std::string);
        int Add_Layer(int, int, int*, std::string);
        int Add_Input_Layer(int, int*);
        void train(float**, float**, int, int, int); 
        void test(float**, float**, std::string);
        void record_network();
        void batch_train(float**,float**,int,int);
        Network();
};

Network::Network(){
    no_layers = 0;
    in_dim = 0;
    in_size[0] = 0;
    in_size[1] = 0;
    output_size = 0;
}

int Network::Add_Layer(int num_of_Nodes, std::string activation_fn){
    if(!no_layers){
        return 1;
    }
    else{
        try{
            layers.emplace_back(new Layer(num_of_Nodes,layers[no_layers-1]->layer_length,activation_fn));
            no_layers++;
            output_size = num_of_Nodes;
        } catch(Unknown_Activation_Function_Exception e){
            // write code to check if the new layer was incorrectly initialized, and delete it if necessary
            return 1;
        }
    }
    return 0;
}

int Network::Add_Input_Layer(int input_dim, int* input_size){
    if(!input_dim){
        std::cout<<"Error: input dimension is "<<input_dim<<". Input dimension must be greater than 1\n";
        return 1;
    }
    if(input_dim == 1){
        layers.emplace_back(new Layer(*(input_size),1,"identity"));
        in_size[1] = 1;
    }
    else if(input_dim >= 2){
        std::cout<<"Undefined";
        return 1;
    }
    in_dim = input_dim;
    in_size[0] = *(input_size);
    output_size = *(input_size);
    no_layers++;
    return 0;
}

int Network::Add_Layer(int no_nodes, int input_dim, int* input_size, std::string activation_function){
    if(!no_layers){
        int i = this->Add_Input_Layer(input_dim, input_size);
        if(!i){
            no_layers++;
            in_dim = input_dim;
            in_size[0] = *(in_size);
            (in_dim == 1)?(in_size[1] = 1):(in_size[1] = *(in_size+1)); 
            int j = this->Add_Layer(no_nodes,activation_function);
            if(!j){
                return 0;
            }
            no_layers--;
        }
        return 1;
    }
    int i = this->Add_Layer(no_nodes, activation_function);
    if(!i){
        no_layers++;
        return 0;
    }
    return 1;
}

void Network::batch_train(float** x, float** y, int base, int batch_size){
    float* err = new float[output_size];
    for(int i=0;i<output_size;i++){
        *(err+i) = 0;
    }
    for(int i=base;i<base+batch_size;i++){
        float **x_t = new float*[in_size[0]];
        for(int j=0;j<in_size[0];j++){
            *(x_t+j) = (*(x+i)+j);
        }
        float* temp = forward_propogate(x_t);
        for(int j=0;j<output_size;j++){
            *(err+j) += *(temp+j);
        }
        delete x_t;
        delete temp;
    }
    for(int i=0;i<output_size;i++){
        *(err+i) /= batch_size;
    }
    back_propogate(err);
    return;
}

void Network::train(float** x, float** y, int length, int batch_size, int epochs){
    while(epochs){
        epochs--;
        for(int i=0;i<length/batch_size+1;i++){
            std::cout<<"base "<<i<<" "<<batch_size*i<<std::endl;
            if(i == length/batch_size){
                batch_train(x,y,batch_size*i, length%batch_size);
            }
            else{
                batch_train(x,y,batch_size*i,batch_size);
            }
        }
    }
    return;
}

void Network::record_network(){
    for(int i=0;i<no_layers;i++){
        layers[i]->record_layer();
    }
}

float* Network::forward_propogate(float** data){
    float** temp = data;
    float* out = NULL;
    for(int i=0;i<no_layers;i++){
        float* n = layers[i]->forward_propogate(temp);
        if(i!=no_layers-1){
            int y = layers[i+1]->layer_length;
            int p = layers[i]->layer_length;
            if(i!=0){
                delete *(temp);
                delete temp;
            }
            temp = new float*[y];
            for(int j=0;j<y;j++){
                temp[j] = n;
            }
        }
        else{
            out = n;

        }
    }
    for(int i=0;i<no_layers;i++){
        layers[i]->store();
        layers[i]->clear();
    }
    return out;
}

int Network::back_propogate(float* x){
    return 0;
}

#endif