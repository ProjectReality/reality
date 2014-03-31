//#include	"OgreRenderer.hpp"
//#include	"StereoCamera.hpp"
//#include	"Oculus.hpp"
//#include	"ARManager.hpp"
//#include <opencv2/imgproc/imgproc.hpp>
//
//
//#ifdef _WIN32
//#include <windows.h>
//#endif
//#include <stdio.h>
//#include <stdlib.h>
//#include <AR/gsub.h>
//#include <AR/video.h>
//#include <AR/param.h>
//#include <AR/ar.h>
//
//////
////// Camera configuration.
//////
////#ifdef _WIN32
////char			*vconf = "Data\\WDM_camera_flipV.xml";
////#else
////char			*vconf = "";
////#endif
////
////int             xsize, ysize;
////int             thresh = 100;
////int             count = 0;
////
////char           *cparam_name = "Data/camera_para.dat";
////ARParam         cparam;
////
////char           *patt_name = "Data/patt.hiro";
////int             patt_id;
////double          patt_width = 80.0;
////double          patt_center[2] = { 0.0, 0.0 };
////double          patt_trans[3][4];
////
////static void   init(void);
////static void   cleanup(void);
////static void   keyEvent(unsigned char key, int x, int y);
////static void   mainLoop(void);
////static void   draw(void);
////ARManager		ar;
////
////int main(int argc, char **argv)
////{
////	init();
////	arVideoCapStart();
////	ar.init();
////	ar.start();
////	argMainLoop(NULL, keyEvent, mainLoop);
////	ar.stop();
////	return (0);
////}
////
////static void   keyEvent(unsigned char key, int x, int y)
////{
////	/* quit if the ESC key is pressed */
////	if (key == 0x1b) {
////		printf("*** %f (frame/sec)\n", (double)count / arUtilTimer());
////		cleanup();
////		exit(0);
////	}
////}
////
/////* main loop */
////static void mainLoop(void)
////{
////	ARMarkerInfo    *marker_info;
////	int             marker_num;
////	ARUint8         *dataPtr;
////
////	/* grab a vide frame */
////	if ((dataPtr = (ARUint8 *)arVideoGetImage()) == NULL) {
////		arUtilSleep(2);
////		return;
////	}
////	if (count == 0) arUtilTimerReset();
////	count++;
////	argDrawMode2D();
////	argDispImage(dataPtr, 0, 0);
////
////	/* detect the markers in the video frame */
////	ar.setFrame(dataPtr);
////	/*if (arDetectMarker(ar.getFrame(), ar.getTresh(), &marker_info, &marker_num) >= 0)
////	{
////		std::cout << "=====> Marker NUM : " << marker_num << std::endl;
////	}*/
////	arVideoCapNext();
////
////	argSwapBuffers();
////}
////
////static void init(void)
////{
////	ARParam  wparam;
////
////	/* open the video path */
////	if (arVideoOpen(vconf) < 0) exit(0);
////	/* find the size of the window */
////	if (arVideoInqSize(&xsize, &ysize) < 0) exit(0);
////	printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);
////
////	/* set the initial camera parameters */
////	if (arParamLoad(cparam_name, 1, &wparam) < 0) {
////		printf("Camera parameter load error !!\n");
////		exit(0);
////	}
////	arParamChangeSize(&wparam, xsize, ysize, &cparam);
////	arInitCparam(&cparam);
////	printf("*** Camera Parameter ***\n");
////	arParamDisp(&cparam);
////	/*if ((patt_id = arLoadPatt(patt_name)) < 0) {
////		printf("pattern load error !!\n");
////		exit(0);
////	}*/
////
////	/* open the graphics window */
////	argInit(&cparam, 1.0, 0, 0, 0, 0);
////}
////
/////* cleanup function called when program exits */
////static void cleanup(void)
////{
////	arVideoCapStop();
////	arVideoClose();
////	argCleanup();
////}
//
//int		main()
//{
//  IRenderer*	render;
//  StereoCamera	camera;
//  cv::Mat*		frame;
//  double		video_size[2];
//  ARManager		ar;
//
//  char           *cparam_name = "Data/camera_para.dat";
//  ARParam						cparam;
//
//  VirtualOculus *rift = new VirtualOculus();
//  rift = rift->Init();
//
//  /* set the initial camera parameters */
//  if (arParamLoad(cparam_name, 1, &cparam) < 0) {
//	  printf("Camera parameter load error !!\n");
//	  exit(0);
//  }
//  arParamDisp(&cparam);
//  arInitCparam(&cparam);
//  if ((arLoadPatt("Data/patterns/patt.hiro")) < 0) {
//	  std::cout << "Pattern file load error" << std::endl;
//	  return 1;
//  }
//  //ugly test
//  float test = rift->getDistordScale();
//  int xwindow = rift->getResolution()[0];
//  int ywindow = rift->getResolution()[1];
//  //StereoEyeParams* eyes = rift->getEyesParams();
//
//  camera.OpenCamera();
//	
//  video_size[0] = camera.CameraGet(CV_CAP_PROP_FRAME_WIDTH, 0);
//  video_size[1] = camera.CameraGet(CV_CAP_PROP_FRAME_HEIGHT, 0);
//
//  render = new OgreRenderer(video_size, rift);
// 
//
//  // Scene creation
//  render->createEntity("Test", "");
//  
//  // Free grab & get to get rid of the first frame
//  camera.GrabFrames();                                                                                                                                                                      
//
//  // AR init
// // ar.init();
// // ar.start();
//
//  // Render loop
//  while(render->isAlive())
//    {
//	  if (ARManager::verbose)
//	  {
//		  if (ar.isChanged())
//		  {
//			  std::list<AssetInfo> markers = ar.getMarkers();
//			  for (std::list<AssetInfo>::iterator it = markers.begin(); it != markers.end(); it++)
//			  {
//				  std::cout << "Marker : " << it->getPattName() << std::endl;
//			  }
//		  }
//	  }
//	  if (camera.FrameAvailable()) {
//
//		  ARMarkerInfo    *a_markerInfo = NULL;
//		  int				markerNum = -1;
//
//		  frame = camera.GetFrame(); 
//		  //arParamChangeSize(&cameraParam, frame.rows, frame.cols, &cameraParam);
//		  cv::Mat myFrame;// (frame[0].size(), CV_8U);
//		  //cv::cvtColor(frame[0], myFrame, CV_BGR2GRAY);
//		  frame->convertTo(myFrame, CV_8U);
//		  //ar.setFrame(myFrame.data);
//		  ar.setFrame(frame[0].data);
//		  arParamChangeSize(&cparam, frame[0].rows, frame[0].cols, &cparam);
//		  //ar.change(frame[0]);
//		  std::cout << "Rhaaaaaaaaaaaaa" << std::endl;
//		  if (arDetectMarker(frame[0].data, ar.getTresh(), &a_markerInfo, &markerNum) >= 0)
//		  {
//			  std::cout << "||||| Marker NUM : " << markerNum << std::endl;
//		  }
//		  std::cout << "Rhaaaaaaaaaaaaa2" << std::endl;
//		  render->loadCam(frame[0], frame[1]);
//		  boost::thread new_pic(&StereoCamera::camWorker, camera);
//	  }
//      render->rotateEntity("Test", 0, 1, 0);
//      render->render();
//  }
//  //ar.stop();
//  delete render;
//  return 0;
//}
//
//// invest: utiliser le même flux webcam pour les deux viewports en le dćalant

/*
	// Load (default) camera parameters
	ARParam cameraParam;
	if (arParamLoad("Data/camera_para.dat", 1, &cameraParam) < 0) {
		cout << "Camera calibration parameters file load error" << endl;
		return 1;
	}
	//arInitCparam(&cameraParam);

	// Load kanji pattern
	int patternID = -1;
	if ((patternID = arLoadPatt("Data/patterns/patt.kanji")) < 0) {
		cout << "Pattern file load error" << endl;
		return 1;
	}
	*/


#include <AR/ar.h>
#include <AR/video.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

	//using namespace std;

	//int main(int argc, char* argv[])
	//{
	//	arDebug = 1;
	//	ARParam cameraParam;
	//	if (arParamLoad("Data/camera_para.dat", 1, &cameraParam) < 0) {
	//		cout << "Camera calibration parameters file load error" << endl;
	//		return 1;
	//	}
	//	arParamChangeSize(&cameraParam, 640, 480, &cameraParam);
	//	//cpar;
	//	//arInitCparam(&cameraParam);
	//	arParam = cameraParam;
	//	// Load kanji pattern
	//	int patternID = -1;
	//	if ((patternID = arLoadPatt("Data/patterns/patt.kanji")) < 0) {
	//		cout << "Pattern file load error" << endl;
	//		return 1;
	//	}

	//	VideoCapture cap(0); // open the video camera no. 0

	//	if (!cap.isOpened())  // if not success, exit program
	//	{
	//		cout << "Cannot open the video cam" << endl;
	//		return -1;
	//	}

	//	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	//	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	//	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	//	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	//	while (1)
	//	{
	//		Mat frame;
	//		ARMarkerInfo    *a_markerInfo = NULL;
	//		int				markerNum = -1;
	//		bool bSuccess = cap.read(frame); // read a new frame from video

	//		if (!bSuccess) //if not success, break loop
	//		{
	//			cout << "Cannot read a frame from video stream" << endl;
	//			break;
	//		}
	//		if (arDetectMarker(frame.data, 100, &a_markerInfo, &markerNum) >= 0)
	//		{
	//			std::cout << "||||| Marker NUM : " << markerNum << std::endl;
	//		}
	//		imshow("MyVideo", frame); //show the frame in "MyVideo" window

	//		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	//		{
	//			cout << "esc key is pressed by user" << endl;
	//			break;
	//		}
	//	}
	//	return 0;

	//}


////frame.convertTo(frame, CV_16U);
//std::cout << "______________________________________________________________________________________" << std::endl;
//for (int i = 0; i < frame.cols; i++)
//{
//	for (int j = 0; j < frame.rows; j++)
//		std::cout << (unsigned int)(frame.data[i * frame.cols + j]);
//	std::cout << std::endl;
//}
//std::cout << "______________________________________________________________________________________" << std::endl;
//for (int i = 0; i < frame.cols; i++)
//{
//	for (int j = 0; j < frame.rows; j++)
//		std::cout << (unsigned int)(dataPtr[i * frame.cols + j]);
//	std::cout << std::endl;
//}

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>


using namespace cv;
//
// Camera configuration.
//
#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

int             xsize, ysize;
int             thresh = 100;
int             count = 0;

char           *cparam_name = "Data/camera_para.dat";
ARParam         cparam;

char           *patt_name = "Data/patterns/patt.hiro";
int             patt_id;
double          patt_width = 80.0;
double          patt_center[2] = { 0.0, 0.0 };
double          patt_trans[3][4];

static void   init(void);
static void   cleanup(void);
static void   keyEvent(unsigned char key, int x, int y);
static void   mainLoop(void);
static void   draw(void);

VideoCapture cap(0);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	printf("Image size (x,y) =\n");
	init();
	printf("Image size (x,y)\n");
	arVideoCapStart();
	argMainLoop(NULL, keyEvent, mainLoop);
	return (0);
}

static void   keyEvent(unsigned char key, int x, int y)
{
	/* quit if the ESC key is pressed */
	if (key == 0x1b) {
		printf("*** %f (frame/sec)\n", (double)count / arUtilTimer());
		cleanup();
		exit(0);
	}
}
#include <opencv2/imgproc/imgproc.hpp>
/* main loop */
static void mainLoop(void)
{
	ARUint8         *dataPtr;
	ARMarkerInfo    *marker_info;
	int             marker_num;
	int             j, k;

	Mat other_frame;
	cap.read(other_frame);
	Mat	frame;
	//other_frame.convertTo(frame, CV_8SC1, 0, 0);
	//std::cout << frame.channels() << std::endl;
	//frame.reshape(1, 480);
//	IplImage *img = new IplImage(frame);
//	IplImage * img_out = new IplImage;
	//cvCvtColor(img, img, CV_BGR2YCrCb);
	//cvConvertImage(&frame, &frame_in, CV_CVTIMG_SWAP_RB);
	/* grab a vide frame*/
	/*if ((dataPtr = (ARUint8 *)arVideoGetImage()) == NULL) {
		arUtilSleep(2);
		return;
	} 
	Mat frame(640, 480, CV_8SC1, dataPtr);*/
	cv::cvtColor(other_frame, frame, CV_BGR2GRAY);
	frame.convertTo(frame, CV_8U);
	std::cout << "________________________________________________" << std::endl;
	std::cout << "Channel " << frame.channels() << std::endl;
	std::cout << "Size " << frame.size << std::endl;
	std::cout << "isCont " << frame.isContinuous() << std::endl;
	std::cout << "Dim " << frame.dims << std::endl;
	std::cout << "Depth " << frame.depth() << std::endl;
	std::cout << "type " << frame.type() << std::endl;

	//frame.convertTo(frame, CV_16U);
	/*std::cout << "______________________________________________________________________________________" << std::endl;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < frame.rows; j++)
			std::cout << (unsigned int)(frame.data[i * frame.cols + j]) << " - " ;
		std::cout << std::endl;
	}
	std::cout << "______________________________________________________________________________________" << std::endl;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
			std::cout << (unsigned int)(dataPtr[i * frame.cols + j]) << " - ";
		std::cout << std::endl;
	}*/
	if (count == 0) arUtilTimerReset();
	count++;

	argDrawMode2D();
	argDispImage((ARUint8 *)frame.data, 0, 0);

	/* detect the markers in the video frame */
	if (arDetectMarker((ARUint8 *)frame.data, thresh, &marker_info, &marker_num) < 0) {
		cleanup();
		exit(0);
	}

	arVideoCapNext();

	/* check for object visibility */
	k = -1;
	for (j = 0; j < marker_num; j++) {
		if (patt_id == marker_info[j].id) {
			if (k == -1) k = j;
			else if (marker_info[k].cf < marker_info[j].cf) k = j;
		}
	}
	if (k == -1) {
		argSwapBuffers();
		return;
	}

	/* get the transformation between the marker and the real camera */
	arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);

	draw();

	argSwapBuffers();
}

static void init(void)
{
	ARParam  wparam;

	/* open the video path */
	if (arVideoOpen("Data/WDM_camera_flipV.xml") < 0) exit(0);
	/* find the size of the window */
	if (arVideoInqSize(&xsize, &ysize) < 0) exit(0);
	printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

	/* set the initial camera parameters */
	if (arParamLoad(cparam_name, 1, &wparam) < 0) {
		printf("Camera parameter load error !!\n");
		exit(0);
	}
	arParamChangeSize(&wparam, xsize, ysize, &cparam);
	arInitCparam(&cparam);
	printf("*** Camera Parameter ***\n");
	arParamDisp(&cparam);

	if ((patt_id = arLoadPatt(patt_name)) < 0) {
		printf("pattern load error !!\n");
		exit(0);
	}

	/* open the graphics window */
	argInit(&cparam, 1.0, 0, 0, 0, 0);
}

/* cleanup function called when program exits */
static void cleanup(void)
{
	arVideoCapStop();
	arVideoClose();
	argCleanup();
}

static void draw(void)
{
	double    gl_para[16];
	GLfloat   mat_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat   mat_flash[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat   mat_flash_shiny[] = { 50.0 };
	GLfloat   light_position[] = { 100.0, -200.0, 200.0, 0.0 };
	GLfloat   ambi[] = { 0.1, 0.1, 0.1, 0.1 };
	GLfloat   lightZeroColor[] = { 0.9, 0.9, 0.9, 0.1 };

	argDrawMode3D();
	argDraw3dCamera(0, 0);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* load the camera transformation matrix */
	argConvGlpara(patt_trans, gl_para);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(gl_para);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0, 0.0, 25.0);
	glutSolidCube(50.0);
	glDisable(GL_LIGHTING);

	glDisable(GL_DEPTH_TEST);
}
