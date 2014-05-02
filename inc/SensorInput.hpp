#pragma once

#include "OgreQuaternion.h"
#include "OgreVector3.h"

namespace OVR
{
	class HMDDevice;
	class SensorFusion;
	class DeviceManager;
	class SensorDevice;

	namespace Util
	{
		namespace Render
		{
			class StereoConfig;
		}
	}
}

namespace OIS
{
	class InputManager;
	class Keyboard;
	class Mouse;
	class JoyStick;
}

class SensorInput
{
public:
	SensorInput(Ogre::SceneManager *sm, Ogre::RenderWindow *win, Ogre::SceneNode *parent = 0);
	~SensorInput();
	bool setupOculus();
	void shutDownOculus();
	bool startUpOIS();
	bool shutDownOIS();
	void captureInput();

	/// Retrieve the SceneNode that contains the two cameras used for stereo rendering.
	Ogre::SceneNode *getCameraNode();

	/// Retrieve the current orientation of the Oculus HMD.
	Ogre::Quaternion getOrientation() const;

	/// Retrieve the Euler Angles of the Oculus HMD.
	Ogre::Vector3 GetEulerAnglesSensor() const;

	/// Retrieve the acceleration of the Oculus HMD.
	Ogre::Vector3 getAccelerationData();

	/// Retrieve the Velocity Angle of the Oculus HMD.
	Ogre::Vector3 getAngularVelocityData();

	/// Retrieve the Magnetude of the Oculus HMD.
	Ogre::Vector3 getMagnetometerData();

	/// Provide data in an output file.
	/// Delete this function in final release (Just use it to capture data)
	void getSensorData();

protected:
	OVR::DeviceManager *deviceManager;
	OVR::HMDDevice *hmd;
	OVR::Util::Render::StereoConfig *stereoConfig;
	OVR::SensorDevice *sensor;
	OVR::SensorFusion *sensorFusion;
	OIS::InputManager *input_mgr;
	OIS::Keyboard *keyboard;
	OIS::Mouse *mouse;
	std::vector<OIS::JoyStick*> joystick_list;
	OIS::JoyStick *main_joystick();
	OIS::JoyStick *getJoystick(int idx);
	Ogre::SceneManager *sceneManager;
	Ogre::RenderWindow *window;
	Ogre::SceneNode *cameraNode;
	Ogre::Quaternion orientation;
	Ogre::Camera *cameras[2];
	Ogre::Viewport *viewports[2];
	Ogre::CompositorInstance *compositors[2];
	bool oculusReady;
	bool ogreReady;
	float centreOffset;

public:

	OIS::JoyStick *main_joystick()
	{
		if (joystick_list.size() == 0) {
			return nullptr;
		}
		return joystick_list[0];
	}

	OIS::JoyStick *getJoystick(int idx)
	{
		if (idx < joystick_list.size()) {
			return joystick_list[idx];
		}
		return nullptr;
	}
};