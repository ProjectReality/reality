#include "Object.hpp"


Object::Object(std::string _name, std::string _mesh, Ogre::SceneManager* scene) : Entity(_name)
{
	frameCpt = 0;
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
	double distance = -0.1;

	info.OgreGetPoseParameters(position, orientation);
	if (info.id > 2048)
	{
		double a, b, c, d, x, y, z, rotVect[3];
		a = orientation[0];
		b = orientation[1];
		c = orientation[2];
		d = orientation[3];
		x = 0;
		y = 0;
		z = -0.05;
		double rotMat[3][3];

		rotMat[0][0] = pow(a, 2) + pow(b, 2) - pow(c, 2) - pow(d, 2);
		rotMat[0][1] = (2 * b * c) - (2 * a * d);
		rotMat[0][2] = (2 * b * d) + (2 * a * c);
		rotMat[1][0] = (2 * b * c) + (2 * a * d);
		rotMat[1][1] = pow(a, 2) - pow(b, 2) + pow(c, 2) - pow(d, 2);
		rotMat[1][2] = (2 * c * d) - (2 * a * b);
		rotMat[2][0] = (2 * b * d) + (2 * a * c);
		rotMat[2][1] = (2 * c * d) - (2 * a * b);
		rotMat[2][2] = pow(a, 2) - pow(b, 2) - pow(c, 2) + pow(d, 2);

		rotVect[0] = (rotMat[0][0] * x) + (rotMat[0][1] * y) + (rotMat[0][2] * z);
		rotVect[1] = (rotMat[1][0] * x) + (rotMat[1][1] * y) + (rotMat[1][2] * z);
		rotVect[2] = (rotMat[2][0] * x) + (rotMat[2][1] * y) + (rotMat[2][2] * z);

		position[0] += rotVect[0];
		position[1] += rotVect[1];
		position[2] += rotVect[2];
	}
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
