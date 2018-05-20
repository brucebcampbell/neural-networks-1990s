
extern struct layer *layer_p;

extern struct network ntwk_info;

void lms(struct layer *layer_p,struct network ntwk_info);

void lms_get_data(void);

static void lms_file_open(void);

static void lms_output_network(struct layer *layer_p);
