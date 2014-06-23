#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include <string>
#include <Ogre.h>
#include "aruco.h"

/**
* @brief Class for all asset of the project
* @details This class contain all usefull information for positionned an asset in the 3D scene.
* 
*/

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
	void rotate(Ogre::Vector3 v, Ogre::Radian r);
    void rotate(float yaw, float pitch, float roll);

	void setScale(Ogre::Vector3 pos); 
	void setOrientation(Ogre::Quaternion pos);
	void setOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void update();

    std::string getName() const;
	void setName(const std::string &value);

	int getId() const;
	void setId(int value);

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

    Ogre::Entity *getEnt() const;
    void setEnt(Ogre::Entity *value);

    Ogre::SceneNode *getNode() const;
    void setNode(Ogre::SceneNode *value);

    Ogre::SceneManager *getScene() const;
    void setScene(Ogre::SceneManager *value);

	void visible(bool h);
	bool isHide();

	bool isAVisible() const;
	void setAVisible(bool v);

	double getBaseRoll() const;
	void setBaseRoll(double value);

	double getBasePitch() const;
	void setBasePitch(double value);

	double getBaseYaw() const;
	void setBaseYaw(double value);

private:
	int					id;
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
	double              base_yaw;
	double              base_pitch;
	double              base_roll;
	bool				ishide;
	bool				always_visible;
};

#endif
