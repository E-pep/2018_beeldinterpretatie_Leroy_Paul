#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


///--------------------------------------Variabelen-------------------------------------
Mat testafbeelding;
Mat testafbeelding_gray;
Mat dst, detected_edges;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
RNG rng(12345);

int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 5;
const char* window_name = "Edge Map";

int main(int argc,const char** argv)
{


    ///--------------------------------------Input & Argumenten-----------------------------

    CommandLineParser parser(argc,argv,
    "{help h|  |show this message}"
    "{image  im|  |(required)}"
    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }

    string image_string(parser.get<string>("image"));
    if(image_string.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }


    testafbeelding = imread(image_string, IMREAD_COLOR);

    if( testafbeelding.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    cvtColor( testafbeelding, testafbeelding_gray, COLOR_BGR2GRAY );

    Mat canny_output;
    Canny( testafbeelding_gray, dst, 100, 100*2 );


    findContours(dst, contours,hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);


  /// Draw contours
  Mat drawing = Mat::zeros( dst.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );

    waitKey(0);
    return 0;
}
