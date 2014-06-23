/*
**	Class created by Alexandre Haulotte for Reality project
**	This class is used to detect marker in environement
*/

#ifndef		__AR_MANAGER__
# define	__AR_MANAGER__

#include	<list>
#include	<map>
#include	<string>
#include	<iostream>

#include	<boost/thread.hpp>

#include	<opencv2/opencv.hpp>
#include	<opencv2/highgui/highgui.hpp>

#ifdef _WIN32
	#include	<Windows.h>
#endif // _WIN32

#include	"TinyXML/tinyxml.h"
#include	"Object.hpp"
#include	"aruco.h"


/**
* @brief Class for the tracking of Marker
* @details This class is used to detect in all frame if a pattern is present or not.
* It also used to make a robust detection and correct a wrong detection.
*
*/

#pragma once
class ARManager
{
private:
	boost::mutex										m_marker; /**< This is a Boost mutex used to protect the tracking when it is building the list of detected marker. */
	boost::thread										arThread; /**< This thread is used to create a detector routine. */
	aruco::MarkerDetector								markerDetector; /**< This is the detector provide by ARUCO used to track any marker. */
	cv::Mat												frame; /**< The frame is the captured frame copied from the camera. */
	std::map<int, std::string>							listObjects; /**< This is a list of mesh name linked to their Marker Id. */
	std::map<int, aruco::Marker>						markerFound; /**< The map markerFound is a map of all found marker in one cycle of the tracking routine. */
	bool												frameChange; /**< This boolean is used in the tracking routine to know if the frame is a new one. */
	bool												markerChange; /**< This boolean is used to signified that the actual fram was analized. */
	aruco::CameraParameters								cameraMatrix; /**< The cameraMatrix is build from a config file used to avoid distortion of each camera. */
	std::vector<std::map<std::string, aruco::Marker>>	histo; /**< This vector is an historic of a range of previous detected map of Marker used to get a better tracking */

public:
	std::map<int, aruco::Marker>						markerFoundCopy;
	static const bool			verbose = true; /**< This boolean is used to activate or desactivate the debug in the tracking methods */


public:
	ARManager();
	~ARManager();

public:
	/**
	* @brief This function is used to add a new marker Id and mesh name in the list listObjects
	* @details This function is used to add a new marker Id and mesh name in the list listObjects
	* @param name The name of the mesh.
	* @param id The Id of the marker linked with this mesh.
	* @see listObjects
	**/
	void							addPatternInList(std::string name, int id);

	/**
	* @brief This function is used to start the thread of the detector routine
	* @details This function is used to start the thread of the detector routine arThread which is arLoop
	* @see arThread
	* @see stop()
	* @see arLoop()
	**/
	void							start();

	/**
	* @brief This function is used to stop the thread of the detector routine
	* @details This function is used to stop the thread of the detector routine arThread which is arLoop
	* @see arThread
	* @see start()
	* @see arLoop()
	**/
	void							stop();

	/**
	* @brief This function is used to set a new input frame
	* @details This function is used to set a new input frame to track marker insisde.
	* @param p_frame The new frame capture from camera.
	* @see getFrame()
	* @return If the new frame is succefully saved return true. In all other case return false.
	**/
	bool							setFrame(cv::Mat p_frame);

	/**
	* @brief This function is used to get the frame
	* @details This function is used to get the frame in the detector routine.
	* @see setFrame(cv::Mat p_frame)
	* @return Return the actual saved frame
	**/
	cv::Mat							getFrame() const;

	/**
	* @brief This function set an initial list of marker
	* @details This function set an initial list of marker used to inittialize the tracking
	* @param markers the list of marker id and mesh name
	**/
	void							setMarkerList(std::map<int, std::string> &markers);

	/**
	* @brief This function is to get the map of marker detected by the routine.
	* @see addMarker()
	* @return The map of detected marker from the actual frame.
	**/
    std::map<int, aruco::Marker>	getMarkers();

	/**
	* @brief This function is used to know if their is a new map of detected markers.
	* @see getMarkers()
	* @return True if the map of detected markers has changed and False the map is the same than in the previous loop.
	**/
	bool							isChanged();

	/**
	* @brief Clear the content of the markerFound map.
	* @param p_frame The new frame capture from camera.
	**/
	void							clearMarker();

	/**
	* @brief Function used to set the vector of detected Marker.
	* @details Function used to set the vector of detected Marker. It is called by the detection routine.
	* @param markers The new vector of detected marker.
	**/
	void							addMarker(std::vector<aruco::Marker> markers);

	/**
	* @brief This function is used to clean the vector of Marker found.
	* @param p_Pattern The new vector of marker found in the actual frame.
	* @see computeNewMap()
	**/
	void							cleanVector(std::vector<aruco::Marker>& p_Pattern);

	/**
	* @brief This function compute the fitness of the new position of the markers.
	* @param p_Pattern The new vector of marker found in the actual frame.
	* @see computeNewMap()
	* @return The distance between the detected markers and the previous one.
	**/
	int								calcDist(std::vector<aruco::Marker>& p_Pattern);

	/**
	* @brief This function compute the new list of marker detected.
	* @details This function compute the new list of marker detected from the historic of previous detection and the actual detected pattern.
	* @see histo
	* @see markerFound
	* @see calcDist(std::vector<aruco::Marker>& p_Pattern)
	* @see cleanVector(std::vector<aruco::Marker>& p_Pattern)
	* @return The new list of Marker position modified in function of the robust tracking algorithm.
	**/
	std::map<int, aruco::Marker>	computeNewMap();

	/**
	* @brief This function is used to get the distortion camera parameter.
	* @see cameraMatrix
	* @return The distortion parameter of the camera
	**/
	aruco::CameraParameters			getCamParam() const;

private:
	/**
	* @brief This function is the tracking routine
	* @details This function is the tracking routine used in the detector thread.
	* @param ar This is a pointer on the ARManager used in all the project. Here it is used to get a frame and send the detected marker.
	* @see start()
	* @see stop()
	**/
	static void arLoop(ARManager *ar);

};

#endif
