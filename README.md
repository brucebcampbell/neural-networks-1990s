klNNGA

This is the original documentation of some algorithm work I did in the 90's. 
======

Neural Network & Genetic Algorithms  - 1995.       

Neural Network Library

CONTENTS
1)  What Is Neural Network Library?
2)  What's included
3)  What's not included
4)  How to use the example files
5)  The rules - how to use the program
6)  More on the training algorithms.
7)  Pointers to more info. on Neural Networks


WHAT IS NEURAL NETWORK LIBRARY?
Neural Network Library is a program for training and running feedforward neural networks.    
Features - for now:
a)  Up to 4 hidden layers.
b)  5 different types of activation function that may vary across layers 
c)  Bias may be added to any combination of layers
d)  Three different types of training algorithms:
     i)    backpropagation
     ii)   minimum disturbance
     iii)  least mean square

WHAT'S INCLUDED  in the klNNGA Git Repository
1)  neur32.exe - a Windows32 bit version running in a console box
2)  The complete set of source files that should compile with minor if any changes
3)  Several examples of network config files and associated data
4) bpwin1.0 an MFC application with a rather brittle UI to set up a multilayer feed forward Neural Network architecture. 
     
HOW TO USE EXAMPLE FILES 
To train a simple network on three data pairs:
1) Start the program
2) At the first file prompt, enter bp3.txt, then enter a 0 at the next prompt for training 
3) At the second file prompt, enter datbp3.txt
4) At the third prompt enter the name of the file you want output results to be written to
   One will be created if you specify a file that doesn't exist.
5) Remember to run have all of the above files in the same directory as the program,   otherwise enter the full path of the file.

To run the above trained network:
1) Start the program
2) At the first file prompt, enter bp3.txt, then enter a 1 at the next prompt for running
3) At the second file prompt, enter datbpr.txt
4) At the third file prompt, a file to write results to
5) At the fourth file prompt, enter the output file from the training run
 
Note these files are very simple examples designed to demonstrate the training and running of very simple networks.  You should examine all of the files mentioned  above to get an idea of what the program expects as input and how it formats output.


Constrints 
When you run the program  it will ask you for a series of files.  First is a config
file for the network you want to run.  This contains information on the topology 
of the network, activation functions, training algorithm, and training parameters.
The program will ask you if you are training or running the network.  If you are 
running the network, then the training fields in the config file are is not used.
  
The network config files included as examples contain important comments that you  should read.  It is important to use the file formats that the examples use.  You  are free to alter these as needed or create your own.  The constraints are that the  correct number of data fields appear in the proper order each preceded by the delimiter ':'
After a network has been configured, the program asks for a data file.  All data should  be in comma delimited format (cvs - no " enclosing data).  For training data, the input  vectors should preceded the desired output vectors.  You don't need to worry about  whitespace in the data files, but don't include anything else other than data, commas  and whitespace in the data files.

If you have chosen to train a network, the program will ask for an output file name.   If you want to run the network after it has been trained, this file will contain the  weights in the proper input format to run the network on real data.  The program outputs  the weights of a trained network along with error results for the training algorithm.   For large data sets, these files can get large.

If you are running a previously trained network, you will have to provide the program  with the weights.  Use the output of a previous training run, otherwise generate your  own weighs file.  The constraints are that the weights appear before the delimiter ':' in the order in which they are encountered going from left to right and across nodes,  and layers and then up through the layers.  If you are providing your own weights file,  it is important to remember to include a bias weight as the first weight for each node  on each layer.  You should do this regardless of whether or not a bias is used.  The  program allocates a bias weight for each node on each layer and uses a bias value of  zero when one is not wanted.

You can alter the source code in any way to suit you needs.  If you redistribute it after altering, please take all/some credit.  


MORE ON THE TRAINING ALGORITHMS
There are three training modes available.  Backpropagation and least mean squares  are fairly standard, good descriptions can be found in an introductory book on  neural networks.  The minimum disturbance algorithm is adapted from -  Madaline Rule II: A Training Algorithm For Neural Networks, R. Winter, B. Widrow,  Proceedings of the IEEE Second International Conference on Neural Networks, I 401, 1988.    


 MORE INFO
Introduction to the Theory of Neural Computation, J. Hertz, A. Krogh, R. Palmer  Santa Fe Institute Lecture Notes Volume I, 1991 
Written by physicists, so there's more statistical mechanics than one would find elsewhere. A very good book.

Parallel Distributed Processing Vols I & II, Rummelhart, McClelland
MIT Press 1986   A cog-sci perspective. Another good book.  

Also check out the newsgroup comp.ai.neural-nets and read the FAQ's




