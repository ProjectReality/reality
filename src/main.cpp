#include <cmath>
#include <map>
#include <Rocket/Core.h>


#include	"OgreRenderer.hpp"
#include	"StereoCamera.hpp"
#include	"Oculus.hpp"
#include	"ARManager.hpp"
#include    "Object.hpp"
#include    "Overlay.hpp"

int   main()
{
    OgreRenderer*  render;
    StereoCamera  camera;
    cv::Mat*    frame;
    double    video_size[2];
    ARManager   ar;
    std::map<std::string, Object*> objects;

    VirtualOculus *rift = new VirtualOculus();
    rift = rift->Init();

    //ugly test
    float test = rift->getDistordScale();
    int xwindow = rift->getResolution()[0];
    int ywindow = rift->getResolution()[1];

    camera.OpenCamera();

    video_size[0] = camera.CameraGet(CV_CAP_PROP_FRAME_WIDTH, 0);
    video_size[1] = camera.CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 0);

    render = new OgreRenderer(video_size, rift);


    render->startUI();
    render->startRealityRender();


    //Overlay *hud = new Overlay();

    // Scene creation
    objects["Test"] = new Object("Test", "EarthGlobe.mesh", render->getScene());

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
                objects["Test"]->updateData(ar.getMarkers().front());
            }
            render->loadCam(frame[0], frame[1]);
            boost::thread new_pic(&StereoCamera::camWorker, camera);
        }
        render->render();
    }
    ar.stop();
    delete render;
    return 0;
}


