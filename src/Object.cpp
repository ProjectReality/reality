#include "Object.hpp"


Object::Object(std::string _name, std::string _mesh, Ogre::SceneManager* scene) : Entity(_name)
{
	if (scene != NULL)
	{
		this->setEnt(scene->createEntity(_name, _mesh));
		this->setNode(scene->getRootSceneNode()->createChildSceneNode());
		this->getEnt()->setCastShadows(true);
		this->getNode()->attachObject(this->getEnt());
		this->setScale(Ogre::Vector3(0.00015, 0.00015, 0.00015));
		this->visible(false);
		this->update();
	}
}

void Object::updateData(aruco::Marker info)
{
	double position[3], orientation[4];

	info.OgreGetPoseParameters(position, orientation);
	this->setPosition(position[0], position[1], position[2]);
	Ogre::Quaternion q(orientation[0], orientation[1], orientation[2], orientation[3]);
	Ogre::Quaternion y(Ogre::Degree(this->getBaseYaw()), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion p(Ogre::Degree(this->getBasePitch()), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion r(Ogre::Degree(this->getBaseRoll()), Ogre::Vector3::UNIT_X);
	if (info.ssize > 19999)
	{
		Ogre::Quaternion roll(Ogre::Degree(60 * info.ssize - 20000), Ogre::Vector3::UNIT_X);
		this->setOrientation(q * roll);
	}
	else
		this->setOrientation(q);
	this->rotate(this->getBaseYaw(), this->getBasePitch(), this->getBaseRoll());
}

int Object::getPatt_id() const
{
    return patt_id;
}

void Object::setPatt_id(int value)
{
    patt_id = value;
}


std::ostream& operator<<(std::ostream& os, const Object& pat)
{
    os << "Name = " << pat.getName() << " Asset = " << pat.getName() << " Id = " << pat.getPatt_id() << std::endl;
    os << "X = " << pat.getX() << " Y = " << pat.getY() << " Z = " << pat.getZ() << std::endl;
    os << "Yaw = " << pat.getYaw() << " Pitch = " << pat.getPitch() << " Roll = " << pat.getRoll() << std::endl;
    return os;
}
