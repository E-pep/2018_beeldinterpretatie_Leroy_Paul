#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,const char** argv)
{
    string ImageName1 = "test.png";
    string ImageName2 = "test2.png";
    Mat Image1;
    Mat Image2;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_gray  ig|  |(required)}"
        "{image_color  ic|  |(required)}"
    );


   Image1 = imread(ImageName1, IMREAD_COLOR);
   Image2 = imread(ImageName2, IMREAD_COLOR);


    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    imshow( "Original image", Image1 );
    waitKey(0);

    return 0;
}
