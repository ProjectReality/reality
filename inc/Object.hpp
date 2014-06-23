#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include "Entity.hpp"


/**
* @brief Class child of Entity, used for 3D assets used for the augmented reality
* @details This class contain all usefull informations for positionned an object in the 3D scene according to the marker tracked for this object.
*
*/

class Object : public Entity
{
public:
    using Entity::Entity;
    Object(std::string _name, std::string _mesh, Ogre::SceneManager* scene);
    void updateData(aruco::Marker info);


    int getPatt_id() const;
    void setPatt_id(int value);

private:
    std::string		assetName;
    int				patt_id;
};

 std::ostream& operator<<(std::ostream& os, const Object& obj);

#endif
