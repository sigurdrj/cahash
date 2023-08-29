#ifndef HASH_HPP
#define HASH_HPP

#include <iostream>
#include <string>
#include <array>

#include "calib/calib.hpp"

#define GETBITATINDEX(n,idx) ((n)>>(idx))&1

using std::string;
using std::array;

// Puts the string on the grid as big endian binary
void set_grid_to_str(calib::Calib &ca, const string str){
	const unsigned width = ca.get_width();

	int y=-1; // One is immediately added to this so it will start at 0
	for (unsigned charIdx=0; charIdx<str.size(); charIdx++){
		for (unsigned char bitIdx=0; bitIdx<8; bitIdx++){
			unsigned x = (charIdx*8 + bitIdx) % width;
			y = x==0 ? y+1 : y;
			bool bit = GETBITATINDEX(str[charIdx], 7-bitIdx); // Big-endian notation
			ca.set_state(x,y,bit);
		}
	}
}

// Turns the grid from big endian binary ascii to a string
string grid_to_str(calib::Calib &ca){
	const unsigned width = ca.get_width();
	const unsigned height = ca.get_height();

	string out="";

	int y=-1; // One is immediately added to this so it will start at 0
	for (unsigned charIdx = 0; charIdx < width*height/8; charIdx++){
		unsigned char tmp=0;
		for (unsigned char bitIdx=0; bitIdx<8; bitIdx++){
			unsigned x = (charIdx*8 + bitIdx) % width;
			y = x==0 ? y+1 : y;
			tmp = (tmp<<1) | ca.get_state(x,y);
		}
		out += tmp;
	}

	return out;
}

string hash(const string input, const unsigned numIterations=1000, const unsigned numThreads=1){
	// TODO Maybe add a diferent ca/rule to iterate every other iteration or so to prevent attacks using hashlife from being any viable
	// TODO Use a better rule and neighborhood than this
	calib::Calib ca(128,128);
	ca.set_num_threads(numThreads);
	ca.set_rule(calib::Calib::rulestring_to_rule("b15/s37"));

	if (ca.get_width() % numThreads != 0)
		std::cerr << "Grid width is not divisible by the number of threads. Output will be inaccurate\n";

	set_grid_to_str(ca, input);

	for (unsigned i=0; i<numIterations; i++)
		ca.update_using_threads();

	return grid_to_str(ca);
}

#undef GETBITATINDEX

#endif // HASH_HPP
