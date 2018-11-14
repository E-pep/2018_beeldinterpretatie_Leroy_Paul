#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void segmenteer_bgr(Mat image);
void segmenteer_hsv(Mat image);

int main(int argc,const char** argv)
{
    ///variabless
    string ImageName1;
    Mat Image1;
    Mat Image2;
    Mat Image3;


    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_1  im1|  |(required)}"
     //   "{image_2  im2|  |(required)}"
     //   "{image_3  im3|  |(required)}"
     //   "{image_4  im4|  |(required)}"
    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string image_1(parser.get<string>("image_1"));
    if(image_1.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }




    ///inlezen van afbeeldingen

   Image1 = imread(image_1, IMREAD_COLOR);






    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }


    ///openen van originele afbeeldingen

    imshow("verkeersbord 1", Image1);



    ///opdracht 1:  Segmenteer de verkeersborden in de BGR kleurenruimte

    segmenteer_bgr(Image1);

    ///opdracht 2: Segmenteer de verkeersborden in de HSV kleurenruimte

    waitKey(0);
    return 0;
}

void segmenteer_bgr(Mat image)
{
    //split the image in red,green and blue
    Mat gesplitst[3];
    split(image,gesplitst);
    Mat RED = gesplitst[2];
    Mat GREEN = gesplitst[1];
    Mat BLUE = gesplitst[0];

    Mat mask = Mat::zeros(image.rows, image.cols, CV_8UC1);
    mask = ((RED>120) & (GREEN<100) & (BLUE<100));

    //Segmenteer deze kanalen en combineer tot een 3 channel beeld

    Mat finaal(image.rows, image.cols, CV_8UC3);

    Mat Seg_Red = RED & mask;
    Mat Seg_Green = GREEN & mask;
    Mat Seg_Blue = BLUE & mask;


    vector<Mat> combination;
    combination.push_back(Seg_Blue);
    combination.push_back(Seg_Green);
    combination.push_back(Seg_Red);
    merge(combination, finaal);
    imshow("segmenteer en comibineer", finaal);

    return;
}

void segmenteer_hsv(Mat image)
{


}
