#include "Entity.hpp"


Entity::Entity(std::string _name, std::string _mesh, Ogre::SceneManager* scene)
{
    if ((_name.length() < 1)) {
        std::cerr << "Error: createEntity: invalid name" << std::endl;
        return;
    }
    this->name = _name;
    this->ent  = scene->createEntity(_name, _mesh);
    this->node = scene->getRootSceneNode()->createChildSceneNode();
    this->node->attachObject(ent);
}

void Entity::setPosition(float x, float y, float z)
{
   this->node->setPosition(Ogre::Vector3(x, y, z));
}

void Entity::setPosition(Ogre::Vector3 pos)
{
    this->node->setPosition(pos);
}

void Entity::move(float x, float y, float z)
{
    this->node->translate(Ogre::Vector3(x, y, z));
}

void Entity::move(Ogre::Vector3 mvt)
{
      this->node->translate(mvt);
}

void Entity::setRotation(float yaw, float pitch, float roll)
{
    Ogre::Quaternion y(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
    Ogre::Quaternion p(Ogre::Degree(pitch), Ogre::Vector3::UNIT_X);
    Ogre::Quaternion r(Ogre::Degree(roll), Ogre::Vector3::UNIT_Z);
    this->node->setOrientation(y*p*r);
}

void Entity::rotate(float yaw, float pitch, float roll)
{
    Ogre::Quaternion y(Ogre::Degree(yaw), Ogre::Vector3::UNIT_Y);
    Ogre::Quaternion p(Ogre::Degree(pitch), Ogre::Vector3::UNIT_X);
    Ogre::Quaternion r(Ogre::Degree(roll), Ogre::Vector3::UNIT_Z);
   this->node->rotate(y*p*r);
}
