#define NOMINMAX

#include "SensorInput.hpp"

using namespace OVR;
using namespace std;

SensorInput::SensorInput(Ogre::SceneManager *sm, Ogre::RenderWindow *win, Ogre::SceneNode *parent) :sensorFusion(0),
stereoConfig(0),
hmd(0),
deviceManager(0),
oculusReady(false),
ogreReady(false),
sensor(0)
{
	sceneManager = sm;
	window = win;
	cameraNode = parent;
}

SensorInput::~SensorInput()
{
	shutDownOculus();
}

bool SensorInput::setupOculus()
{
	if (oculusReady)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus: Already Initialised");
		return true;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus: Initialising system");
	System::Init(Log::ConfigureDefaultLog(LogMask_All));
	deviceManager = DeviceManager::Create();
	if (!deviceManager)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus: Failed to create Device Manager");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus: Created Device Manager");
	stereoConfig = new Util::Render::StereoConfig();
	if (!stereoConfig)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus: Failed to create StereoConfig");
		return false;
	}
	centreOffset = stereoConfig->GetProjectionCenterOffset();
	Ogre::LogManager::getSingleton().logMessage("Oculus: Created StereoConfig");
	hmd = deviceManager->EnumerateDevices<HMDDevice>().CreateDevice();
	if (!hmd)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus: Failed to create HMD");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus: Created HMD");
	HMDInfo devinfo;
	hmd->GetDeviceInfo(&devinfo);
	stereoConfig->SetHMDInfo(devinfo);
	sensor = hmd->GetSensor();
	if (!sensor)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus: Failed to create sensor");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus: Created sensor");
	sensorFusion = new SensorFusion();
	if (!sensorFusion)
	{
		Ogre::LogManager::getSingleton().logMessage("Oculus: Failed to create SensorFusion");
		return false;
	}
	Ogre::LogManager::getSingleton().logMessage("Oculus: Created SensorFusion");
	sensorFusion->AttachToSensor(sensor);
	oculusReady = true;
	Ogre::LogManager::getSingleton().logMessage("Oculus: Oculus setup completed successfully");
	return true;
}

void SensorInput::shutDownOculus()
{
	oculusReady = false;

	delete stereoConfig;
	stereoConfig = 0;
	delete sensorFusion;
	sensorFusion = 0;

	if (sensor)
	{
		sensor->Release();
	}
	if (hmd)
	{
		hmd->Release();
		hmd = 0;
	}
	if (deviceManager)
	{
		deviceManager->Release();
		deviceManager = 0;
	}

	System::Destroy();
}

// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Using+OIS
bool SensorInput::startUpOIS()
{
	OVR_ASSERT(!input_mgr);
	OVR_ASSERT(!keyboard);
	OVR_ASSERT(!mouse);
	OVR_ASSERT(joystick_list.size() == 0);

	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	
	window->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	input_mgr = OIS::InputManager::createInputSystem(pl);

	bool buffered_mode = true;

	if (input_mgr->getNumberOfDevices(OIS::OISKeyboard) > 0) {
		keyboard = static_cast<OIS::Keyboard*>(input_mgr->createInputObject(OIS::OISKeyboard, buffered_mode));
	}

	/*if (input_mgr->getNumberOfDevices(OIS::OISMouse) > 0) {
		mouse = static_cast<OIS::Mouse*>(input_mgr->createInputObject(OIS::OISMouse, buffered_mode));
		const OIS::MouseState &info_source = mouse->getMouseState();
		info_source.width = window->getWidth();
		info_source.height = window->getHeight();
	}*/

	if (input_mgr->getNumberOfDevices(OIS::OISJoyStick) > 0) {
		joystick_list.resize(input_mgr->getNumberOfDevices(OIS::OISJoyStick));

		for (size_t i = 0; i < joystick_list.size(); ++i) {
			joystick_list[i] = static_cast<OIS::JoyStick*>(input_mgr->createInputObject(OIS::OISJoyStick, buffered_mode));
		}
	}

	return true;
}

void SensorInput::captureInput()
{
	keyboard->capture();
	mouse->capture();
	for (auto joystick : joystick_list) {
		joystick->capture();
	}
}

bool SensorInput::shutDownOIS()
{
	if (input_mgr) {
		if (mouse) {
			input_mgr->destroyInputObject(mouse);
			mouse = nullptr;
		}
		if (keyboard) {
			input_mgr->destroyInputObject(keyboard);
			keyboard = nullptr;
		}

		if (joystick_list.size() > 0) {
			for (auto joystick : joystick_list) {
				input_mgr->destroyInputObject(joystick);
			}
			joystick_list.clear();
		}

		OIS::InputManager::destroyInputSystem(input_mgr);
		input_mgr = nullptr;
	}

	return true;
}

Ogre::SceneNode *SensorInput::getCameraNode()
{
	return cameraNode;
}

Ogre::Quaternion SensorInput::getOrientation() const
{
	if (oculusReady)
	{
		Quatf hmdOrient = sensorFusion->GetOrientation();
		return Ogre::Quaternion(hmdOrient.w, hmdOrient.x, hmdOrient.y, hmdOrient.z);
	}
	else
	{
		return Ogre::Quaternion::IDENTITY;
	}
}

Ogre::Vector3 SensorInput::GetEulerAnglesSensor() const
{
	float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;

		Quatf hmdOrient = sensorFusion->GetOrientation();
		hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll);
		return Ogre::Vector3(yaw, pitch, roll);
}

Ogre::Vector3 SensorInput::getAccelerationData()
{
	return Ogre::Vector3(sensorFusion->GetAcceleration().x, sensorFusion->GetAcceleration().y, sensorFusion->GetAcceleration().z);
}

Ogre::Vector3 SensorInput::getAngularVelocityData()
{
	return Ogre::Vector3(sensorFusion->GetAngularVelocity().x, sensorFusion->GetAngularVelocity().y, sensorFusion->GetAngularVelocity().z);
}

Ogre::Vector3 SensorInput::getMagnetometerData()
{
	return Ogre::Vector3(sensorFusion->GetMagnetometer().x, sensorFusion->GetMagnetometer().y, sensorFusion->GetMagnetometer().z);
}

// Delete this function in final release (Just use it to capture data)
void SensorInput::getSensorData()
{
	ofstream outFile;
	outFile.open("SensorData.txt", std::ofstream::app);
	if (outFile.is_open())
	{
		outFile << "Acceleration Data : x [" << sensorFusion->GetAcceleration().x << "], y [" << sensorFusion->GetAcceleration().y << "], z [" << sensorFusion->GetAcceleration().z << "]\n" <<
			"Gyro Data : x [" << sensorFusion->GetAngularVelocity().x << "], y [" << sensorFusion->GetAngularVelocity().y << "], z [" << sensorFusion->GetAngularVelocity().z << "]\n" <<
			"Magne Data : x [" << sensorFusion->GetMagnetometer().x << "], y [" << sensorFusion->GetMagnetometer().y << "], z [" << sensorFusion->GetMagnetometer().z << "]\n" <<
			"Radian Orient Data : x [" << sensorFusion->GetOrientation().x << "], y [" << sensorFusion->GetOrientation().y << "], z [" << sensorFusion->GetOrientation().z << "], w [" << sensorFusion->GetOrientation().w << "]\n" <<
			"Degree Orient Data : x [" << RadToDegree(sensorFusion->GetOrientation().x) << "], y [" << RadToDegree(sensorFusion->GetOrientation().y) << "], z [" << RadToDegree(sensorFusion->GetOrientation().z) << "], w [" << RadToDegree(sensorFusion->GetOrientation().w) << "]\n\n";
	}
	outFile.close();
}