# Reality Project

- - -

# Windows & Linux

## Build Requirements

* Ogre3D 1.9 ([WindowsVC12](https://bitbucket.org/realityproject/reality/downloads/OgreSDK_vc12_v1-9-0.zip) ([Linux Source](https://bitbucket.org/realityproject/reality/downloads/sinbad-ogre-dd30349ea667.zipogre-1.9-source.zipogre-source-1.9.tar.gz))   <[http://www.ogre3d.org/](http://www.ogre3d.org/)>
* OpenCV 2.4.8 ([Windows](https://bitbucket.org/realityproject/reality/downloads/opencv-2.4.8.exe)) ([Linux](https://bitbucket.org/realityproject/reality/downloads/opencv-linux-2.4.8.zip))  <[http://opencv.org/](http://opencv.org/)>
* Boost 1.55 ([All](https://bitbucket.org/realityproject/reality/downloads/boost_1_55.zip))  <[http://www.boost.org/](http://www.boost.org/)>
* Cmake  <[http://www.cmake.org](http://www.cmake.org)>
* Oculus SDK 0.2.5c ([Windows](https://bitbucket.org/realityproject/reality/downloads/ovr_sdk_win_0.2.5c.zip)) ([Linux](https://bitbucket.org/realityproject/reality/downloads/ovr_sdk_linux_0.2.5c.tar.gz))  <[https://developer.oculusvr.com/](https://developer.oculusvr.com/?action=dl)>
* Aruco <[http://sourceforge.net/projects/aruco/files/)>

# Windows

You can find everything you need in there : [realityproject/reality/downloads](https://bitbucket.org/realityproject/reality/downloads)

## Env setup


```
#!
BOOST_INC=C:\Users\vayan\Libs\boost_1_55
BOOST_ROOT=C:\Users\vayan\Libs\boost_1_55
CMAKE_MODULE_PATH=C:\Users\vayan\Libs\OgreSDK\CMake
OCULUS_SDK_ROOT_DIR=C:\Users\vayan\Libs\OculusSDK
OGRE_HOME=C:\Users\vayan\Libs\OgreSDK
OPENCV_DIR=C:\Users\vayan\Libs\opencv\build
OPENCV_ROOT=C:\Users\vayan\Libs\opencv\build
ARUCO_ROOT=C:\Users\vayan\Libs\aruco\build
```

Maybe you don't need every env but I don't remember which one work :)

# Linux

## Env setup

Copy the OculusSDK folder in /usr/lib and it'll work.

If you have some error about plugins you'll maybe need to compile Ogre 1.9 Plugin_CgProgramManager 
