#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

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

    if( argc <= 2)
    {
        cout << "te weinig argumenten meegegeven" << endl;
        return -1;
    }


    ///Collect data from arguments

    ImageName1 = argv[1];
    ImageName2 = argv[2];

    cout << ImageName1 << endl;
    cout << ImageName2 << endl;

    ///inlezen van afbeelding1

   Image1 = imread("/home/paul/Desktop/school/1819/2018_labo_beeldinterpretatie/introductie/test.png", IMREAD_COLOR);

  // cout << Image1 << endl;

   Image2 = imread("/home/paul/Desktop/school/1819/2018_labo_beeldinterpretatie/introductie/testColor.png", IMREAD_COLOR);

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



    waitKey(0);                                     /// Wait for a keystroke in the window

    return 0;
}
