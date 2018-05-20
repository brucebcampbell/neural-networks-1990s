
class layer{

public:
	int units;
	enum {LINEAR,LOGISTIC,TANH,THRESHOLD} node_type;
	double bias;
	
public:
	layer();
	~layer();
	void operator=(layer &source);

};

class network:public layer{

public:
	enum {BKP,MD,LMS,RUN} training_type;
	int layers;
	int input_dim;
	double training_param;
	int num_vectors;
	int passes;
	char input_file_buf[256];
	char output_file_buf[256];
	char weights_file_buf[256];
	double lower_range,upper_range; //interval to choose weights from when initializing network
	//converting above three objects to vector layer-p for compatability with bk-prop fn's
	layer *layer_p;

public:
	network();//create layers 
	~network();//delete layers
	void operator=(network &source);

};


void win_train();