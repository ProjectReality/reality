#include "Hexapat.hpp"

Hexapat::Hexapat(int _hexa[6]) 
{
	hexa[0] = _hexa[0];
	hexa[1] = _hexa[1];
	hexa[2] = _hexa[2];
	hexa[3] = _hexa[3];
	hexa[4] = _hexa[4];
	hexa[5] = _hexa[5];
}

std::map<int, aruco::Marker>	Hexapat::detect(std::map<int, aruco::Marker> markerFound)
{

}

Hexapat::~Hexapat() {}