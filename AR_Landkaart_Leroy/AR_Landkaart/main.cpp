#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



int main(int argc,const char** argv)
{
    string ImageName1 = "test.png";
    Mat Image1;
    Mat Image1_gray;
    Mat detected_edges;
    int lowThreshold = 100;
    int ratio = 3;
    int kernel_size = 3;

    HOGDescriptor d1;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_gray  ig|  |(required)}"
        "{image_color  ic|  |(required)}"
    );


   Image1 = imread(ImageName1, IMREAD_COLOR);



    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    imshow( "Original image", Image1 );


    //We need the gray color

    cvtColor(Image1, Image1_gray, CV_BGR2GRAY);



    waitKey(0);
    return 0;
}



