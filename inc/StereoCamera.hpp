#ifndef _R_STEREO_CAM_HPP_
#define _R_STEREO_CAM_HPP_

#include    <opencv2/opencv.hpp>
#include    <opencv2/stitching/stitcher.hpp>
#include    <opencv2/highgui/highgui.hpp>

#include    <boost/date_time.hpp>
#include    <boost/filesystem.hpp>
#include    <boost/thread.hpp>

#include	"Logger.hpp"

#define LEFT 0
#define RIGHT 1

//! Handle the two cameras
/*!
Class to capture frame from the two camera
*/

class StereoCamera
{
private:
    cv::VideoCapture camera[2];
    cv::Mat frame[2];
    bool camera_open;
    bool mono_cam;
    bool frame_available;

public:
    StereoCamera();
    ~StereoCamera();
    void OpenCamera();
    void CloseCamera();

    //! Output every property of the camera
    void TestCamera();

    //! Tell if new frame in buffer
    bool FrameAvailable();

    //! Multi thread the grabbing of the two frame
    void GrabFrames();

    //! Wrapper of the VideoCapture::set of OpenCV
    /*!
        @param param Property identifier. It can be one of the following:
        - CV_CAP_PROP_POS_MSEC Current position of the video file in milliseconds.
        - CV_CAP_PROP_POS_FRAMES 0-based index of the frame to be decoded/captured next.
        - CV_CAP_PROP_POS_AVI_RATIO Relative position of the video file: 0 - start of the film, 1 - end of the film.
        - CV_CAP_PROP_FRAME_WIDTH Width of the frames in the video stream.
        - CV_CAP_PROP_FRAME_HEIGHT Height of the frames in the video stream.
        - CV_CAP_PROP_FPS Frame rate.
        - CV_CAP_PROP_FOURCC 4-character code of codec.
        - CV_CAP_PROP_FRAME_COUNT Number of frames in the video file.
        - CV_CAP_PROP_FORMAT Format of the Mat objects returned by retrieve() .
        - CV_CAP_PROP_MODE Backend-specific value indicating the current capture mode.
        - CV_CAP_PROP_BRIGHTNESS Brightness of the image (only for cameras).
        - CV_CAP_PROP_CONTRAST Contrast of the image (only for cameras).
        - CV_CAP_PROP_SATURATION Saturation of the image (only for cameras).
        - CV_CAP_PROP_HUE Hue of the image (only for cameras).
        - CV_CAP_PROP_GAIN Gain of the image (only for cameras).
        - CV_CAP_PROP_EXPOSURE Exposure (only for cameras).
        - CV_CAP_PROP_CONVERT_RGB Boolean flags indicating whether images should be converted to RGB.
        - CV_CAP_PROP_WHITE_BALANCE Currently unsupported
        - CV_CAP_PROP_RECTIFICATION Rectification flag for stereo cameras (note: only supported by DC1394 v 2.x backend currently)
        @param value Value of the property
        @param camera To set if we want to change the property of one cam.
    */
    void CameraSet(int param, double value, int camera_id = -1);

    //! Wrapper of the VideoCapture::get of OpenCV
    /*!
        @param param Property identifier. It can be one of the following:
        - CV_CAP_PROP_POS_MSEC Current position of the video file in milliseconds.
        - CV_CAP_PROP_POS_FRAMES 0-based index of the frame to be decoded/captured next.
        - CV_CAP_PROP_POS_AVI_RATIO Relative position of the video file: 0 - start of the film, 1 - end of the film.
        - CV_CAP_PROP_FRAME_WIDTH Width of the frames in the video stream.
        - CV_CAP_PROP_FRAME_HEIGHT Height of the frames in the video stream.
        - CV_CAP_PROP_FPS Frame rate.
        - CV_CAP_PROP_FOURCC 4-character code of codec.
        - CV_CAP_PROP_FRAME_COUNT Number of frames in the video file.
        - CV_CAP_PROP_FORMAT Format of the Mat objects returned by retrieve() .
        - CV_CAP_PROP_MODE Backend-specific value indicating the current capture mode.
        - CV_CAP_PROP_BRIGHTNESS Brightness of the image (only for cameras).
        - CV_CAP_PROP_CONTRAST Contrast of the image (only for cameras).
        - CV_CAP_PROP_SATURATION Saturation of the image (only for cameras).
        - CV_CAP_PROP_HUE Hue of the image (only for cameras).
        - CV_CAP_PROP_GAIN Gain of the image (only for cameras).
        - CV_CAP_PROP_EXPOSURE Exposure (only for cameras).
        - CV_CAP_PROP_CONVERT_RGB Boolean flags indicating whether images should be converted to RGB.
        - CV_CAP_PROP_WHITE_BALANCE Currently unsupported
        - CV_CAP_PROP_RECTIFICATION Rectification flag for stereo cameras (note: only supported by DC1394 v 2.x backend currently)
        @param id of the camera
    */
    double CameraGet(int param, int camera = -1);


    //! To be used in a thread
    static void FrameWorker(cv::VideoCapture cm, cv::Mat *frame);

    static void camWorker(StereoCamera camera);

    cv::Mat* GetFrame();

    /**
     * @brief getCameras
     * @param c : ID of the camera (LEFT or RIGHT)
     * @return
     */
    cv::VideoCapture getCamera(int c);

    bool isOpen();
    bool isMono();
};

#endif //_R_STEREO_CAM_HPP_
