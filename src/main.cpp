#include	"OgreRenderer.hpp"
#include	"StereoCamera.hh"

int		main()
{
  IRenderer*	render;
  StereoCamera	camera;
  cv::Mat*		frame;
  
  render = new OgreRenderer();
  camera.OpenCamera();

  // Scene creation
  render->createEntity("Test", "");
  
  // Free grab & get to get rid of the first frame
  camera.GrabFrames();                                                                                                                                                                      

  // Render loop
  while(render->isAlive())
    {
	  if (camera.FrameAvailable()) {
		  frame = camera.GetFrame();
		  render->loadCam(frame[0], frame[1]);
		  boost::thread new_pic(&StereoCamera::camWorker, camera);
	  }
      render->rotateEntity("Test", 0, 1, 0);
      render->render();
    }
  delete render;
  return 0;
}

// invest: utiliser le même flux webcam pour les deux viewports en le dćalant
