#ifndef _CORE_HPP
#define _CORE_HPP

#include	"OgreRenderer.hpp"
#include	"StereoCamera.hpp"
#include	"Oculus.hpp"
#include	"ARManager.hpp"
#include    "Object.hpp"
#include	"Utils.hpp"

/**
* @brief Core of the reality project.
* @details This class contain the main routine and make all communication between the different blocs of Reality.
*
*/

class Core
{
private:
	OgreRenderer*					render;
	StereoCamera					camera;
	cv::Mat*						frame;
	double							video_size[2];
	ARManager						ar;
	std::map<int, Object*>			objects;
	VirtualOculus					*rift;
    int                             argc;
    char**                          argv;

public:
    Core(int argc, char** argv);
	~Core();

public:
	/**
	* @brief This is the initilisation function
	* @details This function initialise the different composants of Reality as the list of possible object that ogre must load,
	* the capture system...
	* @see start()
	**/
	void	init();

	/**
	* @brief This function is the main routine of Reality
	* @details this function start the detection routine and begin to capture the webcam input and track pattern inside.
	**/
	void	start();

	/**
	* @brief This function build the list of object that the project will used for the augmented reality.
	* @details This list is built from a "database file" in xml which contain the name of all asset but also many information 
	* as their scale and thair initial rotation. 
	* @param filename The name of the xml database.
	* @return the number of object initialized
	**/
	int	buildObjectsList(std::string filename = "Data/db.xml");

    /**
     * @brief return the number of argument given in the command line
     * @return number
     */
    int get_n_arg();

    /**
     * @brief get_cam
     * @return
     */
	StereoCamera get_camera();

	void startOgre();

};

#endif
