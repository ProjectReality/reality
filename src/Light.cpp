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

double Light::meanLong(double x){
    float pi = 3.1415926;
    float tpi = 2 * pi;

    float b = x / tpi;
    float a = tpi * (b - (floor(fabs(b))));
    if (a < 0)
        a += tpi;
    return a;
}

double Light::GetPositionSun(int year, int month, int day, int hourUT, int mins) {
    //some constant
    double pi = 3.1415926;
    double tpi = 2 * pi;
    double rads = pi / 180;
    double degs = 180 / pi;
    hourUT = hourUT + mins / 60;

    /*
     * Get the days to J2000
     * hourUT is UT in decimal hours
     * only work until 2099 :/
      */
    double d =
            367 * year - 7 * (year + (month + 9) / 12) / 4 +
            275 * month / 9 + day - 730531.5 + hourUT / 24;

    // Obliquity of the ecliptic
    double obliq = 23.439 * rads - .0000004 * rads * d;

    //Find the Mean Longitude of the Sun
    double mLongSun = meanLong(280.461 * rads + .9856474 * rads * d);

    //Find the Mean anomaly of the Sun
    double mAnoSun = meanLong(357.528 * rads + .9856003 * rads * d);

    //Find the ecliptic longitude of the sun
    double lambda = meanLong(mLongSun + 1.915 * rads * sin(mAnoSun) + .02 * rads * sin(2 * mAnoSun));


    //Find the Right Ascension (alpha) and Declination (delta) of the Sun
    double x = cos(obliq) * sin(lambda);
    double y = cos(lambda);
    double alpha = tan(x / y);
    if (x < 0)
        alpha += pi;
    if (y < 0 && x > 0)
        alpha += tpi;

    double delta = asin(sin(obliq) * sin(lambda));

    double r = 1.00014 - .01671 * cos(mAnoSun) - .00014 * cos(2 * mAnoSun);

    double equation = (mLongSun - alpha) * degs * 4;

    double longi = lambda * degs;
    double RA = alpha * degs / 15; //Right Ascension
    double DEC = delta * degs; //Declination

    return 0.0;
}

void Light::getTime()
{
    using namespace boost::gregorian;

    date today = day_clock::local_day();

    std::cout << "Today is " << to_simple_string(today) << std::endl;
}
