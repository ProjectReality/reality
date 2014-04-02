#include	"OgreRenderer.hpp"
#include	"StereoCamera.hpp"
#include	"Oculus.hpp"
#include	"ARManager.hpp"

int		main()
{
  IRenderer*	render;
  StereoCamera	camera;
  cv::Mat*		frame;
  double		video_size[2];
  ARManager		ar;

  VirtualOculus *rift = new VirtualOculus();
  rift = rift->Init();

  //ugly test
  float test = rift->getDistordScale();
  int xwindow = rift->getResolution()[0];
  int ywindow = rift->getResolution()[1];
  //StereoEyeParams* eyes = rift->getEyesParams();

  camera.OpenCamera();
	
  video_size[0] = camera.CameraGet(CV_CAP_PROP_FRAME_WIDTH, 0);
  video_size[1] = camera.CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 0);

  render = new OgreRenderer(video_size, rift);
 

  // Scene creation
  render->createEntity("Test", "");
  
  // Free grab & get to get rid of the first frame
  camera.GrabFrames();                                                                                                                                                                      

  // AR init
  ar.init();
  ar.start();

  // Render loop
  while(render->isAlive())
    {
	  if (ARManager::verbose)
	  {
		  if (ar.isChanged())
		  {
			  std::list<AssetInfo> markers = ar.getMarkers();
			  for (std::list<AssetInfo>::iterator it = markers.begin(); it != markers.end(); it++)
			  {
				  std::cout << "Marker : " << it->getPattName() << std::endl;
			  }
		  }
	  }
	  if (camera.FrameAvailable()) {
		  int				markerNum = -1;

		  frame = camera.GetFrame(); 
		  cv::Mat myFrame;
		  frame->convertTo(myFrame, CV_8U);
		  ar.setFrame(frame[0]);
		  if (ar.isChanged())
		  {
			  ar.draw(frame[0]);
		  }
		  render->loadCam(frame[0], frame[1]);
		  boost::thread new_pic(&StereoCamera::camWorker, camera);
	  }
      render->rotateEntity("Test", 0, 1, 0);
      render->render();
  }
  ar.stop();
  delete render;
  return 0;
}


