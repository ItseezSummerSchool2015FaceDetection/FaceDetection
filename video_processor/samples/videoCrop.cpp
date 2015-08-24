#include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"

 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv;

 /** Function Headers */
 void detectAndDisplay( Mat frame );

 /** Global variables */
 String face_cascade_name = "haarcascade_frontalface_default.xml";

 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 string window_name = "Capture - Face detection";
 RNG rng(12345);

 /** @function main */
 int main( int argc, const char** argv )
 {
   VideoCapture cap(0);
   Mat frame;

   //-- 1. Load the cascades
   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
   

   //-- 2. Read the video stream
   if( cap.isOpened() )
   {
       cap>>frame;
       while(frame.empty()){
        cap>>frame;
       }
     while( true )
     {
   
         cap>>frame;
   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { detectAndDisplay( frame ); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 27 ) { break; }
      }
   }
   return 0;
 }

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    static int counter = 0;
    counter++;
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.01, 20, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
      cout<<faces[i].width<<" "<<faces[i].height<<endl;
   // rectangle(frame,faces[i],Scalar(255,255,0),2);
    Rect roi_rect = faces[i];
    roi_rect.y -=roi_rect.height * 0.3;
    roi_rect.height *= 1.4;
    roi_rect.x -= roi_rect.width*0.1;
    roi_rect.width *= 1.2;
    
    roi_rect = roi_rect & Rect(0,0,frame.cols,frame.rows);
    
    Mat finalImage;
    Size OptimalSize(64,64);
    resize(frame(roi_rect), finalImage, OptimalSize, 0, 0, INTER_AREA);
    char name[100];
    sprintf(name, "%d_%d.png", counter,i);
    imshow("",finalImage);
    imwrite(name,finalImage);

    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;

    
  }
  cout<<endl;
  //-- Show what you got
  //imshow( window_name, frame );
 }