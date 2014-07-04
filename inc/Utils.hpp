#ifndef __UTILS___
#define __UTILS___

#pragma once

/**
 * @brief The Utils class
 */
class Utils
{
public:
	Utils();
	~Utils();

public:
	static double c_to_d(char* c);
	static double c_to_d(const char* c);
	static int c_to_i(char* c);
	static int c_to_i(const char* c);
};

#endif
