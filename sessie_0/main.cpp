#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void tekenvierkant(Mat img);
void tekenlijn(Mat img);

int main(int argc,const char** argv)
{
    ///variables
    string ImageName1;
    string ImageName2;
    Mat Image1;
    Mat Image2;


    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_gray  ig|  |(required)}"
        "{image_color  ic|  |(required)}"
    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string image_gray(parser.get<string>("image_gray"));
    if(image_gray.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }
    cout << image_gray << endl;

    string image_color(parser.get<string>("image_color"));
    if(image_color.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }

    cout << image_gray << endl;
    ///inlezen van afbeelding1

   Image1 = imread(image_gray, IMREAD_COLOR);


   Image2 = imread(image_color, IMREAD_COLOR);

    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    if( Image2.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image2" << std::endl ;
        return -1;
    }

    ///images apart origineel visualiseren ------------------------------------------------------------------------------------------------------------------------------

    /// window van image 1

    namedWindow( "Display original image 1", WINDOW_AUTOSIZE ); /// Create a window for display.
    imshow( "Display original image 1", Image1 );                /// Show our image inside it.

    ///window van image 2
    imshow( "Display original image 2", Image2 );                /// Show our image inside it.




    ///De kleurafbeelding opsplitst in kleurkanalen en die apart visualiseert--------------------------------------------------------------------------------------------

    Mat gesplitst[3];
    split(Image2,gesplitst);
    ///splitst op in bgr
    namedWindow("blauw kanaal", WINDOW_AUTOSIZE);
    imshow("blauw kanaal", gesplitst[0]);

    namedWindow("groen kanaal", WINDOW_AUTOSIZE);
    imshow("groen kanaal", gesplitst[1]);


    namedWindow("rood kanaal", WINDOW_AUTOSIZE);
    imshow("rood kanaal", gesplitst[2]);

    ///De kleurenafbeelding omzet in een grijswaarden beeld.-------------------------------------------------------------------------------------------------------------

    Mat grijs;

    ///COLOR_BGR2GRAY => omzetten naar grijswaarden
    cvtColor( Image2, grijs, COLOR_BGR2GRAY );

    namedWindow("grijswaarden", WINDOW_AUTOSIZE);
    imshow("grijswaarden", grijs);


    ///Over de pixels van je zelfgemaakte grijswaarden afbeelding loopt en deze waardes print naar de command line.------------------------------------------------------

    ///cout << grijs << endl;
    for(int i =0;i < grijs.rows; i++)
    {
        for(int j = 0; j< grijs.cols; j++)
        {
           cout <<  (int)grijs.at<uchar>(i,j);
        }
        cout << endl;
    }

    ///Een canvas maakt en daar wat figuren op tekent (rechthoek, cirkel, ...)--------------------------------------------------------------------------------------------
    ///https://docs.opencv.org/3.4.0/d3/d96/tutorial_basic_geometric_drawing.html
    Mat canvas = Mat::zeros(400,400,CV_8UC3);       ///aanmaken van canvas
    tekenvierkant(canvas);
    tekenlijn(canvas);

    imshow("canvas",canvas);


    waitKey(0);                                     /// Wait for a keystroke in the window

    return 0;
}

/// functies voor het tekenen op het canvas
void tekenvierkant(Mat img)
{
    rectangle(img,Point(0,0),Point(50,50),Scalar(0,255,255),2,0);
}

void tekenlijn(Mat img)
{
    line(img, Point(200,200),Point(400,400),Scalar(12,150,75),8,0);
}
