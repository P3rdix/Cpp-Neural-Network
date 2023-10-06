#ifndef NODE
#define NODE

#include <string>
#include <cmath>
#include <random>
#include <iostream>

class Node{
    private:
        int n;
        float* weights;
        float* weights_n;
        float* input;
        float output;
        float sum;
        float der_val;
        std::string fn;
    public:
        Node(int, std::string);
        int activation_fn();
        int derivative();
        void print_vars(){
            for(int i=0;i<n;i++){
                std::cout<<*(weights+i)<<"\t"<<*(input+i)<<"\n";
            }
        }
        ~Node(){
            delete[] weights;
            delete[] input;
        }
};

Node::Node(int no_inputs, std::string acn){
    n = no_inputs;
    weights = new float[no_inputs];
    input = new float[no_inputs];
    for(int i=0;i<no_inputs;i++){
        *(weights+i) = (float)(rand()) / (float)(RAND_MAX);
        *(input+i) = 0.0;
    }
    fn = acn;
    weights_n = NULL;
    output = 0.0;
    sum = 0.0;
    der_val = 0.0;
    
}

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

#endif