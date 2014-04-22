#include "AssetInfo.hpp"

AssetInfo::AssetInfo(std::string pName, std::string aName)
:patternName(pName), assetName(aName)
{
}

AssetInfo::~AssetInfo()
{
}


void AssetInfo::setInfo(ARma::Pattern mkInfo)
{
	this->info = mkInfo;
}

ARma::Pattern AssetInfo::getInfo() const
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