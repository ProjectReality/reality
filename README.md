# Reality Project

- - -

# Windows & Linux

## Build Requirements

* Ogre3D 1.9 ([WindowsVC12](https://bitbucket.org/realityproject/reality/downloads/OgreSDK_vc12_v1-9-0.zip) ([Linux Source](https://bitbucket.org/realityproject/reality/downloads/sinbad-ogre-dd30349ea667.zipogre-1.9-source.zipogre-source-1.9.tar.gz))   <[ogre3d.org](http://www.ogre3d.org/)>
* OpenCV 2.4.8 ([Windows](https://bitbucket.org/realityproject/reality/downloads/opencv-2.4.8.exe)) ([Linux](https://bitbucket.org/realityproject/reality/downloads/opencv-linux-2.4.8.zip))  <[opencv.org](http://opencv.org/)>
* Boost 1.55 ([All](https://bitbucket.org/realityproject/reality/downloads/boost_1_55.zip))  <boost.org/](http://www.boost.org/)>
* Cmake  <[cmake.org](http://www.cmake.org)>
* Oculus SDK 0.2.5c ([Windows](https://bitbucket.org/realityproject/reality/downloads/ovr_sdk_win_0.2.5c.zip)) ([Linux](https://bitbucket.org/realityproject/reality/downloads/ovr_sdk_linux_0.2.5c.tar.gz))  <[developer.oculusvr.com](https://developer.oculusvr.com/?action=dl)>
* Aruco <[Aruco Source Files](http://sourceforge.net/projects/aruco/files/)>
* Librocket <[librocket.com](http://librocket.com/)>
* Google Test <[code.google.com/p/googletest](https://code.google.com/p/googletest/)>

# Windows

You can find everything you need in there : [realityproject/reality/downloads](https://bitbucket.org/realityproject/reality/downloads)

You need to build librocket first :

get the latest source https://github.com/libRocket/libRocket

download freetype : https://bitbucket.org/realityproject/reality/downloads/freetype-2.4.10.tar.bz2

then read the README in https://github.com/libRocket/libRocket/tree/master/Build/vc2012

You also need the libboost_program_options.lib on windows you can find the binary here :  https://bitbucket.org/realityproject/reality/downloads/libboost_program_options.zip

just copy all the libboost_program_options*.lib where you had your others boost libs.


For Google Test you have to build it of course, but be careful to build gtest-md not the other one.

## Env setup

```
#!
BOOST_ROOT=C:\Users\vayan\Libs\boost_1_55
CMAKE_MODULE_PATH=C:\Users\vayan\Libs\OgreSDK\CMake
OCULUS_SDK_ROOT_DIR=C:\Users\vayan\Libs\OculusSDK
OGRE_HOME=C:\Users\vayan\Libs\OgreSDK
OPENCV_DIR=C:\Users\vayan\Libs\opencv\build
ARUCO_ROOT=C:\Users\vayan\Libs\aruco\build
ROCKETDIR=XX:\foo\libRocket\bin
GTEST_ROOT=X:\Dev\gtest-1.7.0
```
Maybe you don't need every env but I don't remember which one work :)


# Linux

If you install Google Test from the package mnger you need to build it too, it only download the source.

## Env setup

Copy the OculusSDK folder in /usr/lib and it'll work.

If you have some error about plugins you'll maybe need to compile Ogre 1.9 Plugin_CgProgramManager
