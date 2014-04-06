#include    "StereoCamera.hpp"

StereoCamera::StereoCamera() {
    camera_open = false;
    frame_available = false;
}

StereoCamera::~StereoCamera() {
    if (camera_open) CloseCamera();
}

void StereoCamera::OpenCamera() {
    camera[LEFT].open(0);
    camera[RIGHT].open(1);

    if (!camera[LEFT].isOpened()) {
        std::cerr << "StereoCamera: OpenCamera() - Couldn't Open first Camera" << std::endl;
        exit(11);
    }
    if (!camera[RIGHT].isOpened()) {
        std::cerr << "StereoCamera: OpenCamera() - Couldn't Open second Camera" << std::endl <<
        "StereoCamera: OpenCamera() - Mirroring the first camera" << std::endl;
        camera[RIGHT] = camera[LEFT];
    }
    camera_open = true;
    frame_available = true;
}

void StereoCamera::CloseCamera() {
    if (!camera[LEFT].isOpened())  camera[LEFT].release();
    if (!camera[RIGHT].isOpened())  camera[RIGHT].release();
    camera_open = false;
    frame_available = false;
}

void StereoCamera::CameraSet(int param, double value, int camera_id) {
    if (camera_id < 0) {
        camera[camera_id].set(param, value);
    }
    else {
        camera[LEFT].set(param, value);
        camera[RIGHT].set(param, value);
    }
}

double StereoCamera::CameraGet(int param, int camera_id) {
    return camera[camera_id].get(param);
}

void StereoCamera::TestCamera() {
    for (int i = 0; i < 2; ++i) {
        std::cout
        << "camera " << i << " - CV_CAP_PROP_POS_MSEC = " << camera[i].get(CV_CAP_PROP_POS_MSEC) << std::endl
        << "camera " << i << " - CV_CAP_PROP_POS_FRAMES = " << camera[i].get(CV_CAP_PROP_POS_FRAMES) << std::endl
        << "camera " << i << " - CV_CAP_PROP_POS_AVI_RATIO = " << camera[i].get(CV_CAP_PROP_POS_AVI_RATIO) << std::endl
        << "camera " << i << " - CV_CAP_PROP_FRAME_WIDTH = " << camera[i].get(CV_CAP_PROP_FRAME_WIDTH) << std::endl
        << "camera " << i << " - CV_CAP_PROP_FRAME_HEIGHT = " << camera[i].get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl
        << "camera " << i << " - CV_CAP_PROP_FPS = " << camera[i].get(CV_CAP_PROP_FPS) << std::endl
        << "camera " << i << " - CV_CAP_PROP_FOURCC = " << camera[i].get(CV_CAP_PROP_FOURCC) << std::endl
        << "camera " << i << " - CV_CAP_PROP_FRAME_COUNT = " << camera[i].get(CV_CAP_PROP_FRAME_COUNT) << std::endl
        << "camera " << i << " - CV_CAP_PROP_FORMAT = " << camera[i].get(CV_CAP_PROP_FORMAT) << std::endl
        << "camera " << i << " - CV_CAP_PROP_MODE = " << camera[i].get(CV_CAP_PROP_MODE) << std::endl
        << "camera " << i << " - CV_CAP_PROP_BRIGHTNESS = " << camera[i].get(CV_CAP_PROP_BRIGHTNESS) << std::endl
        << "camera " << i << " - CV_CAP_PROP_CONTRAST = " << camera[i].get(CV_CAP_PROP_CONTRAST) << std::endl
        << "camera " << i << " - CV_CAP_PROP_SATURATION = " << camera[i].get(CV_CAP_PROP_SATURATION) << std::endl
        << "camera " << i << " - CV_CAP_PROP_HUE = " << camera[i].get(CV_CAP_PROP_HUE) << std::endl
        << "camera " << i << " - CV_CAP_PROP_GAIN = " << camera[i].get(CV_CAP_PROP_GAIN) << std::endl
        << "camera " << i << " - CV_CAP_PROP_EXPOSURE = " << camera[i].get(CV_CAP_PROP_EXPOSURE) << std::endl
        << "camera " << i << " - CV_CAP_PROP_CONVERT_RGB = " << camera[i].get(CV_CAP_PROP_CONVERT_RGB) << std::endl
        << "camera " << i << " - CV_CAP_PROP_WHITE_BALANCE_BLUE_U = " << camera[i].get(CV_CAP_PROP_WHITE_BALANCE_BLUE_U) << std::endl
        << "camera " << i << " - CV_CAP_PROP_RECTIFICATION = " << camera[i].get(CV_CAP_PROP_RECTIFICATION) << std::endl
        << std::endl;
    }
}

void StereoCamera::FrameWorker(VideoCapture cm, Mat *frame) {
    if (!cm.grab()){
        std::cerr << "StereoCamera: FrameWorker() - Couldn't grab" << std::endl;
    }
    if (!cm.retrieve(*frame)){
        std::cerr << "StereoCamera: FrameWorker() - Couldn't retrieve" << std::endl;
    }
}

void StereoCamera::camWorker(StereoCamera camera) {
    camera.GrabFrames();
}

void StereoCamera::GrabFrames() {
    frame_available = false;
    boost::thread Thread_FrameLEFT(&StereoCamera::FrameWorker, camera[LEFT], &frame[LEFT]);
    boost::thread Thread_FrameRIGHT(&StereoCamera::FrameWorker, camera[RIGHT], &frame[RIGHT]);

    Thread_FrameLEFT.join();
    Thread_FrameRIGHT.join();
    frame_available = true;
}

bool StereoCamera::FrameAvailable() {
    return frame_available;
}

Mat* StereoCamera::GetFrame() {
    return frame;
}

