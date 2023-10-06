#ifndef NETWORK
#define NETWORK
#include <string>
#include <iostream>
#include <cmath>


struct data{
    float* Output_Values;
    int size;
};

class Node{
    private:
        float* weights;
        float* weights_n;
        float* input;
        float output;
        float sum;
        float der_val;
        std::string fn;
    public:
        int activation_fn();
        int derivative();
};

int Node::activation_fn(){
    
    if(fn == "identity"){
        output = sum;
        return 1;
    }
    else if(fn == "step"){
        output = ((sum>=0)?(1):(0));
        return 1;

    }
    else if(fn == "sigmoid"){
        output = 1.0/(1.0+exp(-1.0*sum));
        return 1;
    }
    else if(fn == "tanh"){
        output = (tanh(sum));
        return 1;
    }
    else if(fn == "arctan"){
        output = (atan(sum));
        return 1;
    }
    else if(fn == "relu"){
        output = ((sum>0)?(sum):(0));
        return 1;
    }
    else{
        return 0;
    }
}

int Node:: derivative(){
    if(fn == "identity"){
        der_val = 1;
        return 1;
    }
    else if(fn == "step"){
        der_val = 0;
        return 1;

    }
    else if(fn == "sigmoid"){
        der_val = output*(1-output);
        return 1;
    }
    else if(fn == "tanh"){
        der_val = 1-output*output;
        return 1;
    }
    else if(fn == "arctan"){
        der_val = 1.0/(1.0+sum*sum);
        return 1;
    }
    else if(fn == "relu"){
        der_val = ((sum>0)?(sum):(0));
        return 1;
    }
    else{
        return 0;
    }
}

class Layer{
    private:
        Layer* next;
        int layer_length;
        Node* nodes;
    public:
        void display();
};

class Network{
    private:
        int num_layers;
        Layer* Start;
        int forward_propogate(float**, int, int, struct data*);
        int back_propogate();
    public:
        int Add_Layer(int, std::string);
        int train(float**, float**, int, int, int); 
        int test(float**, float**, std::string);
};

int Network::Add_Layer(int num_of_Nodes, std::string activation_fn){
    return 0;
}

int Network::train(float** x, float** y, int length, int epochs, int batch_size){
    if(num_layers == 0){
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