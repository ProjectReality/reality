#ifndef		__ASSET_INFO__
# define	__ASSET_INFO__

#include	<string>

#include	<boost/multi_array.hpp>

#include	"ARma/pattern.h"

#pragma once
class AssetInfo
{
private:
	ARma::Pattern	info;
	cv::Mat			patt_frame;
	std::string		patternName;
	std::string		assetName;
	int				patt_id;


public:
	AssetInfo(std::string pName, std::string aName);
	~AssetInfo();

public:
	void						setInfo(ARma::Pattern mkInfo);
	ARma::Pattern				getInfo() const;
	void						setPattName(std::string pName);
	std::string					getPattName() const;
	void						setAssName(std::string aName);
	std::string					getAssName() const;
	void						setId(int p_id);
	int							getId() const;
	cv::Mat						getPattFrame() const;
	void						setPattFrame(cv::Mat p_frame);
};

#endif