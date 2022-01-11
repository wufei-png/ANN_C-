

#include "utils.h"
#include "layer.h"

using namespace std;
Layer::Layer(int num_nodes, int pernode_input_num)
	:num_of_nodes(num_nodes),
	pernode_input_num(pernode_input_num)
{
	weights = new double*[num_of_nodes];
	outputs = new double[num_of_nodes];
	errors = new double[num_of_nodes];

	initial();
}

Layer::~Layer()
{

	for (int i = 0; i < num_of_nodes; i++)
	{
		delete []weights[i];
	}

	delete []weights;

	delete []outputs;

	delete []errors;
}

void Layer::initial()
{
	for (int i = 0; i < num_of_nodes; ++i)
	{
		weights[i] = new double[pernode_input_num + 1];
		for (int w = 0; w < pernode_input_num + 1; w++)
		{
            weights[i][w] = RandomFloat0_5();;
		}
	}
}

