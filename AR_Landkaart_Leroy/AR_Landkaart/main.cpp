#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;



///functions

vector<string> getCountrylist(string pathName);
vector<vector<Point>> getCountryContours(string pathName);


vector<Point> component_analyse(Mat image);

vector<Point> get_hulls(Mat image);

vector<vector<Mat>> get_vacation_pictures(vector<string> countrynames, string pathNameToVacation);


static void on_trackbar( int, void* )
{

}



/// Global variables

// default values for the sliders
int hue_min_slider = 50;
int hue_max_slider = 85;

int saturation_min_slider = 60;
int saturation_max_slider = 255;

int value_min_slider = 60;
int value_max_slider = 255;



int main(int argc,const char** argv)
{
    string pathName = "/home/paul/Desktop/school/1819/2018_beeldinterpretatie_Leroy_Paul/AR_Landkaart_Leroy/AR_Landkaart/country_contours";
    string pathNameToVacation = "/home/paul/Desktop/school/1819/2018_beeldinterpretatie_Leroy_Paul/AR_Landkaart_Leroy/AR_Landkaart/vacation_pictures";
    // vector filled with names of everey country with a contour
    vector<string> CountryList = getCountrylist(pathName);

    // Same order as countryList vector
    vector<vector<Mat>> picturesVector;

    //check all names in the vector
    cout << "countrylist contains:" << endl;
    for(int countList = 0; countList < CountryList.size(); countList++)
    {
        cout << CountryList.at(countList) << endl;
    }

    // vector filled with the contourpoints of the countries => used as template
    vector<vector<Point>> CountryContours = getCountryContours(pathName);


    // get the vacation pictures and sture them in a vector of vectors. Index = country

    picturesVector = get_vacation_pictures(CountryList, pathNameToVacation);


/*    for(int test = 0; test<testinlees.at(0).size(); test++)
    {
        string teststring;
        teststring = "test" + test;
        imshow(teststring, testinlees.at(0).at(test));
    }
*/
    string ImageName1 = "Belgium.jpg";
    Mat Image1;

    string ImageName2 = "vorm2.jpg";
    Mat Image2;

    char stop;
    Mat frame;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_gray  ig|  |(required)}"
        "{image_color  ic|  |(required)}"
    );


// ------------------------------------------------------------------------ reading in test Image-------------------------------------------------
   Image1 = imread(ImageName1, IMREAD_COLOR);
   Image2 = imread(ImageName2, IMREAD_COLOR);


    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    imshow( "Original image", Image1 );

        //read out webcam
    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout << "Cam could notbe opened" << endl;
        return -1;
    }

   // ------------------------------------------------------------------------ --------------------------------------------------------------------
    component_analyse(Image1);
   /// We are going to base ourself on Session 2:
   /// Search red in our image
    namedWindow("thresholds", WINDOW_AUTOSIZE); // Create Window
    createTrackbar( "HUE minimum", "thresholds", &hue_min_slider, 180, on_trackbar );
    createTrackbar( "HUE maximum", "thresholds", &hue_max_slider, 180, on_trackbar );
    createTrackbar( "Saturation minimum", "thresholds", &saturation_min_slider, 255, on_trackbar );
    createTrackbar( "Saturation maximum", "thresholds", &saturation_max_slider, 255, on_trackbar );
    createTrackbar( "value minimimum", "thresholds", &value_min_slider, 255, on_trackbar );
    createTrackbar( "value maximum", "thresholds", &value_max_slider, 255, on_trackbar );
    imshow( "thresholds",Image1);
    vector<Point> hull1;
    vector<Point> hull2;
    vector<Point> hull3;
    double testshape = 100;
    double temp_testshape = 100;
    int index;
    hull2 = get_hulls(Image1);
    hull3 = get_hulls(Image2);

        while (true)
    {
        cap >> frame;
        hull1 = component_analyse(frame);
        hull2 = get_hulls(Image1);
        if(!hull1.empty())
        {
            for(int matchShapeCounter = 0; matchShapeCounter < CountryContours.size(); matchShapeCounter++)
            {
                temp_testshape = matchShapes(hull1, CountryContours.at(matchShapeCounter),1,1);
                if(temp_testshape <= testshape)
                {
                    testshape = temp_testshape;
                    index = matchShapeCounter;
                }
            }
            //cout << "testshape:" << index << endl;
            cout << "detected country" << CountryList.at(index) << endl;
        }
        else
        {
            //cout << "No hull detected" << endl;
        }
        imshow( "thresholds",frame);





        stop = (char) waitKey(30);
        if (stop == 'q')
        {
            break;
        }
    }


    waitKey(0);
    return 0;
}

vector<Point> component_analyse(Mat image)
{
    //afbeelding omzetten naar HSV
    Mat image_hsv;
    cvtColor(image, image_hsv, CV_BGR2HSV);
    imshow("image in HSV domein", image_hsv);


    int hue_low= hue_min_slider;
    int saturation_low = saturation_min_slider;
    int value_low = value_min_slider;

    int hue_high = hue_max_slider;
    int saturation_high = saturation_max_slider;
    int value_high = value_max_slider;

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
        double area0 = contourArea(contours.at(i));
        if(area0 > 50000)
        {
            hulls.push_back(contours.at(i));
        }
    }
   // printf("contour size of frames: %d \r\n", hulls.size());
    drawContours(finaal, hulls, -1, 255, -1);
    imshow("contours", finaal);


    Mat contours2;
    contours2 = Mat::zeros(image.rows, image.cols, CV_8UC3);
    drawContours(contours2, hulls, -1, 255, -1);
    imshow("contours2", contours2);

    ///samenvoegen afbeelding met masker over

    Mat totaal;
    totaal = Mat::zeros(image.rows, image.cols, CV_8UC3);
    image.copyTo(totaal,finaal);
    imshow("totaal", totaal);

    if(!hulls.empty())
    {
        return hulls.at(0);
    }
    else
    {
        return vector<Point>();
    }
}


vector<Point> get_hulls(Mat image)
{
    Mat image_hsv;
    cvtColor(image, image_hsv, CV_BGR2HSV);
    Mat finaal(image.rows, image.cols, CV_8UC3);
    inRange(image_hsv, Scalar(0, 100, 0), Scalar(180, 255, 255), finaal);
    imshow("get_hulls", finaal);

        ///contours en hulls

    vector< vector<Point>> contours;
    findContours(finaal.clone(), contours,  RETR_EXTERNAL, CHAIN_APPROX_NONE);
    vector< vector<Point>> hulls;

    for(size_t i=0; i<contours.size(); i++)
    {
            hulls.push_back(contours.at(i));
   //         cout << "contour grootte van templates:" << contourArea(contours.at(i)) << endl;
    }

    Mat contours2;
    contours2 = Mat::zeros(image.rows, image.cols, CV_8UC3);
    drawContours(contours2, hulls, -1, 255, -1);
    imshow("contours van template", contours2);

  //  printf("hulls shape: %d \r\n", hulls.size() );
    return hulls.at(0);
}

vector<string> getCountrylist(string pathName)
{
    vector<string> returnNames;
    vector<String> fileNames;
    String fullPathName = pathName + "/*.jpg";
    glob(fullPathName, fileNames, false);

    string tempString;
    for (size_t i=0; i<fileNames.size(); i++)
    {
        tempString = fileNames.at(i);
        tempString.erase(0,pathName.length()+1);
        tempString.pop_back();
        tempString.pop_back();
        tempString.pop_back();
        tempString.pop_back();
        cout << "found file for the list: " << tempString  << endl;
        returnNames.push_back(tempString);
    }
    return returnNames;
}



vector<vector<Point>> getCountryContours(string pathName)
{
    // vector filled with the contours of the countries
    vector<vector<Point>> contoursList;

    //variables
    Mat CountryImage;
    Mat CountryImage_hsv;
    Mat Total_Contour;
    //temporary contour vector for pushing in other vector
    vector<Point> contour;

    //stick .jpg after string
    String fullPathName = pathName + "/*.jpg";

    //vector that keeps the full path filenames
    vector<String> fileNames;

    //find all pathnames in directory
    glob(fullPathName, fileNames, false);

    // loop all pictures found in file
    size_t picture_count = fileNames.size(); //number of png files in images folder
    for (size_t i=0; i<picture_count; i++)
    {
        cout << "found file: " << fileNames.at(i)  << endl;
        // read in Images and put in Mat variable
        CountryImage = imread(fileNames.at(i), IMREAD_COLOR);

        // Convert to HSV space, easier for different tints of green
        cvtColor(CountryImage, CountryImage_hsv, CV_BGR2HSV);

        // We put a threshold on the image and only keep the green parts
        Mat Image_thresholded(CountryImage.rows, CountryImage.cols, CV_8UC3);
        inRange(CountryImage_hsv, Scalar(0, 100, 0), Scalar(180, 255, 255), Image_thresholded);

        // We find all the contours in the picture
        vector< vector<Point>> Allcontours;
        findContours(Image_thresholded.clone(), Allcontours,  RETR_EXTERNAL, CHAIN_APPROX_NONE);

        // We only keep the biggest contour
        for(size_t j=0; j<Allcontours.size(); j++)
        {
            if(contourArea(Allcontours.at(j)) > 50000)
            {
                contoursList.push_back(Allcontours.at(j));
            }
        }

    }
    vector< vector<Point>> test;
    test.push_back(contoursList.at(1));
    Total_Contour = Mat::zeros(CountryImage.rows, CountryImage.cols, CV_8UC3);
    drawContours(Total_Contour, test, -1, 255, -1);
    imshow("Total_Contour", Total_Contour);

    return contoursList;
}


vector<vector<Mat>> get_vacation_pictures(vector<string> countrynames, string pathNameToVacation)
{
    //list we are going to return
    vector<vector<Mat>> picturesList;
    vector<Mat> tempVector;
    Mat tempImage;
    vector<String> fileNames;
    String temp;
    size_t picture_count;

    //loop over every country we have a contour of
    for(int countrycounter=0; countrycounter<countrynames.size(); countrycounter++)
    {
        temp = pathNameToVacation + "/" + countrynames.at(countrycounter) + "/*.jpg";
        cout << "filename for path to pictures" << temp << endl;
        //get every picturepath in the file
        glob(temp, fileNames, false);

        //read in every picture in the file
        picture_count = fileNames.size(); //number of png files in images folder
        for (size_t i=0; i<picture_count; i++)
        {
            tempImage = imread(fileNames.at(i), IMREAD_COLOR);
            tempVector.push_back(tempImage);
        }
        picturesList.push_back(tempVector);
        tempVector.clear();
    }

    return picturesList;
}
