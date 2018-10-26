#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc,const char** argv)
{
    ///variables
    string ImageName1;
    Mat Image1;
    Mat Image2;


    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_color  ic|  |(required)}"
        "{image_modal  im|  |(required)}"
    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string image_color(parser.get<string>("image_color"));
    if(image_color.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }

    string image_modal(parser.get<string>("image_modal"));
    if(image_modal.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }



    ///inlezen van afbeelding1

   Image1 = imread(image_color, IMREAD_COLOR);
   Image2 = imread(image_modal, IMREAD_GRAYSCALE);





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

  /*  namedWindow( "Display original image 1", WINDOW_AUTOSIZE ); /// Create a window for display.
    imshow( "Display original image 1", Image1 );                /// Show our image inside it.

   */ namedWindow( "Display original image 2", WINDOW_AUTOSIZE ); /// Create a window for display.
    imshow( "Display original image 2", Image2 );                /// Show our image inside it.


    /// segmenting skin color


    ///split the image in red,green and blue
    Mat gesplitst[3];
    split(Image1,gesplitst);
    Mat RED = gesplitst[2];
    Mat GREEN = gesplitst[1];
    Mat BLUE = gesplitst[0];

    ///zeros to fill mask with
    Mat mask1 = Mat::zeros(Image1.rows, Image1.cols, CV_8UC1);
    Mat mask2 = mask1.clone();



    /// 2 for loops method to run over every pixel

    for(int i =0;i < Image1.rows; i++)
    {
        for(int j = 0; j< Image1.cols; j++)
        {
            if((RED.at<uchar>(i,j)>95) && (GREEN.at<uchar>(i,j)>40)&& (BLUE.at<uchar>(i,j)>20)&& ((max(RED.at<uchar>(i,j),max(GREEN.at<uchar>(i,j),BLUE.at<uchar>(i,j))) - min(RED.at<uchar>(i,j),min(GREEN.at<uchar>(i,j),BLUE.at<uchar>(i,j))))>15) &&
            (abs(RED.at<uchar>(i,j)-GREEN.at<uchar>(i,j))>15) && (RED.at<uchar>(i,j)>GREEN.at<uchar>(i,j)) && (RED.at<uchar>(i,j)>BLUE.at<uchar>(i,j)))
             {
                mask1.at<uchar>(i,j) = 255;
             }
        }
    }

    //namedWindow( "Display segmented", WINDOW_AUTOSIZE ); /// Create a window for display.
    //imshow( "Display segmented", mask1 );                /// Show our image inside it.

    /// matrix methode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    ///Segmenteer deze kanalen en combineer tot een 3 channel beeld

    ///Mat finaal(Image1.rows, Image1.cols, CV_8UC3);









    ///kasticket

    Mat maskAA;
    threshold(Image2,maskAA,0,255,THRESH_OTSU | THRESH_BINARY);
    imshow("maskAA",maskAA);


    /// Histogram equalization

    Mat equalized;
    Mat maskA;
    equalizeHist(Image2, equalized);
    imshow("equalized", equalized);
    threshold(Image2,maskA,0,255,THRESH_OTSU | THRESH_BINARY);
    imshow("maskA",maskA);

    /// CLAHE
    Mat result_clahe;
    Ptr<>

    waitKey(0);                                     /// Wait for a keystroke in the window

    return 0;
}
