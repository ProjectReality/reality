#include "Hexapat.hpp"
#include "Logger.hpp"

Hexapat::Hexapat(int _id, int _basePatt)
{
	id = _id;
	basePatt = _basePatt;
}

std::map<int, aruco::Marker>	Hexapat::detect(std::map<int, aruco::Marker> markerFound)
{
	bool isFound = false;
	std::map<int, aruco::Marker>::iterator tmpMarker;
	if (previous != 0 && ((tmpMarker = markerFound.find(previous)) != markerFound.end()))
	{
		isFound = true;
		aruco::Marker m = tmpMarker->second;
		m.Rvec = m.Rvec;
		m.ssize = 20000 + (m.id - basePatt);
		m.id = id;
		markerFound.insert(std::pair<int, aruco::Marker>(id, m));
	}

	for (int i = 0; i < 6; i++)
	{
		if ((tmpMarker = markerFound.find(basePatt + i)) != markerFound.end())
		{
			if (!isFound)
			{
				isFound = true;
				aruco::Marker m = tmpMarker->second;
				m.Rvec = m.Rvec;
				previous = m.id;
				m.id = id;
				m.ssize = 20000 + i;
				markerFound.insert(std::pair<int, aruco::Marker>(id, m));
			}
			markerFound.erase(tmpMarker);
		}
	}
	return markerFound;
}

cv::Mat Hexapat::calcRotation(cv::Mat Rvec, int index)
{
	float rotBase = 60.0;
	float rotMat[4][4] = { { cos(rotBase * index * PI / 180), -sin(rotBase * index * PI / 180), 0, 0 },
						{ sin(rotBase * index * PI / 180), cos(rotBase * index * PI / 180), 0, 0 },
						{ 0, 0, 1, 0 },
						{ 0, 0, 0, 1} };
	float rotVec[4] = { Rvec.at<float>(0), Rvec.at<float>(1), Rvec.at<float>(2) , 1};

	for (int row = 0; row < 3; row++) {
		Rvec.at<float>(row) = 0;
		for (int col = 0; col < 3; col++) {
			Rvec.at<float>(row) += rotMat[row][col] * rotVec[col];
		}
	}

	return (Rvec);
}

Hexapat::~Hexapat() {}