#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include <string>
#include <Ogre.h>


/**
 * @brief The Entity class
 */
class Entity
{
public:
    /**
     * @brief Entity
     * @param _name
     */
    Entity(std::string _name);
    ~Entity();

    /**
     * @brief setPosition
     * @param x
     * @param y
     * @param z
     */
    void setPosition(float x, float y, float z);

    /**
     * @brief setPosition
     * @param pos
     */
    void setPosition(Ogre::Vector3 pos);

    /**
     * @brief move
     * @param x
     * @param y
     * @param z
     */
    void move(float x, float y, float z);

    /**
     * @brief move
     * @param mvt
     */
    void move(Ogre::Vector3 mvt);

    /**
     * @brief setRotation
     * @param yaw
     * @param pitch
     * @param roll
     */
    void setRotation(float yaw, float pitch, float roll);

    /**
     * @brief rotate
     * @param yaw
     * @param pitch
     * @param roll
     */
    void rotate(float yaw, float pitch, float roll);

    /**
     * @brief update
     */
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

    Ogre::Entity *getEnt() const;
    void setEnt(Ogre::Entity *value);

    Ogre::SceneNode *getNode() const;
    void setNode(Ogre::SceneNode *value);

    Ogre::SceneManager *getScene() const;
    void setScene(Ogre::SceneManager *value);

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
