#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include "ARma/pattern.h"
#include "Entity.hpp"

/**
 * @brief The Object class
 */
class Object : public Entity
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

    /**
     * @brief updateData
     * @param info
     */
    void updateData(ARma::Pattern info);

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
    ARma::Pattern	info;
    cv::Mat			patt_frame;
    std::string		patternName;
    std::string		assetName;
    int				patt_id;
};

 std::ostream& operator<<(std::ostream& os, const Object& obj);

#endif
