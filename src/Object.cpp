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
	double distance = -0.1;

	info.OgreGetPoseParameters(position, orientation);
	this->setPosition(position[0], position[1], position[2]);
	Ogre::Quaternion q(orientation[0], orientation[1], orientation[2], orientation[3]);
	Ogre::Quaternion y(Ogre::Degree(this->getBaseYaw()), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion p(Ogre::Degree(this->getBasePitch()), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion r(Ogre::Degree(this->getBaseRoll()), Ogre::Vector3::UNIT_X);



	if (info.ssize > 19999)
	{
		double mat[16];		// numérotée en colonne puis ligne 	
		double Y_quat = orientation[2];
		double w = orientation[0];
		double x = orientation[1];
		double z = orientation[3];
		mat[0] = 1 - 2 * ((Y_quat*Y_quat) + (z*z));
		mat[1] = 2 * ((x*Y_quat) - (z*w));
		mat[2] = 2 * ((x*z) + (Y_quat*w));
		mat[4] = 2 * ((x*Y_quat) + (z*w));
		mat[5] = 1 - 2 * ((x*x) + (z*z));
		mat[6] = 2 * ((Y_quat*z) - (x*w));
		mat[8] = 2 * ((x*z) - (Y_quat*w));
		mat[9] = 2 * ((Y_quat*z) + (x*w));
		mat[10] = 1 - 2 * ((x*x) + (Y_quat*Y_quat));
		mat[3] = mat[7] = mat[11] = 0;
		mat[15] = 1;
		double x1 = 0;
		double y1 = 0;
		double z1 = 0.2;
		double z2 = 0.005;
		double z3 = -0.1;
		double newpos_x = position[0] + (x1*mat[0]) + (y1*mat[4]) + (z1*mat[8]);
		double newpos_y = position[1] + (x1*mat[1]) + (y1*mat[5]) + (z2*mat[9]);
		double newpos_z = position[2] + (x1*mat[2]) + (y1*mat[6]) + (z3*mat[10]);
		this->setPosition(newpos_x, newpos_y, newpos_z);
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
