#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include <string>
#include <Ogre.h>

class Entity
{   
public:
    Entity(std::string _name, std::string _mesh, Ogre::SceneManager* scene);

    void setPosition(float x, float y, float z);
    void setPosition(Ogre::Vector3 pos);

    void move(float x, float y, float z);
    void move(Ogre::Vector3 mvt);

    void setRotation(float yaw, float pitch, float roll);
    void rotate(float yaw, float pitch, float roll);

private:
    std::string         name;
    Ogre::SceneManager* scene;
    Ogre::SceneNode*    node;
    Ogre::Entity*       ent;
};

#endif
