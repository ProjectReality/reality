#include "Hexapat.hpp"

Hexapat::Hexapat(int _id, int _basePatt)
{
	id = _id;
	basePatt = _basePatt;
}

std::map<int, aruco::Marker>	Hexapat::detect(std::map<int, aruco::Marker> markerFound)
{
	bool isFound = false;
	std::map<int, aruco::Marker>::iterator tmpMarker;

	for (int i = 0; i < 6; i++)
	{
		if ((tmpMarker = markerFound.find(basePatt +i)) != markerFound.end())
		{
			if (!isFound)
			{
				isFound = true;
				tmpMarker->second.Rvec.at<float>(0) += 90 * i;
			}
			else
			{
				markerFound.erase(tmpMarker);
			}
		}
	}
	return markerFound;
}

Hexapat::~Hexapat() {}