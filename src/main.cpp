#include	"OgreRenderer.hpp"
#include	"StereoCamera.hpp"
#include	"Oculus.hpp"
#include	"ARManager.hpp"


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
              ar.draw(frame[0]);
              ar.draw(frame[1]);
              if (ARManager::verbose)
              {
                std::list<AssetInfo>    markerFound = ar.getMarkers();
                ARma::Pattern pat = markerFound.front().getInfo();
                std::cout << "RotVec : " << pat.rotVec << std::endl;
                std::cout << "orientation : " << pat.orientation << std::endl;
                std::cout << "rotMat : " << pat.rotMat << std::endl;
              }
            }

            render->loadCam(frame[0], frame[1]);
            boost::thread new_pic(&StereoCamera::camWorker, camera);
        }
        render->rotateEntity("Test", 0, 1, 0);
        render->moveEntity("Test", 0.6, 0, 0);
        render->render();
    }
    ar.stop();
    delete render;
    return 0;
}


