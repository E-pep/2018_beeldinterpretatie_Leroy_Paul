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
    Mat Image3;


    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_color  ic|  |(required)}"
        "{image_modal  im|  |(required)}"
        "{image_adapted  ima|  |(required)}"
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

    string image_adapted(parser.get<string>("image_adapted"));
    if(image_modal.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }



    ///inlezen van afbeelding1

   Image1 = imread(image_color, IMREAD_COLOR);
   Image2 = imread(image_modal, IMREAD_GRAYSCALE);
   Image3 = imread(image_adapted, IMREAD_COLOR);





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

    namedWindow( "Display original image 2", WINDOW_AUTOSIZE ); /// Create a window for display.
    imshow( "Display original image 2", Image2 );                /// Show our image inside it.
    */

    /// segmenting skin color


    ///split the image in red,green and blue
    Mat gesplitst[3];
    split(Image1,gesplitst);
    Mat RED = gesplitst[2];
    Mat GREEN = gesplitst[1];
    Mat BLUE = gesplitst[0];

    ///zeros to fill mask with zeros
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

    namedWindow( "Display segmented loop", WINDOW_AUTOSIZE ); /// Create a window for display.
    imshow( "Display segmented loop", mask1 );                /// Show our image inside it.



    /// matrix methode

    mask2 = (RED>95) & (GREEN>40) & (BLUE>20) & ((max(RED,max(GREEN,BLUE)) - min(RED,min(GREEN,BLUE)))>15) & (abs(RED-GREEN)>15) & (RED>GREEN) & (RED>BLUE);

    mask2 = mask2 * 255; ///masker bestaat nu uit 1 of 0. maal 255 doen om duidelijk verschil te zien


    namedWindow( "Display segmented matrix", WINDOW_AUTOSIZE ); /// Create a window for display.
    imshow( "Display segmented matrix", mask2 );                /// Show our image inside it.


    ///Segmenteer deze kanalen en combineer tot een 3 channel beeld

    Mat finaal(Image1.rows, Image1.cols, CV_8UC3);

    Mat Seg_Red = RED & mask2;
    Mat Seg_Green = GREEN & mask2;
    Mat Seg_Blue = BLUE & mask2;


    vector<Mat> combination;
    combination.push_back(Seg_Red);
    combination.push_back(Seg_Green);
    combination.push_back(Seg_Blue);
    merge(combination, finaal);
    imshow("segmenteer en comibineer", finaal);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    ///kasticket

    Mat maskAA;
    threshold(Image2,maskAA,0,255,THRESH_OTSU | THRESH_BINARY);
    imshow("OTSU",maskAA);
    // probleem: groot deel van ons ticket is niet meer zichtbaar

    /// Histogram equalization

    Mat maskA;
    equalizeHist(Image2.clone(), maskA);
    imshow("equalized", maskA);
    threshold(maskA,maskAA,0,255,THRESH_OTSU | THRESH_BINARY);
    imshow("uqualized en otsu",maskAA);

    /// CLAHE
    Mat result_clahe;
     Ptr<CLAHE> clahe_pointer = createCLAHE();
    clahe_pointer->setTilesGridSize(Size(15,15));
    clahe_pointer->setClipLimit(1);
    clahe_pointer->apply(Image2.clone(), result_clahe);
    threshold(result_clahe, maskAA, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("CLAHE ", maskAA);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///1.2


    ///split the image in red,green and blue
    Mat gesplitst2[3];
    split(Image3,gesplitst);
    Mat RED2 = gesplitst[2];
    Mat GREEN2 = gesplitst[1];
    Mat BLUE2 = gesplitst[0];

    ///zeros to fill mask with zeros
    Mat mask3 = Mat::zeros(Image3.rows, Image3.cols, CV_8UC1);
    mask3 = (RED2>95) & (GREEN2>40) & (BLUE2>20) & ((max(RED2,max(GREEN2,BLUE2)) - min(RED2,min(GREEN2,BLUE2)))>15) & (abs(RED2-GREEN2)>15) & (RED2>GREEN2) & (RED2>BLUE2);

    ///Opening

    erode(mask3, mask3, Mat(), Point(-1,-1), 2);
    dilate(mask3, mask3, Mat(), Point(-1,-1), 2);
    imshow("opening", mask3);

    ///Closing
    dilate(mask3, mask3, Mat(), Point(-1,-1), 5);
    erode(mask3, mask3, Mat(), Point(-1,-1), 5);
    imshow("closing", mask3);



    ///contours en hulls

    vector< vector<Point>> contours;
    findContours(mask3.clone(), contours,  RETR_EXTERNAL, CHAIN_APPROX_NONE);
    vector< vector<Point>> hulls;

        for(size_t i=0; i<contours.size(); i++)
        {
        vector<Point> hull;
        convexHull(contours[i], hull);
        hulls.push_back(hull);
        }

    drawContours(mask3, hulls, -1, 255, -1);
    imshow("contours", mask3);


        ///Segmenteer deze kanalen en combineer tot een 3 channel beeld

    Mat finaal2(Image3.rows, Image3.cols, CV_8UC3);

    Mat Seg_Red2 = RED2 & mask3;
    Mat Seg_Green2 = GREEN2 & mask3;
    Mat Seg_Blue2 = BLUE2 & mask3;


    vector<Mat> combination2;
    combination2.push_back(Seg_Red2);
    combination2.push_back(Seg_Green2);
    combination2.push_back(Seg_Blue2);
    merge(combination2, finaal2);
    imshow("segmenteer en comibineer 2", finaal2);





    waitKey(0);
    return 0;
}
