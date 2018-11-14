#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void segmenteer_bgr(Mat image);
Mat segmenteer_hsv(Mat image);
void component_analyse(Mat image);

static void on_trackbar( int, void* )
{

}




int hue_min_slider = 160;
int hue_max_slider = 180;

int saturation_min_slider = 150;
int saturation_max_slider = 250;

int value_min_slider = 150;
int value_max_slider = 250;

int main(int argc,const char** argv)
{
    ///variabless
    string ImageName1;
    Mat Image1;
    Mat Image2;
    Mat Image3;
    char stop;


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

    //segmenteer_hsv(Image1);

    ///Opdracht 3: Gebruik connected component analyse om enkel het stopbord over te houden (zie sessie_1)

//component_analyse(Image1);

    ///Opdracht 4: Gebruik een trackbar om de optimale threshold waardes te bepalen
    //https://docs.opencv.org/3.4.0/da/d6a/tutorial_trackbar.html

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

        component_analyse(Image1);
        imshow( "thresholds",segmenteer_hsv(Image1));
        stop = (char) waitKey(30);
        if (stop == 'q')
        {
            break;
        }
    }

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

    //vector maken om kleuren in samen te voegen

    vector<Mat> combination;
    combination.push_back(Seg_Blue);
    combination.push_back(Seg_Green);
    combination.push_back(Seg_Red);
    merge(combination, finaal);
    imshow("segmenteer en comibineer bgr", finaal);

    return;
}

Mat segmenteer_hsv(Mat image)
{


    //afbeelding omzetten naar HSV
    Mat image_hsv;
    cvtColor(image, image_hsv, CV_BGR2HSV);
    imshow("image in HSV domein", image_hsv);

/*    // H, S en V splitsen
    Mat gesplitst[3];
    split(image,gesplitst);
    Mat V = gesplitst[2];
    Mat S = gesplitst[1];
    Mat H = gesplitst[0];
*/

    // threshold tussen welke laten zien en welken niet

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



    return finaal;
}


void component_analyse(Mat image)
{



    //afbeelding omzetten naar HSV
    Mat image_hsv;
    cvtColor(image, image_hsv, CV_BGR2HSV);
    imshow("image in HSV domein", image_hsv);

/*    // H, S en V splitsen
    Mat gesplitst[3];
    split(image,gesplitst);
    Mat V = gesplitst[2];
    Mat S = gesplitst[1];
    Mat H = gesplitst[0];
*/

    // threshold tussen welke laten zien en welken niet

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
