

#ifndef __BPNET_H__
#define __BPNET_H__

#include <vector>

#include "layer.h"

using namespace std;

#define SIGMOID_PARAMETER 0.7

class bpNet
{
public:
    bpNet(int numInputs, double learningRate);
    ~bpNet();
public:
    inline double getError(void) { return sum_error; }
    void addLayer(int num_nodes);
    bool train_oneimage(const double inputs[], const double targets[]);
    void forward_all_layers(const double inputs[], double* outputs[]);

private:

    inline double sigmoid(double activation, double response);
    void forward_oneLayer(Layer& now_layer, const double inputs[]);
    void forward(const double inputs[], const double targets[]);
    void backward(const double inputs[]);
    void backward_onelayer(Layer& now_layer,  const double prevOutActivations[], double prevOutErrors[],int j);


private:
    int num_inputs;
    int num_hiddenlayers;
    double learningrate;
    double sum_error;
    vector<Layer*> layers;
};



#endif
