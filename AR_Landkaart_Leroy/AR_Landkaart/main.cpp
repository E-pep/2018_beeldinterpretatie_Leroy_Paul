#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



///functions

vector<string> getCountrylist(string pathName);
void getCountryContours(string pathName);


vector<Point> component_analyse(Mat image);

vector<Point> get_hulls(Mat image);


static void on_trackbar( int, void* )
{

}



/// Global variables
int hue_min_slider = 142;
int hue_max_slider = 180;

int saturation_min_slider = 95;
int saturation_max_slider = 250;

int value_min_slider = 117;
int value_max_slider = 250;



int main(int argc,const char** argv)
{
    string pathName = "/home/paul/Desktop/school/1819/2018_beeldinterpretatie_Leroy_Paul/AR_Landkaart_Leroy/AR_Landkaart/country_contours";
    string ImageName1 = "vorm.jpg";
    getCountryContours(ImageName1);
    return 0;
    Mat Image1;

    string ImageName2 = "vorm2.jpg";
    Mat Image2;

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
   Image2 = imread(ImageName2, IMREAD_COLOR);


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
    vector<Point> hull1;
    vector<Point> hull2;
    vector<Point> hull3;
    double testshape;
    hull2 = get_hulls(Image1);
    hull3 = get_hulls(Image2);

        while (true)
    {

        cap >> frame;
        hull1 = component_analyse(frame);
        hull2 = get_hulls(Image1);
        if(!hull1.empty())
        {
        testshape = matchShapes(hull1, hull2,1,1);
        printf("matchshapes: %f \r\n", testshape);
        }
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

vector<Point> component_analyse(Mat image)
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
        double area0 = contourArea(contours.at(i));
        if(area0 > 50000)
        {
            hulls.push_back(contours.at(i));
        }
    }
    printf("contour size of frames: %d \r\n", hulls.size());
    drawContours(finaal, hulls, -1, 255, -1);
    imshow("contours", finaal);


    Mat contours2;
    contours2 = Mat::zeros(image.rows, image.cols, CV_8UC3);
    drawContours(contours2, hulls, -1, 255, -1);
    imshow("contours2", contours2);

    ///samenvoegen afbeelding met masker over

    Mat totaal;
    totaal = Mat::zeros(image.rows, image.cols, CV_8UC3);
    image.copyTo(totaal,finaal);
    imshow("totaal", totaal);

    if(!hulls.empty())
    {
        return hulls.at(0);
    }
    else
    {
        return vector<Point>();
    }
}


vector<Point> get_hulls(Mat image)
{
    Mat image_hsv;
    cvtColor(image, image_hsv, CV_BGR2HSV);
    Mat finaal(image.rows, image.cols, CV_8UC3);
        int hue_low= hue_min_slider;
    int saturation_low = saturation_min_slider;
    int value_low = value_min_slider;

    int hue_high = hue_max_slider;
    int saturation_high = saturation_max_slider;
    int value_high = value_max_slider;
    inRange(image_hsv, Scalar(0, 100, 0), Scalar(180, 255, 255), finaal);
    imshow("get_hulls", finaal);

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

    printf("hulls shape: %d \r\n", hulls.size() );
    return hulls.at(0);
}

vector<string> getCountrylist(string pathName)
{
    vector<string> returnNames;
    vector<String> fileNames;
    String fullPathName = pathName + "/*.jpg";
    glob(fullPathName, fileNames, false);

    string tempString;
    for (size_t i=0; i<fileNames.size(); i++)
    {
        tempString = fileNames.at(i);
        tempString.erase(0,pathName.length()+1);
        tempString.erase(4);
        cout << "found file: " << tempString  << endl;
        returnNames.push_back(tempString);
    }
    return returnNames;
}



vector<vector<Point>> getCountryContours(string pathName)
{
    Mat CountryImage;
    vector<vector<Point>> contourList;
    vector<Point> contour;

    String fullPathName = pathName + "/*.jpg";
    vector<String> fileNames;
    glob(fullPathName, fileNames, false);

    size_t picture_count = fileNames.size(); //number of png files in images folder
    for (size_t i=0; i<picture_count; i++)
    {
        cout << "found file: " << fileNames.at(i)  << endl;

    }

}
