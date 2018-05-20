
class bp_unit_train{
public:	
    double *weights;
    double linear_output; 
    double activation;
    double derivative;
    double delta;

	bp_unit_train();
	~bp_unit_train();
};//end class bp_unit_train    

class bkp : public network{
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
    
	bp_unit_train **bp_unit_p; //matrix of pointers to units
	bp_unit_train *x;  // optimization variable to be used where layer and unit are fixed 

	double *data_p,*desired_out;  // input & output vectors
	int count,count_l,count_u,count_w;// counters for training vectors, layers, units, weights
	int vector_number; //counter fo the number of vectors in data set
	double error;   
	double **global_error; // an array of ptrs to error sums, indexed over training passes 
	int output_dim;
	//these are here until another input routine is written
    FILE *fp,*fp1,*fp3;  //fp is data input file
	char buf[256];

public:
	bkp(network *ntwk_input);
	~bkp();
	void operator=(bkp &source);
	void back_prop(); //calls propagate,error-comp,update_wight,get_data,file_open

private:
	void   bp_propagate(double *data_v);
	double bp_error_comp(double *desired_out);
	void   bp_update_weight();
	void   bp_get_data();
	void   bp_file_open();
	void   bp_output_network();
	
}; //end class bkp declaration


static long int save_position;

//replace with passes and count from passes to 0
//int pass_num=0;  

// indicates the random choice of training vector selected from file
//make member of get data
//int vector_number;


