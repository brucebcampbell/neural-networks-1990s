
extern struct layer *layer_p;

extern struct network ntwk_info;

void run_network(struct layer *layer_p);

static int rn_get_data(void);

static void rn_file_open(void);

static void rn_propagate(struct layer *layer_p);

static void rn_ntwk_init(struct layer *layer_p);

static void write_output(struct layer *layer_p);

static double get_weight(void);

