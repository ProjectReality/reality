#include "Hexapat.hpp"

Hexapat::Hexapat(int _id, int _hexa[6])
{
	id = _id;
	hexa[0] = _hexa[0];
	hexa[1] = _hexa[1];
	hexa[2] = _hexa[2];
	hexa[3] = _hexa[3];
	hexa[4] = _hexa[4];
	hexa[5] = _hexa[5];
}

std::map<int, aruco::Marker>	Hexapat::detect(std::map<int, aruco::Marker> markerFound)
{
	bool isFound = false;
	std::map<int, aruco::Marker> markerHexa;
	std::map<int, aruco::Marker>::iterator tmpMarker;

	for (int i = 0; i < 6; i++)
	{
		if ((tmpMarker = markerFound.find(hexa[i])) != markerFound.end())
		{
			markerHexa[tmpMarker->first] = tmpMarker->second;
			markerFound.erase(tmpMarker);
			if (!isFound)
				isFound = true;
		}
	}
	if (isFound)
	{
		int nbFace = markerHexa.size();
		markerHexa.at(0).Rvec;
	}
	return markerFound;
}

Hexapat::~Hexapat() {}