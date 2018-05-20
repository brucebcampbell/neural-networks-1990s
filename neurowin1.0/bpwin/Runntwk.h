
class rn_unit_train{
public:	
    double *weights;
    double linear_output; 
    double activation;
	rn_unit_train();
	~rn_unit_train();
};//end class rn_unit_train    

class rnntwk : public network{
/* for reference these are the public members of base class network
public:
	int layers;
	int input_dim;
	double training_param;
	int num_vectors;
	int passes;
	layer *layer_p;

 //and these are the members of class layer
	int units;
	enum {LINEAR,LOGISTIC,TANH} node_type;
	double bias;
*/
private:
    
	rn_unit_train **rn_unit_p; //matrix of pointers to units
	double *data_p;  // input vector
	int count,count_l,count_u,count_w;// counters for vectors, layers, units, weights
	int vector_number; //counter fo the number of vectors in data set
	int output_dim;
	int eof_flag;
	//these are here until another input routine is written
    FILE *fp,*fp1,*fp2;  //fp is data input file
	char buf[256];

public:
	rnntwk(network *ntwk_input);
	~rnntwk();
	void run_network(); //calls propagate,get_data,file_open,write_output

private:
	void   rn_propagate();
	double   rn_get_weight();
	int   rn_get_data();
	void   rn_file_open();
	void   rn_write_output();
	
}; //end class rnntwk declaration


