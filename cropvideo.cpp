#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#define rep(i,a,b) for(int i=a;i<b;i++)
using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)
#define IMG_HEIGHT (240)
#define IMG_WIDTH (320)


int main(int argc, char *argv[]){
  cout << "hello" <<endl;
  char winInput;
  //VideoCapture cap("./Open_Source_HD_Video_1080p_MP4.mp4");
  VideoCapture cap("./big_buck_bunny_480p_surround-fix.avi");
  rep(i,0,200){
      Mat image;
      cap.read(image);
      string videostr = "./ppms/frame" + to_string(i+1)+".ppm"; 
      if(i==99)
        imwrite(videostr, image);  
      else if (i==199)
        imwrite(videostr, image);
  }
  
  return 0;
}
