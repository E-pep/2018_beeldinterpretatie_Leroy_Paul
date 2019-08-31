#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;


/// Global variables

// default values for the sliders
int thresholdSlider = 12;
double thresholdSliderDouble = (double)thresholdSlider/100;

int ImageWidth = 200;
int ImageHeight = 200;


///functions

//get a vector filled with every country name
vector<string> getCountrylist(string pathName);
//get a vector filled with every country contour
vector<vector<Point>> getCountryContours(string pathName);

// returns the contour of the country with it's center closest to frame center
vector<Point> ContoursFrame_WholeMap(Mat image);

// vector with all the vacation pictures. Index = which country
vector<vector<Mat>> get_vacation_pictures(vector<string> countrynames, string pathNameToVacation);


//
static void on_trackbar( int, void* )
{
    thresholdSliderDouble = (double)thresholdSlider/100;
}

int main(int argc,const char** argv)
{
    string pathName = "/home/paul/Desktop/school/1819/2018_beeldinterpretatie_Leroy_Paul/AR_Landkaart_Leroy/AR_Landkaart/country_contours";
    string pathNameToVacation = "/home/paul/Desktop/school/1819/2018_beeldinterpretatie_Leroy_Paul/AR_Landkaart_Leroy/AR_Landkaart/vacation_pictures";
    string defaultPictureName = "default.jpg";
    Mat defaultPicture;
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

    char stop;
    Mat frame;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_gray  ig|  |(required)}"
        "{image_color  ic|  |(required)}"
    );


// ------------------------------------------------------------------------ reading in Images-------------------------------------------------

   defaultPicture = imread(defaultPictureName, IMREAD_COLOR);

    if( defaultPicture.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }
    resize(defaultPicture, defaultPicture, Size(ImageWidth, ImageHeight));

    //read out webcam
    //cap 1 is the external camera
    VideoCapture cap(1);

    if(!cap.isOpened())
    {
        cout << "Cam could notbe opened" << endl;
        return -1;
    }
   // ------------------------------------------------------------------------ --------------------------------------------------------------------
   /// We are going to base ourself on Session 2:
   /// Search red in our image
    namedWindow("thresholds", WINDOW_AUTOSIZE); // Create Window
    createTrackbar( "threshold", "thresholds", &thresholdSlider, 50, on_trackbar );
    imshow( "thresholds",defaultPicture);
    vector<Point> hull1;
    vector<Point> hull2;
    vector<Point> hull3;
    double testshape = 100;
    //auto start, ende;
    int index;
    double temp_testshape = 100;
    while (true)
    {
        cap >> frame;
        hull1 =  ContoursFrame_WholeMap(frame);
        if(!hull1.empty())
        {
            testshape = 100;
            for(int matchShapeCounter = 0; matchShapeCounter < CountryContours.size(); matchShapeCounter++)
            {
                temp_testshape = matchShapes(hull1, CountryContours.at(matchShapeCounter),1,1);
                cout << "match value: " <<temp_testshape << endl;
                if(temp_testshape <= testshape && temp_testshape < thresholdSliderDouble)
                {
                    testshape = temp_testshape;
                    index = matchShapeCounter;
                }
            }
            //cout << "testshape:" << index << endl;
            cout << "detected country" << CountryList.at(index) << endl;
            cout << "test shape" << testshape << endl;
            cout << "thresholdSliderDouble" << thresholdSliderDouble << endl;
        }
        imshow( "thresholds",frame);

        for(int ImageCounter = 0; ImageCounter <picturesVector.at(index).size(); ImageCounter++)
        {
            string temp_string = "foto"+ImageCounter;
            if(testshape < thresholdSliderDouble)
            {
                imshow(temp_string, picturesVector.at(index).at(ImageCounter));

            }
            else
            {
                imshow(temp_string, defaultPicture);
            }
           // imshow(temp_string, picturesVector.at(index).at(ImageCounter));
        }




        stop = (char) waitKey(30);
        if (stop == 'q')
        {
            break;
        }
    }


    waitKey(0);
    return 0;
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





   Point2f center;
    float radius;
 //   minEnclosingCircle(test.at(0), center, radius);
    Total_Contour = Mat::zeros(CountryImage.rows, CountryImage.cols, CV_8UC3);
    drawContours(Total_Contour, test, -1, 255, -1);
    Moments moment = moments((cv::Mat)test.at(0));
    double area = moment.m00;
    int x,y;
    x = moment.m10/area;
    y = moment.m01/area;
    circle( Total_Contour, Point(x,y), 7, Scalar( 0, 0, 255 ), FILLED, LINE_8 );
 /*   circle( Total_Contour, center, 7, Scalar( 0, 0, 255 ), FILLED, LINE_8 );
    circle( Total_Contour, center, radius, Scalar( 255, 0, 255 ), 2, LINE_8 );*/
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
            resize(tempImage, tempImage, Size(ImageWidth, ImageHeight));
            tempVector.push_back(tempImage);
        }
        picturesList.push_back(tempVector);
        tempVector.clear();
    }

    return picturesList;
}


vector<Point> ContoursFrame_WholeMap(Mat image)
{
    vector< vector<Point>> contours;
    vector< vector<Point>> bigContours;
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    double areaContour;
    Moments moment;
    double area;
    int x,y;
    int xClosest =0;
    int yClosest =0;
    int returnIndexbigContours = 0;
    Mat ImageGray;
    cvtColor(image, ImageGray, CV_BGR2GRAY);

    /// CLAHE
    // thresholding
    // Less noise than OTSU
    //contrast amplification is limited, so as to reduce this problem of noise amplification.
    //source: https://en.wikipedia.org/wiki/Adaptive_histogram_equalization#Contrast_Limited_AHE
    Mat result_clahe;
    Ptr<CLAHE> clahe_pointer = createCLAHE();
    clahe_pointer->setTilesGridSize(Size(15,15));
    clahe_pointer->setClipLimit(1);
    clahe_pointer->apply(ImageGray.clone(), result_clahe);
    Mat claheThreshold = Mat::zeros(ImageGray.rows, ImageGray.cols, CV_8UC1);
    threshold(result_clahe, claheThreshold, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("CLAHE ", claheThreshold);

    ///contours
    // search for contours in the thresholded frame
    findContours(claheThreshold.clone(), contours,  RETR_EXTERNAL, CHAIN_APPROX_NONE);


    //
    for(int i=0; i<contours.size(); i++)
    {
        areaContour = contourArea(contours.at(i));

        if(areaContour > 40000)
        {
            bigContours.push_back(contours.at(i));
        }
    }

    Mat ContoursTotal;
    ContoursTotal = Mat::zeros(image.rows, image.cols, CV_8UC3);
    drawContours(ContoursTotal, bigContours, -1, 255, -1);


    for(int j = 0; j < bigContours.size(); j++)
    {
        moment = moments((cv::Mat)bigContours.at(j));
        area = moment.m00;
        x = moment.m10/area;
        y = moment.m01/area;


        if( (abs(x-image.cols/2) < abs(xClosest - image.cols/2)) && (abs(y-image.cols/2) < abs(x-yClosest - image.cols/2)))
        {
            xClosest = x;
            yClosest = y;
            returnIndexbigContours = j;
        }
    }
    circle( ContoursTotal, Point(xClosest,yClosest), 7, Scalar( 0, 0, 255 ), FILLED, LINE_8 );

    imshow("ContoursTotal", ContoursTotal);


    if(!bigContours.empty())
    {
        return bigContours.at(returnIndexbigContours);
    }
    else
    {
        return vector<Point>();
    }
}


