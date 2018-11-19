/*
* help: https://docs.opencv.org/3.4.0/de/da9/tutorial_template_matching.html
*
*/


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc,const char** argv)
{
    ///variabless
    Mat Image1;
    Mat Image_template;
    Mat Image_result;


    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_1  im1|  |(required)}"
        "{image_template  im_temp|  |(required)}"
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

    string image_2(parser.get<string>("image_template"));
    if(image_2.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }




    ///inlezen van afbeeldingen

   Image1 = imread(image_1, IMREAD_COLOR);
   Image_template = imread(image_2, IMREAD_COLOR);

    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }


    ///openen van originele afbeeldingen

    //imshow("loopband", Image1);
    //imshow("template", Image_template);


/*
    ///opdracht 1:Gebruik template matching om een object te vinden in een inputbeeld

    ///template matching toepassen

    matchTemplate( Image1.clone(), Image_template, Image_result, TM_CCOEFF_NORMED);

    ///normaliseren van resultaat

    normalize( Image_result, Image_result, 0, 1, NORM_MINMAX, -1, Mat() );


    ///waarde maxima nemen, SQDIFF geeft minimum terug
    Image_result = 1- Image_result;


    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc( Image_result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    matchLoc = minLoc;

    Mat img_display = Image1.clone();

    rectangle( img_display, matchLoc, Point( matchLoc.x + Image_template.cols , matchLoc.y + Image_template.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( Image_result, matchLoc, Point( matchLoc.x + Image_template.cols , matchLoc.y + Image_template.rows ), Scalar::all(0), 2, 8, 0 );
    imshow( "image", img_display );
    imshow( "result", Image_result );
 */
    ///opdracht 2: Pas de template matching aan om lokaal naar maxima te zoeken, zodanig dat je alle matches vind


    ///template matching toepassen

    matchTemplate( Image1, Image_template, Image_result, TM_CCORR);

    ///normaliseren van resultaat

    normalize( Image_result, Image_result, 0, 1, NORM_MINMAX, -1, Mat());
    threshold(Image_result,Image_result,0.6,1,3);

    ///waarde maxima nemen, SQDIFF geeft minimum terug
    Image_result = 1- Image_result;

    Mat img_display = Image1.clone();
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    matchLoc = minLoc;
    minMaxLoc( Image_result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );


    for(int i = 0 ; i < Image1.rows; i++)
    {
        for(int j = 0; j < Image1.cols; j++)
        {

        }
    }
    imshow( "image", img_display );
    imshow( "result", Image_result );


    waitKey(0);

    return 0;
}


void MatchingMethod( int, void* )
{

}

