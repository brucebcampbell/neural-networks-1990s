// klNNGA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" void main_GA();

extern "C" void main_NN();

int _tmain(int argc, _TCHAR* argv[])
{
	main_GA();

	main_NN();


	return 0;
}

