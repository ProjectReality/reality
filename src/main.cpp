#include	"OgreRenderer.hpp"
#include	"StereoCamera.hpp"
#include	"Oculus.hpp"
#include	"ARManager.hpp"
#include	<math.h> 

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
    //render->createEntity("Test", "Cube.mesh");
    render->createEntity("Test2", "Spider.mesh");

    // Free grab & get to get rid of the first frame
    camera.GrabFrames();

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
				render->setRotationEntity("Test2", pat.getYaw(), pat.getPitch(), pat.getRoll());
				render->setPosEntity("Test2", pat.getInfo().transVec.at<float>(0)*5, -pat.getInfo().transVec.at<float>(1)*5, -pat.getInfo().transVec.at<float>(2));
            }
            render->loadCam(frame[0], frame[1]);
            boost::thread new_pic(&StereoCamera::camWorker, camera);
		}
		//render->moveEntity("Test", 0.2, 0.2, 0);
        render->render();
    }
    ar.stop();
    delete render;
    return 0;
}


