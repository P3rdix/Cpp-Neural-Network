#ifndef LAYER
#define LAYER

#include "Node.hpp"

class Layer{
    private:
        Layer* next;
        int layer_length;
        Node* nodes;
    public:
        void display();
};

#endif