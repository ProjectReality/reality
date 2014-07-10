#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include "Entity.hpp"


/**
* @brief Class child of Entity, used for 3D assets used for the augmented reality
* @details This class contain all usefull informations for positionned an object in the 3D scene according to the marker tracked for this object.
*
*/

class Object : public Reality::Entity
{
public:
    using Entity::Entity;

    /**
     * @brief Object
     * @param _name
     * @param _mesh
     * @param scene
     */
    Object(std::string _name, std::string _mesh, Ogre::SceneManager* scene);

    void updateData(aruco::Marker info);


    /**
     * @brief getPatt_id
     * @return
     */
    int getPatt_id() const;

    /**
     * @brief setPatt_id
     * @param value
     */
    void setPatt_id(int value);

private:
    std::string		assetName;
    int				patt_id;
};

 std::ostream& operator<<(std::ostream& os, const Object& obj);

#endif
