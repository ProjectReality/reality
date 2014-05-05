#include "Object.hpp"


Object::Object(std::string _name, std::string _mesh, Ogre::SceneManager* scene) : Entity(_name)
{
    this->setEnt(scene->createEntity(_name, _mesh));
    this->setNode(scene->getRootSceneNode()->createChildSceneNode());
    this->getNode()->attachObject(this->getEnt());
    this->getEnt()->setCastShadows(true);
    this->update();
}

void Object::updateData(ARma::Pattern info)
{
    info.rotationMatrix(info.getRotvec(), info.getRotMat());

    float p_yaw = -info.rotVec.at<float>(2) * 180 / 3.14f;
    float p_pitch = -info.rotVec.at<float>(1) * 180 / 3.14f;
    float p_roll = info.rotVec.at<float>(0) * 180 / 3.14f;

    float p_x = info.transVec.at<float>(0) * 4;
    float p_y = -info.transVec.at<float>(1) * 9;
    float p_z = -info.transVec.at<float>(2);

    this->setPosition(p_x, p_y, p_z);
    this->setRotation(p_pitch, p_roll, p_yaw);
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
