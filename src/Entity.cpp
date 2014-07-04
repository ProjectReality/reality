#include "Entity.hpp"


Entity::Entity(std::string _name)
{
    if ((_name.length() < 1)) {
        std::cerr << "Error: createEntity: invalid name" << std::endl;
        return;
    }
    this->name = _name;
	this->base_yaw = 0;
	this->base_roll = 0;
	this->base_pitch = 0;
}

Entity::~Entity()
{
    if (this->scene && this->scene->hasEntity(this->name))
        this->scene->destroyEntity(this->name);
}

void Entity::setPosition(float x, float y, float z)
{
    this->node->setPosition(Ogre::Vector3(x, y, z));
	this->x = x;
	this->y = y;
	this->z = z;
    this->update();
}

void Entity::setPosition(Ogre::Vector3 pos)
{
    this->node->setPosition(pos);
    this->update();
}

void Entity::setScale(Ogre::Vector3 pos)
{
	this->node->setScale(pos);
	this->update();
}

void Entity::setOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	this->node->setOrientation(w, x, y, z);
	this->update();
}

void Entity::setOrientation(Ogre::Quaternion pos)
{
	this->node->setOrientation(pos);
	this->update();
}

void Entity::move(float x, float y, float z)
{
    this->node->translate(Ogre::Vector3(x, y, z));
    this->update();
}

void Entity::rotate(Ogre::Vector3 v, Ogre::Radian r)
{
	this->node->rotate(v, r);
	this->update();
}

void Entity::move(Ogre::Vector3 mvt)
{
    this->node->translate(mvt);
    this->update();
}

void Entity::setRotation(float yaw, float pitch, float roll)
{
	Ogre::Quaternion y(Ogre::Degree(yaw + base_yaw), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion p(Ogre::Degree(pitch + base_pitch), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion r(Ogre::Degree(roll + base_roll), Ogre::Vector3::UNIT_Z);
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


int Entity::getId() const
{
	return id;
}

void Entity::setId(int value)
{
	id = value;
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

void Entity::visible(bool h)
{
	ishide = !h;
	this->node->setVisible(h, true);
}

bool Entity::isHide()
{
	return ishide;
}


bool Entity::isAVisible() const
{
	return (always_visible);
}

void Entity::setAVisible(bool v)
{
	always_visible = v;
}

double Entity::getBaseRoll() const
{
	return base_roll;
}

void Entity::setBaseRoll(double value)
{
	base_roll = value;
}

double Entity::getBasePitch() const
{
	return base_pitch;
}

void Entity::setBasePitch(double value)
{
	base_pitch = value;
}

double Entity::getBaseYaw() const
{
	return base_yaw;
}

void Entity::setBaseYaw(double value)
{
	base_yaw = value;
}
