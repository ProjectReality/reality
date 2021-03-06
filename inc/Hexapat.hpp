#ifndef __HEXAPAT_HPP__
#define __HEXAPAT_HPP__

#include	<opencv2/opencv.hpp>
#include    <string>
#include    <map>
#include	<math.h>

#include	"aruco.h"

#define PI 3.14159265

/**
 * @brief Class for Hexagonal pattern
 * @details This class is designed for reality project to create hexagonal pattern built from 6 simple pattern.
 * This is to permit to walk and turn around it.
 */

class Hexapat
{
private:
	int id;
	int	basePatt;
	int previous = 0;

public:
	/**
	* @brief Contructor for Hexapat
	* @details Construct an Hexapat object for detextion
	* @param _basePatt the id of the first of the six pattern (all other must have the five next value 
	* eg : 
	* _basePatt = 101, the other pattern will be between 102 and 106 included).
	**/
	Hexapat(int _id, int _basePatt);

	/**
	* @brief Detructor of the Hexapat
	**/
	~Hexapat();

	/**
	* @brief Detect if the Hexapat is in the list of detected Marker.
	* @param markerFound the found marker in the current frame.
	**/
	std::map<int, aruco::Marker>	detect(std::map<int, aruco::Marker> markerFound);

	/**
	* @brief Calcul the rotation of the object in function of the pattern numbers
	* @param markerFound the found marker in the current frame.
	**/
	cv::Mat							calcRotation(cv::Mat Rvec, int index);
};

#endif
