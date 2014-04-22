#include <cmath>

#include "Light.hpp"

Light::Light(Ogre::SceneManager* scene)
{
    this->scene = scene;
    getTime();
}

Light::~Light()
{

}

void   Light::CreateSunLight(std::string name, Ogre::Vector3 pos)
{
    //1200, 100, 600
    Ogre::Light* sunLight = scene->createLight(name);
    sunLight->setPosition(pos);
    sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
    sunLight->setDiffuseColour(.35, .35, 0.38);
    sunLight->setSpecularColour(.9, .9, 1);

    Ogre::Vector3 dir(-1, -1, 0.5);
    dir.normalise();
    sunLight->setDirection(dir);
    sunLight->setCastShadows(true);
    Lights.push_back(sunLight);
}

double Light::meanLong(double x)
{
    float tpi = 2 * 3.1415926;

    float b = x / tpi;
    float a = tpi * (b - ((b > 0) ? 1 : (((b < 0) ? -1 : 0) * floor(fabs(b)))));
    if (a < 0)
        a = tpi + a;
    return a;
}

double Light::GetPositionSun(int year, int month, int day, int hour, int mins)
{
    //some constant
    double pi = 3.1415926;
    double rads = pi / 180;
    double degs = 180 / pi;
    double hourUT = hour + mins / 60;

    /*
     * Get the days to J2000
     * hourUT is UT in decimal hours
     * only work until 2099 :/
      */
    double d = 367 * year - 7 * (year + (month + 9) / 12) / 4 +
            275 * month / 9 + day - 730531.5 + hourUT / 24;

    //Obliquity of the ecliptic
    double obliq = 23.439 * rads - .0000004 * rads * d;

    //Mean Longitude of the Sun (mLongSun)
    double mLongSun = meanLong(280.461 * rads + .9856474 * rads * d);

    //Mean anomaly of the Sun (mAnoSun)
    double mAnoSun = meanLong(357.528 * rads + .9856003 * rads * d);

    //Ecliptic longitude of the sun (lambda)
    double lambda = meanLong(mLongSun + 1.915 * rads * sin(mAnoSun) + .02 * rads * sin(2 * mAnoSun));

    //Right Ascension (alpha) and Declination (delta) of the Sun
    double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
    double delta = asin(sin(obliq) * sin(lambda));

    //Earth - Sun distance
    double r = 1.00014 - .01671 * cos(mAnoSun) - .00014 * cos(2 * mAnoSun);

    //Equation of Ocarina of Time
    double eqtime = (mLongSun - alpha) * degs * 4; // equation of time

    double longi = lambda * degs; //longitude
    double RA = alpha * degs / 15; //Right Ascension
    double DEC = delta * degs; //Declination

    return RA;
}

int Light::getUTC(int localhour, int timezone)
{
    return localhour + timezone;
}

void Light::getTime()
{
    using namespace boost::gregorian;

    date today = day_clock::local_day();

    std::cout << "Today is " << to_simple_string(today) << std::endl;
}
