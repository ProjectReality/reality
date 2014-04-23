#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include "ARma/pattern.h"
#include "Entity.hpp"

class Object : public Entity
{
public:
    using Entity::Entity;
private:
    ARma::Pattern	info;
    cv::Mat			patt_frame;
    std::string		patternName;
    std::string		assetName;
    int				patt_id;
};

#endif
