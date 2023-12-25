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
        int in_size[2];             //first element is number of nodes. Second element is the number of inputs to the node.
        int output_size;
        int no_threads;
        int thread_rank;

        void calc_err(float*);
        // void update_weights();
        // void clear_network();

    public:

        Network();
        ~Network(){
            MPI_Finalize();
        }

        int Add_Layer(int, std::string);
        int Add_Layer(int, int, int*, std::string);
        int Add_Input_Layer(int, int*);

        float* forward_propogate(float**);
        // void train(float**, float**, int, int, int, float); 
        // void test(float**, float**, std::string);
        // void batch_train(float**,float**,int,int,float);

        void fit();
        void record_network();
};

Network::Network(){
    no_layers = 0;
    in_dim = 0;
    in_size[0] = 0;
    in_size[1] = 0;
    output_size = 0;
}

void Network::fit(){
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &no_threads);
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);
    return;
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

// void Network::batch_train(float** x, float** y, int base, int batch_size, float lr){
//     float* err = new float[output_size];
//     for(int i=0;i<output_size;i++){
//         *(err+i) = 0;
//     }
//     for(int i=base;i<base+batch_size;i++){
//         float **x_t = new float*[in_size[0]];
//         for(int j=0;j<in_size[0];j++){
//             *(x_t+j) = (*(x+i)+j);
//         }
//         float* temp = forward_propogate(x_t);
//         for(int j=0;j<output_size;j++){
//             std::cout<<"Output: "<<*(temp+j)<<std::endl;
//             if(*(temp+j) == 0){
//                 record_network();
//             }
//             *(temp+j) = *(*(y+i)+j) - *(temp+j);
//             std::cout<<"Error: "<<*(temp+j)<<"\n";
//             *(temp+j) /= batch_size;
//             *(temp+j) *= lr;
//         }
//         calc_err(temp);
//         clear_network();
//         delete x_t;
//         delete temp;
//     }
//     update_weights();
//     return;
// }

// void Network::train(float** x, float** y, int length, int batch_size, int epochs, float lr){
//     while(epochs){
//         epochs--;
//         for(int i=0;i<length/batch_size+1;i++){
//             std::cout<<"base "<<i<<" "<<batch_size*i<<std::endl;
//             if(i == length/batch_size){
//                 batch_train(x,y,batch_size*i, length%batch_size, lr);
//             }
//             else{
//                 batch_train(x,y,batch_size*i,batch_size, lr);
//             }
//         }
//     }
//     return;
// }

void Network::record_network(){
    for(int i=0;i<no_layers;i++){
        layers[i]->record_layer();
    }
}











float* Network::forward_propogate(float** data){
    float* out = NULL;
    for(int i=0;i<no_layers;i++){
        int p = layers[i]->layer_length;
        if(i==0){
            layers[i]->input_propogate(data,no_threads,thread_rank);
        }
        else{
            layers[i]->forward_propogate(layers[i-1]->output,no_threads,thread_rank);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if(thread_rank==0){
            for(int j=0;j<p;j++){
                std::cout<<*(layers[i]->output+j)<<"\t";
            }
            std::cout<<std::endl;
        }
        if(i==no_layers-1){
            out = new float[layers[i]->layer_length];
            for(int i=0;i<layers[i]->layer_length;i++){
                *(out+i) = *(layers[i]->output + i);
            }
        }
    }
    return out;
}





// void Network::clear_network(){
//     for(int i=0;i<no_layers;i++){
//         layers[i]->clear_layer();
//     }
// }



// void Network::calc_err(float* err){
//     for(int i=no_layers-1;i>=0;i--){
//         err = layers[i]->calc_err(err);
//     }
//     return;
// }



// void Network::update_weights(){
//     for(int i=0;i<no_layers;i++){
//         layers[i]->update_weights();
//     }
// }

#endif