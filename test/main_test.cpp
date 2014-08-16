#define BOOST_TEST_MODULE Ogre Test
#include <boost/test/unit_test.hpp>

#include "OgreRenderer.hpp"

using namespace boost;
using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(Ogre_test) {
    double  video_size[2];
    video_size[0] = 800;
    video_size[1] = 600;

    OgreRenderer o(video_size, 0);

}
