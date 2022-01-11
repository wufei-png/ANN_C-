

#include "utils.h"
#include "bpnet.h"
#include <cstring>
bpNet::bpNet(int numInputs, double learningRate)//构造函数
    :num_inputs(numInputs),
    num_hiddenlayers(0),
    learningrate(learningRate),
    sum_error(0)
{
}

bpNet::~bpNet()//析取函数
{
    for (int i = 0; i < layers.size(); i++)
    {
        if (layers[i])
        {
            delete layers[i];
        }
    }
}


void bpNet::addLayer(int num_nodes)
{
    int pernode_input_num = (layers.size() > 0) ? layers[num_hiddenlayers]->num_of_nodes : num_inputs;//若没添加过，输入数量为一个图像的像素点数量，相当于batch size为1，若有了隐藏层，为上一层的隐藏层节点数目
    layers.push_back(new Layer(num_nodes, pernode_input_num));
    num_hiddenlayers = layers.size() - 1;
   // cout<<"num_hiddenlayers:"<<num_hiddenlayers<<endl;
}


double bpNet::sigmoid(double x, double parameter)
{
    // sigmoid function: f(x) = 1 /(1 + exp(-x))
    return (1.0 / (1.0 + exp(-x * parameter)));//修改parameter可以改变激活函数图像斜率/形状
}

void bpNet::forward_oneLayer(Layer& now_layer, const double inputs[])//数据前向传播一层
{
    int num_nodes = now_layer.num_of_nodes;
    int pernode_input_num = now_layer.pernode_input_num;
    double* output = now_layer.outputs;//结果直接写回给outputs

    for (int n = 0; n < num_nodes; ++n)//对于第i层的每个节点
    {
        double* weights = now_layer.weights[n];
        double midvalue = 0;
        int k;
        for (k = 0; k < pernode_input_num; ++k)
        {
            midvalue += weights[k] * inputs[k];//这里的inputs[k]对每个结点都一样，但是weights[k]，weights是二维的，每次迭代都不同
        }
        midvalue += weights[k] ;//这里最后一个参数视为bias,也是可以学习的
       output[n] = sigmoid(midvalue, SIGMOID_PARAMETER);//经过激活，最后输出
    }
}

void bpNet::forward_all_layers(const double inputs[], double* outputs[])//测试时候使用
{
    for (int i = 0; i < num_hiddenlayers + 1; i++)
    {
        forward_oneLayer(*layers[i], inputs);
        inputs = layers[i]->outputs;
    }
    *outputs = layers[num_hiddenlayers]->outputs;
}



void bpNet::backward_onelayer(Layer& now_layer, const double prevOutputs[], double prevOutErrors[],int j)
{//https://www.cnblogs.com/jsfantasy/p/12177275.html
    int num_nodes = now_layer.num_of_nodes;
    int pernode_input_num = now_layer.pernode_input_num;
    double* errors = now_layer.errors;
    double* outputs = now_layer.outputs;

    for (int i = 0; i < num_nodes; i++)
    {
        double* weights = now_layer.weights[i];

        double err = errors[i] *outputs[i] * (1 - outputs[i]);

        int w;

        for (w = 0; w < pernode_input_num; w++)
        {   //cout<<"prevOutErrors"<<prevOutErrors;
            if (j>0)
            {//cout<<"prevOutErrors"<<prevOutErrors;
                prevOutErrors[w] += weights[w] * err;
            }	

            weights[w] += learningrate * err * prevOutputs[w];
        }

        weights[w] += err * learningrate ;
    }
}

void bpNet::forward(const double inputs[], const double targets[])//前向传播并计算输出层误差
{
    for (int i = 0; i < num_hiddenlayers + 1; i++)
    {
        forward_oneLayer(*layers[i], inputs);
        inputs = layers[i]->outputs;
    }
    Layer& outLayer = *layers[num_hiddenlayers];//别名
    sum_error = 0;
    for (int i = 0; i < outLayer.num_of_nodes; i++)
    {
        //double err =  outActivations[i] - targets[i];
        double err = targets[i] - outLayer.outputs[i];//onehot编码
        outLayer.errors[i] = err;
        sum_error += err * err;//平方和
    }
}

void bpNet::backward(const double inputs[]) {
    const double* prevOutputs = NULL;//上一层的输入反向传播不需要改
    double* prevOutErrors = NULL;//上一层的loss要改
    for (int i = num_hiddenlayers; i >= 0; i--) {
        if (i > 0) {
            Layer &prev = *layers[(i - 1)];
            prevOutputs = prev.outputs;
            prevOutErrors = prev.errors;
            memset(prevOutErrors, 0, prev.num_of_nodes * sizeof(double));//清零，接下来调用是+=

        } else {
            prevOutputs = inputs;
            prevOutErrors = NULL;
        }

        backward_onelayer(*layers[i], prevOutputs, prevOutErrors,i);
    }
}
bool bpNet::train_oneimage(const double inputs[], const double targets[])
{
    forward(inputs, targets);
    backward(inputs);
    return true;
}



