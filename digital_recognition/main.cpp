

#include <iostream>
#include <time.h>
#include <cstring>
#include "bpnet.h"
#include <chrono>
#include "data_input.h"
#include "utils.h"
using namespace chrono;

#define NUM_NET_OUT 10
#define NUM_NODES_PER_LAYER 500
#define NET_LEARNING_RATE 0.4 //超参数

#define TRAIN_IMAGES_URL "D:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition\\dataset\\train-images.idx3-ubyte"
#define TRAIN_LABELS_URL "D:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition\\dataset\\train-labels.idx1-ubyte"
#define TEST_IMAGES_URL "D:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition\\dataset\\t10k-images.idx3-ubyte"
#define TEST_LABELS_URL  "D:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition\\dataset\\t10k-labels.idx1-ubyte"
//G:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition_with_neuron_network-master\\digital_recognition_with_neuron_network-master\\dataset\\train-images.idx3-ubyte
//#define TRAIN_IMAGES_URL "G:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition_with_neuron_network-master\\digital_recognition_with_neuron_network-master\\dataset\\train-images.idx3-ubyte"
//#define TRAIN_LABELS_URL "G:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition_with_neuron_network-master\\digital_recognition_with_neuron_network-master\\dataset\\train-labels.idx1-ubyte"
//
//#define TEST_IMAGES_URL "G:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition_with_neuron_network-master\\digital_recognition_with_neuron_network-master\\dataset\\t10k-images.idx3-ubyte"
//#define TEST_LABELS_URL  "G:\\onedrive\\OneDrive - sjtu.edu.cn\\c++files\\digital_recognition_with_neuron_network-master\\digital_recognition_with_neuron_network-master\\dataset\\t10k-labels.idx1-ubyte"


void printNumber(unsigned char input[], int width, int height)
{
    int index = 0;
    for (int i=0; i < height; i++)
    {
        for (int j = 0; j < width; j++ )
        {

            if (input[index++])
            {
                cout << "*";
            }
            else
            {
                cout << " ";
            }
        }

        cout << endl;
    }
}

void preProcessData(const unsigned char trainData[], double out[], int size)//预处理数据，转换为01
{
    for (int i = 0; i < size; i++)
    {
        out[i] = (trainData[i] >= 128) ? 1.0 : 0.0;//256/2
    }
}

void preProcessDataWithNoise(const unsigned char trainData[], double out[], int size)//加一个噪声
{
    for (int i = 0; i < size; i++)
    {
        out[i] = ((trainData[i] >= 128) ? 1.0 : 0.0) + RandFloat() * 0.1;//防止过拟合
    }
}

double train_one_Epoch(dataInput& trainData, bpNet& bpnet, int imageSize, int numImages)
{
    double net_target[NUM_NET_OUT];
    char* temp = new char[imageSize];
    double* net_train = new double[imageSize];
    for (int i = 0; i < numImages; i++)
    {
        int label = 0;
        memset(net_target, 0, NUM_NET_OUT * sizeof(double));
        if (trainData.read((char*) &label, temp))
        {
            net_target[label] = 1.0;//onehot编码
            preProcessDataWithNoise((unsigned char*)temp, net_train, imageSize);
            bpnet.train_oneimage(net_train, net_target);

        }
        else
        {
            cout << "cannot read train data " << endl;
            break;
        }
    }
    cout << "the error is:" << bpnet.getError() << " after train_oneimage " << endl;
    delete []net_train;
    delete []temp;
    return bpnet.getError();
}

int test_correct(dataInput& testData, bpNet& bpnet, int imageSize, int numImages)
{
    int correct = 0;
    double* net_out = NULL;
    char* src = new char[imageSize];
    double* net_input = new double[imageSize];
    for (int i = 0; i < numImages; i++)
    {
        int label = 0;
        if (testData.read((char*) &label, src))
        {			
            preProcessData((unsigned char*)src, net_input, imageSize);//测试的时候不用对输入数据加噪声
            bpnet.forward_all_layers(net_input, &net_out);
            int index = 0;
            double max_value=0;
            for (int i = 0; i < NUM_NET_OUT; i++)//找到最大的数，将其index视为模型输出结果
            {
                if (i == 0)max_value = net_out[i];
                else if (net_out[i] > max_value)
                {
                    max_value = net_out[i];
                    index = i;
                }
            }
            if (index == label)
            {
                correct++;
            }
        }
        else
        {
            cout << "cannot read test data" << endl;
            break;
        }
    }
    delete []net_input;
    delete []src;
    return correct;
}

int main()
{
    dataInput trainData;
    dataInput testData;
    bpNet* bpnet = NULL;
    if (trainData.openImageFile(TRAIN_IMAGES_URL,0) && trainData.openLabelFile(TRAIN_LABELS_URL,0))
    {
        int imageSize = 28*28;
        int numImages = 60000;
        int epochtime = 1;//超参数可修改

//        char* temp = new char[imageSize];//可视化数字
//        for (size_t i = 0; i < 2; i++)
//        {
//            if (trainData.readImage(temp))
//            {
//                printNumber((unsigned char*)temp, trainData.imageWidth(), trainData.imageHeight());
//            }
//        }


        bpnet = new bpNet(imageSize, NET_LEARNING_RATE);
        bpnet->addLayer(NUM_NODES_PER_LAYER);//修改网络结构
//        bpnet->addLayer(NUM_NODES_PER_LAYER);
//        bpnet->addLayer(NUM_NODES_PER_LAYER);
//        bpnet->addLayer(NUM_NODES_PER_LAYER);
        //cout<<"bpnet.num_hiddenlayers<<bpnet.num_hiddenlayers;

        bpnet->addLayer(NUM_NET_OUT);
        cout << "start train dpnet..." << endl;
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        for (int i = 0; i < epochtime; i++)
        {
            double err = train_one_Epoch(trainData, *bpnet, imageSize, numImages);
            trainData.reset();
        }

        std::chrono::steady_clock::time_point  t2 = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast<seconds>(t2 - now);
        cout << "cost time:" << int(time_span.count()) << "seconds" ;

        if (testData.openImageFile(TEST_IMAGES_URL,1) && testData.openLabelFile(TEST_LABELS_URL,1))
        {
//            cout<<"进去了";
            imageSize = 28*28;
            numImages = 10000;
            cout << "start test ..." << endl;
            int correct = test_correct(testData, *bpnet, imageSize, numImages);
            cout <<"Testset result:"<<  "ok_cnt: " << correct << ", total: " << numImages << endl;
        }
        else
        {
            cout << "open test image file failed" << endl;
        }
    }
    else
    {
        cout << "open train image file failed" << endl;
    }

    delete bpnet;
    return 0;
}