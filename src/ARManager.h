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

#include	<AR/param.h>
#include	<AR/ar.h>
#include	<AR/gsub.h>

#ifdef _WIN32
	#include	<Windows.h>
#endif // _WIN32


#include	"TinyXML/tinyxml.h"
#include	"AssetInfo.h"

#pragma once
class ARManager
{
private:
	boost::mutex				m_marker;
	boost::thread				arThread;
	ARUint8*					frame = NULL;
	std::map<int, AssetInfo>	markerList;
	std::list<AssetInfo>		markerFound;
	int							threshold = 100;
	bool						frameChange = false;
	bool						markerChange = false;
	static const bool			verbose = true;

public:
	ARManager();
	~ARManager();

public:
	void						init(std::string filename = "db.xml");
	void						start();
	void						stop();
	bool						setFrame(ARUint8 *p_frame);
	ARUint8*					getFrame() const;
	void						setMarkerList(std::map<int, AssetInfo> &markers);
	std::list<AssetInfo>		getMarkers();
	bool						isChanged();
	void						setTresh(int t);
	int							getTresh() const;
	void						clearMarker();
	void						addMarker(ARMarkerInfo info);
	void						validMarkers();

private:
	static void arLoop(ARManager *ar);
};

#endif