#include "AssetInfo.h"

AssetInfo::AssetInfo(std::string pName, std::string aName)
:patternName(pName), assetName(aName)
{
	this->patt_center = new double[2];
	this->patt_center[0] = 0.0;
	this->patt_center[1] = 0.0;
	this->patt_trans = new double*[3];
	for (int i = 0; i < 3; i++)
		this->patt_trans[i] = new double[4];
}

AssetInfo::~AssetInfo()
{
	delete[] this->patt_center;
	for (int i = 0; i < 3; i++)
		delete[] this->patt_trans[i];
	delete[] this->patt_center;
}


void AssetInfo::setInfo(ARMarkerInfo mkInfo)
{
	this->info = mkInfo;
}

ARMarkerInfo AssetInfo::getInfo() const
{
	return (this->info);
}

void AssetInfo::setPattName(std::string pName)
{
	this->patternName = pName;
}

std::string AssetInfo::getPattName() const
{
	return (this->patternName);
}

void AssetInfo::setAssName(std::string aName)
{
	this->assetName = aName;
}

std::string AssetInfo::getAssName() const
{
	return (this->assetName);
}

void AssetInfo::setId(int p_id)
{
	this->patt_id = p_id;
}

int AssetInfo::getId() const
{
	return (this->patt_id);
}

void AssetInfo::setWidth(double width)
{
	this->patt_width = width;
}

double AssetInfo::getWidth() const
{
	return (this->patt_width);
}

void AssetInfo::setCenter(std::pair<double, double> center)
{
	this->patt_center[0] = center.first;
	this->patt_center[1] = center.second;
}

double *AssetInfo::getCenter() const
{
	return (this->patt_center);
}

void AssetInfo::setTranslation(double trans[3][4])
{
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 4; j++)
		this->patt_trans[i][j] = trans[i][j];
}

double** AssetInfo::getTranslation() const
{
	return (this->patt_trans);
}