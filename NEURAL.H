extern struct network{   
    int layers;
    int input_dim;
    int num_vectors;
    double training_param;
    int passes;
    enum {LMS,MIN_DISTURBANCE,BACKPROPAGATION} training;}; 




extern struct layer{                                                                       
    int units;      // number units on layer
    enum {LINEAR,THRESHOLD,LOGISTIC,TANH} node_type; // threshold is a bipolar output, sgn(LINEAR)
    double *output; //pointer to output vector of layer
    double **weights;
    double bias;};
