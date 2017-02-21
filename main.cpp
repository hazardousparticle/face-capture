//OpenCV test detect face and capture photo if face detected

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <ctime>

#define ESCAPE_KEY 27

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);
string timeStr(void);

/** Global variables */
String face_cascade_name = "C:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
string save_folder = "C:/face";
string extension = ".jpg";
RNG rng(12345);

/** @function main */
int main(int argc, const char** argv)
{
	cout << "Photos taken when face detected.\n";
	cout << "Escape and enter to quit" << endl;

	CvCapture* capture;
	Mat frame;

	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading\n"); return -1; };

	//-- 2. Read the video stream
	capture = cvCaptureFromCAM (0);

	if (!capture ){ 
		printf("Could not connect to camera\n" ); return 0; 
	} 

	char key = 0;

	cvNamedWindow( window_name.c_str(), CV_WINDOW_AUTOSIZE ); //Create output window 


	while(key!=ESCAPE_KEY)
	{
		frame = cvQueryFrame(capture);
		if (!frame.empty())
		{
			//imshow(window_name, frame);
			detectAndDisplay(frame);

		}
		//Get the last key that's been pressed for input 
		key = cvWaitKey( 1 ); 
	}

	cvReleaseCapture( &capture ); 
	cvDestroyWindow( window_name.c_str() ); 

	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		//Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		rectangle(frame, Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Point(faces[i].x, faces[i].y), Scalar(0,255,0), 1,8);
		//ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(0, 255, 0), 4, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
	}
	
	if (faces.size() >= 1)
	{
		//take a screen shot if at least 1 face.
		imwrite(save_folder + timeStr() + extension , frame );
		printf("Face detected.\n");
	}
	imshow(window_name, frame);
}

string timeStr(void)
{
	//get now time and fill in a string
	time_t clck;
	time(&clck);

	unsigned long c = clck;

	ostringstream convert;

	convert << c;

	string s = convert.str();
	return s;
}
