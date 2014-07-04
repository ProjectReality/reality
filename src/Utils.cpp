#include <sstream>
#include "Utils.hpp"

double Utils::c_to_d(char* c){
	std::istringstream ss(c);
	double n;
	ss >> n;
	return n;
}

double Utils::c_to_d(const char* c){
	char* tc = const_cast<char*>(c);
	return c_to_d(tc);
}

int Utils::c_to_i(char* c){
	std::istringstream ss(c);
	int n;
	ss >> n;
	return n;
}

int Utils::c_to_i(const char* c){
	char* tc = const_cast<char*>(c);
	return c_to_i(tc);
}

Utils::Utils()
{
}


Utils::~Utils()
{
}
