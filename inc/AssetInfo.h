#ifndef		__ASSET_INFO__
# define	__ASSET_INFO__

#include	<string>

#include	<AR/param.h>
#include	<AR/ar.h>

#include	<boost/multi_array.hpp>

#pragma once
class AssetInfo
{
private:
	ARMarkerInfo	info;
	std::string		patternName;
	std::string		assetName;
	int				patt_id;
	double			patt_width = 80.0;
	double*			patt_center;
	double**		patt_trans;


public:
	AssetInfo(std::string pName = "", std::string aName = "");
	~AssetInfo();

public:
	void						setInfo(ARMarkerInfo mkInfo);
	ARMarkerInfo				getInfo() const;
	void						setPattName(std::string pName);
	std::string					getPattName() const;
	void						setAssName(std::string aName);
	std::string					getAssName() const;
	void						setId(int p_id);
	int							getId() const;
	void						setWidth(double width);
	double						getWidth() const;
	void						setCenter(std::pair<double, double> center);
	double*						getCenter() const;
	void						setTranslation(double trans[3][4]);
	double**					getTranslation() const;
};

#endif