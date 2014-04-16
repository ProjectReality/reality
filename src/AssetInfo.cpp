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

void AssetInfo::setYaw(float p_yaw)
{
	this->yaw = p_yaw;
}

float AssetInfo::getYaw() const
{
	return (this->yaw);
}

void AssetInfo::setPitch(float p_pitch)
{
	this->pitch = p_pitch;
}

float AssetInfo::getPitch() const
{
	return (this->pitch);
}

void AssetInfo::setRoll(float p_roll)
{
	this->roll = p_roll;
}

float AssetInfo::getRoll() const
{
	return (this->roll);
}

void AssetInfo::setRot()
{
	float p_yaw = atan(info.rotMat.at<float>(1, 0) / info.rotMat.at<float>(0, 0)) * 180 / 3.14;
	float p_pitch = atan(-info.rotMat.at<float>(2, 0) / sqrt(pow(info.rotMat.at<float>(2, 1), 2) + pow(info.rotMat.at<float>(2, 2), 2))) * 180 / 3.14;
	float p_roll = atan(info.rotMat.at<float>(2, 1) / info.rotMat.at<float>(2, 2)) * 180 / 3.14;

	this->setYaw(p_yaw);
	this->setPitch(p_pitch);
	this->setRoll(p_roll);
}