#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



///functions

void component_analyse(Mat image);

static void on_trackbar( int, void* )
{

}



/// Global variables
int hue_min_slider = 160;
int hue_max_slider = 180;

int saturation_min_slider = 150;
int saturation_max_slider = 250;

int value_min_slider = 150;
int value_max_slider = 250;



int main(int argc,const char** argv)
{
    string ImageName1 = "test.png";
    Mat Image1;
    char stop;
    Mat frame;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_gray  ig|  |(required)}"
        "{image_color  ic|  |(required)}"
    );


// ------------------------------------------------------------------------ reading in test Image-------------------------------------------------
   Image1 = imread(ImageName1, IMREAD_COLOR);



    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    imshow( "Original image", Image1 );

        //read out webcam
    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout << "Cam could notbe opened" << endl;
        return -1;
    }

   // ------------------------------------------------------------------------ --------------------------------------------------------------------
    component_analyse(Image1);
   /// We are going to base ourself on Session 2:
   /// Search red in our image
    namedWindow("thresholds", WINDOW_AUTOSIZE); // Create Window
    createTrackbar( "HUE minimum", "thresholds", &hue_min_slider, 180, on_trackbar );
    createTrackbar( "HUE maximum", "thresholds", &hue_max_slider, 180, on_trackbar );
    createTrackbar( "Saturation minimum", "thresholds", &saturation_min_slider, 255, on_trackbar );
    createTrackbar( "Saturation maximum", "thresholds", &saturation_max_slider, 255, on_trackbar );
    createTrackbar( "value minimimum", "thresholds", &value_min_slider, 255, on_trackbar );
    createTrackbar( "value maximum", "thresholds", &value_max_slider, 255, on_trackbar );
    imshow( "thresholds",Image1);
        while (true)
    {

        cap >> frame;
        component_analyse(frame);
        imshow( "thresholds",frame);
        stop = (char) waitKey(30);
        if (stop == 'q')
        {
            break;
        }
    }


    waitKey(0);
    return 0;
}

void component_analyse(Mat image)
{
    //afbeelding omzetten naar HSV
    Mat image_hsv;
    cvtColor(image, image_hsv, CV_BGR2HSV);
    imshow("image in HSV domein", image_hsv);


    int hue_low= hue_min_slider;
    int saturation_low = saturation_min_slider;
    int value_low = value_min_slider;

    int hue_high = hue_max_slider;
    int saturation_high = saturation_max_slider;
    int value_high = value_max_slider;
/*
    int hue_low=0;
    int saturation_low = 115;
    int value_low = 145;
    int hue_high = 180;
    int saturation_high = 255;
    int value_high = 255;
*/

    Mat finaal(image.rows, image.cols, CV_8UC3);
    inRange(image_hsv, Scalar(hue_low, saturation_low, value_low), Scalar(hue_high, saturation_high, value_high), finaal);

    imshow("segmenteer threshold HSV", finaal);


///zelfde doen als sessie 1

    //Closing

    dilate(finaal, finaal, Mat(), Point(-1,-1), 5);
    erode(finaal, finaal, Mat(), Point(-1,-1), 5);
    imshow("closing", finaal);


        ///contours en hulls

    vector< vector<Point>> contours;
    findContours(finaal.clone(), contours,  RETR_EXTERNAL, CHAIN_APPROX_NONE);
    vector< vector<Point>> hulls;

    for(size_t i=0; i<contours.size(); i++)
    {
        vector<Point> hull;
        convexHull(contours[i], hull);
        hulls.push_back(hull);
    }

    drawContours(finaal, hulls, -1, 255, -1);
    imshow("contours", finaal);

    ///samenvoegen afbeelding met masker over

    Mat totaal;
    totaal = Mat::zeros(image.rows, image.cols, CV_8UC3);
    image.copyTo(totaal,finaal);
    imshow("totaal", totaal);


    return;
}
