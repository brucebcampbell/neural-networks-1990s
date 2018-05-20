
extern struct layer *layer_p;

extern struct network ntwk_info;

void back_prop(struct layer *layer_p,struct network ntwk_info);

static void bp_unit_p_init(struct layer *layer_p);

static void bp_propagate(struct layer *layer_p,double *data_v);

static double bp_error_comp(struct layer *layer_p,double *desired_out);

static void bp_update_weight(struct layer *layer_p);

static void bp_output_network(struct layer *layer_p);

static void bp_get_data(void);

static void bp_file_open(void);


