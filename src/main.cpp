#include  "OgreRenderer.hpp"
#include  "StereoCamera.hpp"
#include  "Oculus.hpp"
#include  "Light.hpp"

int   main()
{
    IRenderer*  render;
    StereoCamera  camera;
    cv::Mat*    frame;
    double    video_size[2];

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
    l->GetPositionSun(1997,8,7,11,0);

    // Render loop
    while(render->isAlive())
    {
        if (render->getShutDown())
            return 0;

        if (camera.FrameAvailable())
        {
            frame = camera.GetFrame();
            render->loadCam(frame[0], frame[1]);
            boost::thread new_pic(&StereoCamera::camWorker, camera);
        }
        render->rotateEntity("Test", 0, 1, 0);
        render->moveEntity("Test", 0.6, 0, 0);
        render->render();
    }
    delete render;
    return 0;
}

// invest: utiliser le même flux webcam pour les deux viewports en le dćalant
