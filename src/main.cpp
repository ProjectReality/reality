#include	<cmath>

#include	"OgreRenderer.hpp"
#include	"StereoCamera.hpp"
#include	"Oculus.hpp"
#include	"ARManager.hpp"
#include    "Light.hpp"

int   main()
{
    IRenderer*  render;
    StereoCamera  camera;
    cv::Mat*    frame;
    double    video_size[2];
    ARManager   ar;

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
    render->createEntity("Test", "EarthGlobe.mesh");
    //render->createEntity("Test2", "WoodenChair.mesh");

    // Free grab & get to get rid of the first frame
    camera.GrabFrames();

    Light *l = new Light(render->getScene());
    l->getPositionSun(1997,8,7,11,0);
    // AR init
	ar.init();
	ar.start();


    // Render loop
    while(render->isAlive())
    {
        if (render->getShutDown())
            return 0;

        if (camera.FrameAvailable())
        {

            frame = camera.GetFrame();
            ar.setFrame(frame[0]);
            if (ar.isChanged())
            {
                std::list<AssetInfo>    markerFound = ar.getMarkers();
				AssetInfo pat = markerFound.front();
				std::cout << "_____________________________________________________________________" << std::endl;
				//pat.getInfo().showPattern();
				//std::cout << "_________________-----------------------------_______________________" << std::endl;
				//std::cout << "Rows = " << frame->rows << " Cols = " << frame->cols << std::endl;
				std::cout << pat << std::endl; 
				render->setRotationEntity("Test", pat.pitch, pat.roll, pat.yaw);
				render->setPosEntity("Test", pat.x, pat.y, pat.z);
            }
            render->loadCam(frame[0], frame[1]);
            boost::thread new_pic(&StereoCamera::camWorker, camera);
		}
		//render->setRotationEntity("Test", 0, 0, 0);
		//render->setPosEntity("Test", 1, 1, -200);
        render->render();
    }
    ar.stop();
    delete render;
    return 0;
}


