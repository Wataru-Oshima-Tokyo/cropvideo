/*
 *
 *  Example by Sam Siewert 
 *
 *  Created for OpenCV 4.x for Jetson Nano 2g, based upon
 *  https://docs.opencv.org/4.1.1
 *
 *  Tested with JetPack 4.6 which installs OpenCV 4.1.1
 *  (https://developer.nvidia.com/embedded/jetpack)
 *
 *  Based upon earlier simpler-capture examples created
 *  for OpenCV 2.x and 3.x (C and C++ mixed API) which show
 *  how to use OpenCV instead of lower level V4L2 API for the
 *  Linux UVC driver.
 *
 *  Verify your hardware and OS configuration with:
 *  1) lsusb
 *  2) ls -l /dev/video*
 *  3) dmesg | grep UVC
 *
 *  Note that OpenCV 4.x only supports the C++ API
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include<opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)


struct timespec start, stop, total, time_start, time_delta;
double fstart, fstop, ftime_start, ftime_delta;
// int ftotal;
int main()
{
   VideoCapture cam0(0);
   namedWindow("video_display");
   char winInput;

   if (!cam0.isOpened())
   {
       exit(SYSTEM_ERROR);
   }

   cam0.set(CAP_PROP_FRAME_WIDTH, 640);
   cam0.set(CAP_PROP_FRAME_HEIGHT, 480);
   double avg=0.0;
   long double tt_fps=0;
   int delta=0;
   clock_gettime(CLOCK_MONOTONIC, &time_start); ftime_start=(int)time_start.tv_sec;
   int count=0;
   while (1)
   {
      count++;
      Mat frame;
      
    //   tt += (double)total;
      clock_gettime(CLOCK_MONOTONIC, &start); fstart=(double)start.tv_sec + ((double)start.tv_nsec/1000000000.0);
      cam0.read(frame);
      clock_gettime(CLOCK_MONOTONIC, &stop); fstop=(double)stop.tv_sec + ((double)stop.tv_nsec/1000000000.0);
      clock_gettime(CLOCK_MONOTONIC, &time_delta); ftime_delta=(int)time_delta.tv_sec;
      delta = ftime_delta-ftime_start;
      tt_fps += 1/(fstop-fstart);
      if(delta%60==0){
         avg = tt_fps/(count);        
      }
      std::string fps= "FPS: " + std::to_string(1/(fstop-fstart));
      std::string fps_avg= "avg of FPS per min: " + std::to_string(avg);
      std::string time_passed = "Time (sec): " + std::to_string(delta);

      putText(frame, //target image
            time_passed, //text
            Point(10, 30), //top-left position
            FONT_HERSHEY_DUPLEX,
            1.0,
            Scalar(118, 185, 0), //font color
            2);
      putText(frame, //target image
            fps, //text
            Point(10, 60), //top-left position
            FONT_HERSHEY_DUPLEX,
            1.0,
            Scalar(118, 185, 0), //font color
            2);
      putText(frame, //target image
            fps_avg, //text
            Point(10, 90), //top-left position
            FONT_HERSHEY_DUPLEX,
            1.0,
            Scalar(118, 185, 0), //font color
            2);
      imshow("video_display", frame);

      if ((winInput = waitKey(10)) == ESCAPE_KEY)
      //if ((winInput = waitKey(0)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 'n')
      {
	  cout << "input " << winInput << " ignored" << endl;
      }
      
   }

   destroyWindow("video_display"); 
};
