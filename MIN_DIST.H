extern struct layer *layer_p;

extern struct network ntwk_info;

void min_disturbance(struct layer *layer_p,struct network ntwk_info);

static void md_unit_p_init(struct layer *layer_p);

static void md_propagate(struct layer *layer_p,double *data_v);

static void rank(struct layer *layer_p);

static int compare(const void *arg1,const void *arg2);

static double md_error_comp(struct layer *layer_p,double *desired_out);

static void md_update_weight(struct layer *layer_p,int update_unit,int num_inputs,double *data_v);

static void md_output_network(struct layer *layer_p);

static void md_get_data(void);

static void md_file_open(void);



