#include "Hexapat.hpp"
#include "Logger.hpp"

Hexapat::Hexapat(int _id, int _basePatt)
{
	id = _id;
	basePatt = _basePatt;
	Logger::log_tag("DebugHexapat", "Création of Hexapat number %i", id);
}

std::map<int, aruco::Marker>	Hexapat::detect(std::map<int, aruco::Marker> markerFound)
{
	bool isFound = false;
	std::map<int, aruco::Marker>::iterator tmpMarker;
	Logger::log_tag("DebugHexapat", "Detection function of Hexapat number %i", id);
	for (int i = 0; i < 6; i++)
	{
		if ((tmpMarker = markerFound.find(basePatt + i)) != markerFound.end())
		{
			if (!isFound)
			{
				isFound = true;
				tmpMarker->second.Rvec.at<float>(0) += 60 * i;
				markerFound[id] = tmpMarker->second;
				Logger::log_tag("DebugHexapat", "Hexapat number %i detected !", id);
			}
			markerFound.erase(tmpMarker);
		}
	}
	return markerFound;
}

Hexapat::~Hexapat() {}