#include	"OgreRenderer.hpp"
#include	"StereoCamera.hh"

int		main()
{
  IRenderer*	render;
  StereoCamera	camera;
  cv::Mat*	frame;
  
  render = new OgreRenderer();
  camera.OpenCamera();


  //  cam = cv::imread("assets/camtest.jpg");

  
  // Scene creation
  render->createEntity("Test", "");
  
  // Free grab & get to get rid of the first frame
  camera.GrabFrames();                                                                                                                                                                      
  camera.GetFrame();                                                                                                                                                                      

  // Render loop
  while(render->isAlive())
    {
      camera.GrabFrames();                                                                                                                                                                      
      frame = camera.GetFrame();


      render->rotateEntity("Test", 0, 1, 0);
      render->loadCam(frame[0], frame[1]);
      render->render();
      // usleep(1000);
    }
  
  delete render;
  return 0;
}

// invest: utiliser le même flux webcam pour les deux viewports en le dćalant
