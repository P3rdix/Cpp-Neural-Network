#ifndef NODE
#define NODE

#include <string>
#include <cmath>

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

#endif