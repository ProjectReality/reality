#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include <string>
#include <Ogre.h>

class Entity
{   
public:
    Entity(std::string _name);
    ~Entity();

    void setPosition(float x, float y, float z);
    void setPosition(Ogre::Vector3 pos);

    void move(float x, float y, float z);
    void move(Ogre::Vector3 mvt);

    void setRotation(float yaw, float pitch, float roll);
    void rotate(float yaw, float pitch, float roll);

    void update();

    std::string getName() const;
    void setName(const std::string &value);

    double getX() const;
    void setX(double value);

    double getY() const;
    void setY(double value);

    double getZ() const;
    void setZ(double value);

    double getYaw() const;
    void setYaw(double value);

    double getPitch() const;
    void setPitch(double value);

    double getRoll() const;
    void setRoll(double value);

private:
    std::string         name;
    Ogre::SceneManager* scene;
    Ogre::SceneNode*    node;
    Ogre::Entity*       ent;
    double              x;
    double              y;
    double              z;
    double              yaw;
    double              pitch;
    double              roll;
};

#endif
