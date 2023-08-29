#include <iostream>

#include "cahash.hpp"

void usage(){
	std::cout << "Usage: test [data] [number of iterations] [number of threads]\n";
}

int main(int argc, char *argv[]){
	if (argc<4){
		usage();
		return 1;
	}

	const string input = argv[1];
	unsigned numIterations;
	unsigned numThreads;
	try{
		numIterations = std::stoi(argv[2]);
		numThreads    = std::stoi(argv[3]);
	} catch (std::invalid_argument &){
		usage();
		return 2;
	}

	std::cout << hash(input, numIterations, numThreads);
}
