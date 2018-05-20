
class md_unit_train{  
public:
    int trained;                
    double *weights;
    double *weights_temp;    
    double linear_output;
    double threshold_output;
    int rank;
	md_unit_train();
	~md_unit_train();};    // rank member is not used at this point 


class md:public network{
friend int  compare(const void *arg1,const void *arg2);
public:
	md(network *ntwk_input);
	~md();

private:
md_unit_train **md_unit_p;
md_unit_train *z,*x;  // optimization variables to be used in loops where layer and unit are fixed and we're stepping through weights 
                      // z is used in min_disturbance(), x in propagate()  
FILE *fp, *fp1;       
char buf[256];
long int save_position;
double *data_p,*desired_out;  // input & output vectors 
int pass_num;  // counter for the number of training passes for the training data 
int vector_selection;  // indicates the random choice of training vector selected from file, used in min_disturbance() and md_get_data() 
int count,count_l,count_u,count_w;  // counters for stepping through training vectors, layers, units, weights 
int adaption_layer; // indicates the layer nodes are being adapted on 
int count_lr,count_ur;  // used by rank and compare functions 
double error,error_new,temp;  // used in md_update_weights() and min_disturbance() 
double *global_error; // an array of ptrs to error sums, indexed over training passes 
int output_dim;
int **rank_mat; //units ranked by rank() fn accross layers, then rank_mat is used in min_dist() to select units for updating
int update_unit,num_inputs;  // used in min_disturbance(), md_update_weight() 

public:
void min_disturbance();

private:
void md_unit_p_init();
void md_propagate(double *data_v);
void rank();
void fseek_to_vector(int selection);
double md_error_comp(double *desired_out);
void md_update_weight(int update_unit,int num_inputs,double *data_v);
void md_output_network();
void md_get_data();
void md_file_open();
};//end class md declaration


