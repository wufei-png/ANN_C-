
#ifndef __LAYER__
#define __LAYER__

class Layer
{
public:
	Layer(int num_nodes, int pernode_input_num);
	~Layer();
	void initial(void);
public:
	int pernode_input_num; //每个节点的输入数量
	int num_of_nodes; //节点数
	double** weights;
	double* outputs;
	double* errors;
};

#endif

