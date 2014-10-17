#ifndef __HEXAPAT_HPP__
#define __HEXAPAT_HPP__

#include    <string>

#include    "ARManager.hpp"

/**
 * @brief Class for Hexagonal pattern
 * @details Handle all the lights in Ogre, shadow settings, and
 * light placement
 */

class Hexapat
{
private:
	int	hexa[6];

public:
	/**
	* @brief Contructor for Hexapat
	* @details Construct an Hexapat object for detextion
	* @param _hexa[] the array of six pattern int the order
	**/
	Hexapat(int _hexa[6]);
	/**
	* @brief Detructor of the Hexapat
	**/
	~Hexapat();

	/**
	* @brief Detect if the Hexapat is in the list of detected Marker.
	* @param markerFound the found marker in the current frame.
	**/
	std::map<int, aruco::Marker>	detect(std::map<int, aruco::Marker> markerFound);
};

#endif
