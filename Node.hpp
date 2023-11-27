#ifndef NODE
#define NODE

#include <string>
#include <cmath>
#include <random>
#include <iostream>
#include <time.h>

class Node{
    private:
        int n;
        float* weights;
        float* weights_n;
        float* input;
        float* s_input;
        float* s_weights;
        float der_val;
        float s_der_val;
        std::string fn;
    public:
        float output;
        float sum;
        float s_sum;
        float s_output;
        Node(int, std::string);
        ~Node(){
            delete[] weights;
            delete[] input;
            delete[] weights_n;
        }
        void activation_fn();
        void derivative();
        void forward_propogate(float*);
        void update_weights();
        void record_node();
        float* calculate_err(float);
        void clear();
        void store();
};

Node::Node(int no_inputs, std::string acn){
    n = no_inputs;
    weights = new float[no_inputs];
    input = new float[no_inputs];
    s_input = new float[no_inputs];
    weights_n = new float[no_inputs];
    for(int i=0;i<no_inputs;i++){
        *(weights+i) = (float)(rand()) / (float)(RAND_MAX);
        *(input+i) = 0.0;
        *(weights_n+i) = 0.0;
    }
    fn = acn;
    output = 0.0;
    s_output = 0.0;
    sum = 0.0;
    s_sum = 0.0;
    der_val = 0.0;
    s_der_val = 0.0;
}

void Node::activation_fn(){
    
    if(fn == "identity"){
        output = sum;
    }
    else if(fn == "step"){
        output = ((sum>=0)?(1):(0));
    }
    else if(fn == "sigmoid"){
        output = 1.0/(1.0+exp(-1.0*sum));
    }
    else if(fn == "tanh"){
        output = (tanh(sum));
    }
    else if(fn == "arctan"){
        output = (atan(sum));
    }
    else if(fn == "relu"){
        output = ((sum>0)?(sum):(0));
    }
    return;
}

void Node:: derivative(){
    if(fn == "identity"){
        der_val = 1;
    }
    else if(fn == "step"){
        der_val = 0;
    }
    else if(fn == "sigmoid"){
        der_val = output*(1-output);
    }
    else if(fn == "tanh"){
        der_val = 1-output*output;
    }
    else if(fn == "arctan"){
        der_val = 1.0/(1.0+sum*sum);
    }
    else if(fn == "relu"){
        der_val = ((sum>0)?(sum):(0));
    }
    return;
}

void Node::forward_propogate(float* data){
    for(int i=0;i<n;i++){
        *(input+i) = *(data+i);
        sum += *(weights+i)**(input+i);
    }
    activation_fn();
    return;
}

float* Node::calculate_err(float err){
    float *e = new float[n];
    for(int i=0;i<n;i++){
        *(e+i) = 0;
        derivative();
        *(weights_n+i) += err * der_val * *(input+i);
        *(e+i) = err * der_val * *(weights+i);
    }
    return e;
}

void Node::update_weights(){
    for(int i=0;i<n;i++){
        *(weights+i) += *(weights_n+i);
    }
}

void Node::store(){
    s_sum += sum;
    s_output += output;
    s_der_val += der_val;
    for(int i=0;i<n;i++){
        *(s_input+i) += *(input+i);
    }
}

void Node::clear(){
    sum=0;
    der_val = 0;
    output = 0;
    for(int i=0;i<n;i++){
        *(weights_n+i) = 0;
        *(input + i) = 0;
    }
}

void Node::record_node(){
    std::cout<<"Weights:\n";
    for(int i=0;i<n;i++){
        std::cout<<*(weights+i)<<"\t";
    }
    std::cout<<std::endl;
}

#endif