#include "Entity.hpp"


Entity::Entity(std::string _name)
{
    if ((_name.length() < 1)) {
        std::cerr << "Error: createEntity: invalid name" << std::endl;
        return;
    }
    this->name = _name;
}

Entity::~Entity()
{
    this->scene->destroyEntity(this->name);
}

void Entity::setPosition(float x, float y, float z)
{
    this->node->setPosition(Ogre::Vector3(x, y, z));
    this->update();
}

void Entity::setPosition(Ogre::Vector3 pos)
{
    this->node->setPosition(pos);
    this->update();
}

void Entity::move(float x, float y, float z)
{
    this->node->translate(Ogre::Vector3(x, y, z));
    this->update();
}

void Entity::move(Ogre::Vector3 mvt)
{
    this->node->translate(mvt);
    this->update();
}

void Entity::setRotation(float yaw, float pitch, float roll)
{
    Ogre::Quaternion y(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
    Ogre::Quaternion p(Ogre::Degree(pitch), Ogre::Vector3::UNIT_X);
    Ogre::Quaternion r(Ogre::Degree(roll), Ogre::Vector3::UNIT_Z);
    this->node->setOrientation(y*p*r);
    this->update();
}

void Entity::rotate(float yaw, float pitch, float roll)
{
    Ogre::Quaternion y(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
    Ogre::Quaternion p(Ogre::Degree(pitch), Ogre::Vector3::UNIT_X);
    Ogre::Quaternion r(Ogre::Degree(roll), Ogre::Vector3::UNIT_Z);
    this->node->rotate(y*p*r);
    this->update();
}

void Entity::update()
{
    this->x =  this->node->getPosition().x;
    this->y = this->node->getPosition().y;
    this->z = this->node->getPosition().z;

    this->yaw = this->node->getOrientation().getYaw().valueDegrees();
    this->pitch = this->node->getOrientation().getPitch().valueDegrees();
    this->roll = this->node->getOrientation().getRoll().valueDegrees();
}
std::string Entity::getName() const
{
    return name;
}

void Entity::setName(const std::string &value)
{
    name = value;
}
double Entity::getX() const
{
    return x;
}

void Entity::setX(double value)
{
    x = value;
}
double Entity::getY() const
{
    return y;
}

void Entity::setY(double value)
{
    y = value;
}
double Entity::getZ() const
{
    return z;
}

void Entity::setZ(double value)
{
    z = value;
}
double Entity::getYaw() const
{
    return yaw;
}

void Entity::setYaw(double value)
{
    yaw = value;
}
double Entity::getPitch() const
{
    return pitch;
}

void Entity::setPitch(double value)
{
    pitch = value;
}
double Entity::getRoll() const
{
    return roll;
}

void Entity::setRoll(double value)
{
    roll = value;
}
Ogre::Entity *Entity::getEnt() const
{
    return ent;
}

void Entity::setEnt(Ogre::Entity *value)
{
    ent = value;
}

Ogre::SceneNode *Entity::getNode() const
{
    return node;
}
void Entity::setNode(Ogre::SceneNode *value)
{
    node = value;
}








