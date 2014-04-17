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
	float p_yaw = -info.rotVec.at<float>(2) * 180 / 3.14;
	float p_pitch = -info.rotVec.at<float>(1) * 180 / 3.14;
	float p_roll = info.rotVec.at<float>(0) * 180 / 3.14;

	this->setYaw(p_yaw);
	this->setPitch(p_pitch);
	this->setRoll(p_roll);
}

void AssetInfo::setPos()
{
	float p_x = info.transVec.at<float>(0) * 4;
	float p_y = -info.transVec.at<float>(1) * 10;
	float p_z = -info.transVec.at<float>(2);

	this->x = p_x;
	this->y = p_y;
	this->z = p_z;
}

std::ostream& operator<<(std::ostream& os, const AssetInfo& pat)
{
	os << "Name = " << pat.getPattName() << " Asset = " << pat.getAssName() << " Id = " << pat.getId() << std::endl;
	os << "X = " << pat.x << " Y = " << pat.y << " Z = " << pat.z << std::endl;
	os << "Yaw = " << pat.yaw << " Pitch = " << pat.pitch << " Roll = " << pat.roll << std::endl;
	return os;
}