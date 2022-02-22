#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)
Mat src, src_gray;
Mat dst, detected_edges, grad;
int lowThreshold = 6;
const int max_lowThreshold = 100;
const int _ratio = 3;
//set the kernel size 3
const int kernel_size = 3;
const char* window_name = "Edge Map";
int change =0;
int ksize;
int scale;
int delta;
int ddepth;
char winInput;
double avg=0.0;
long double tt_fps=0;
int _delta=0;
int _count;
struct timespec start, stop, total, time_start, time_delta;
double fstart, fstop, ftime_start, ftime_delta;
void MeasureFPS(){
    clock_gettime(CLOCK_MONOTONIC, &stop); fstop=(double)stop.tv_sec + ((double)stop.tv_nsec/1000000000.0);
    clock_gettime(CLOCK_MONOTONIC, &time_delta); ftime_delta=(int)time_delta.tv_sec;

    _delta = ftime_delta-ftime_start;
    tt_fps += 1/(fstop-fstart);
    if(_delta%60==0){
        avg = tt_fps/(_count);        
    }
    std::string fps= "FPS: " + std::to_string(1/(fstop-fstart));
    std::string fps_avg= "avg of FPS per min: " + std::to_string(avg);
    std::string time_passed = "Time (sec): " + std::to_string(_delta);
    Mat frame;
    if(change==0){
        frame = detected_edges;
    }else if (change==1){
        frame = grad;
    }else {
        frame = src;
    }
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
    imshow(window_name, frame);
}
static void SobelTreshold(int, void*)
{
        //earase the noise on the image
    // GaussianBlur(image,src, Size(3,3),0,0,BORDER_DEFAULT);

    //conver tot grayscale

    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    /*
    We calculate the "derivatives" in x and y directions. 
    For this, we use the function Sobel() as shown below: 
    The function takes the following arguments:
        src: In our example, the input image. Here it is CV_8U
        grad_x / grad_y : The output image.
        ddepth: The depth of the output image. We set it to CV_16S to avoid overflow.
        x_order: The order of the derivative in x direction.
        y_order: The order of the derivative in y direction.
        scale, delta and BORDER_DEFAULT: We use default values.
    Notice that to calculate the gradient in x direction we use: xorder=1 and yorder=0. 
    We do analogously for the y direction.
    */
    Sobel(src_gray, grad_x, ddepth,1,0, ksize,scale, delta, BORDER_DEFAULT);
    Sobel(src_gray, grad_y, ddepth,0,1, ksize,scale, delta, BORDER_DEFAULT);

    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    
    /*
    We try to approximate the gradient by adding both directional gradients 
    (note that this is not an exact calculation at all! but it is good for our purposes).
    */
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    // imshow(window_name, grad);
    MeasureFPS();

}
static void CannyThreshold(int, void*)
{
    // removeing the noise and ap since the kernel size is 3, set the size 3 by 3
    
    //detecing an edege by Canny with the lowThreashold and maxThreshold which is 3 times thant the lower one.
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*_ratio, kernel_size );
    //set the dist image all black so that you can put the deteceted edges on the black background
    dst = Scalar::all(0);
    // src.copyTo( dst, detected_edges);
    // imshow( window_name, detected_edges );
    MeasureFPS();
}
int main( int argc, char** argv )
{
//   CommandLineParser parser( argc, argv, "{@input | ../data/fruits.jpg | input image}" );
//   src = imread( parser.get<String>( "@input" ), IMREAD_COLOR ); // Load an image

    cv::CommandLineParser parser(argc, argv,
                               "{@input   |../data/lena.jpg|input image}"
                               "{ksize   k|1|ksize (hit 'K' to increase its value)}"
                               "{scale   s|1|scale (hit 'S' to increase its value)}"
                               "{delta   d|0|delta (hit 'D' to increase its value)}"
                               "{help    h|false|show help message}");
    cout << "The sample uses Sobel or Scharr OpenCV functions for edge detection\n\n";
    parser.printMessage();
    cout << "\nPress 'ESC' to exit program.\nPress 'R' to reset values ( ksize will be -1 equal to Scharr function )";
    
    ksize = parser.get<int>("ksize");
    scale = parser.get<int>("scale");
    delta = parser.get<int>("delta");
    ddepth =CV_16S;

    VideoCapture cam0(0);
   namedWindow("video_display");

   if (!cam0.isOpened())
   {
       exit(SYSTEM_ERROR);
   }

   cam0.set(CAP_PROP_FRAME_WIDTH, 640);
   cam0.set(CAP_PROP_FRAME_HEIGHT, 480);
  clock_gettime(CLOCK_MONOTONIC, &time_start); ftime_start=(int)time_start.tv_sec;

  while(1){
    _count++;
    namedWindow( window_name, WINDOW_AUTOSIZE );
    createTrackbar( "Transform", window_name, &change, 2);
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold);
    //start
    clock_gettime(CLOCK_MONOTONIC, &start); fstart=(double)start.tv_sec + ((double)start.tv_nsec/1000000000.0);
    cam0.read(src);
    dst.create( src.size(), src.type() );
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    blur(src_gray, detected_edges, Size(3,3) );
    
    /* you can set the value of Threshold for the canny function. 
        It can be used for ajusting how much we want to detect the edge
    */


    //call canny Threashold
            createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold);
    if (change==0){
        CannyThreshold(0, 0);
    }else if (change==1){
        SobelTreshold(0,0);
    }else{
        MeasureFPS();
    }   
    
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
  destroyAllWindows();
  return 0;
}
