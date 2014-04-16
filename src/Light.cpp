#include "Light.hpp"

Light::Light(Ogre::SceneManager* scene)
{
    this->scene = scene;
    GetTime();
}

Light::~Light()
{

}

void Light::GetTime() {
    using namespace boost::gregorian;

    date today = day_clock::local_day();

    std::cout << "Today is " << to_simple_string(today) << std::endl;
}
