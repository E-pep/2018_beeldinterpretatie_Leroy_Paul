#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;


/// Global variables

// default values for the sliders
int thresholdSlider = 12;
double thresholdSliderDouble = (double)thresholdSlider/100;

int ImageWidth = 400;
int ImageHeight = 400;


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
    ///The commandlineparser for variables
    CommandLineParser parser(argc,argv,
        "{help h             | | show this message}"
        "{pathContours  pc   | | Path to the directory containing the contours(required)}"
        "{pathVacation  pv   | | Path to the directory containing the vacation pictures(required)}"
    );

        // Help printing
    if(parser.has("help") || argc <= 1) {
        cerr << "Please use absolute paths when supplying your images." << endl;
        parser.printMessage();
        return 0;
    }

    // Parser fail
    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }



///-------------------------------------------------------- Variables -----------------------------------------------------------------------------------------------
    string pathName(parser.get<string>("pathContours"));
    string pathNameToVacation(parser.get<string>("pathVacation"));
    string defaultPictureName = "default.jpg";

    //check if arguments are filled in
    if(pathName.empty() )
    {
        cerr << "give the contour pathname please" << endl;
        return -1;
    }

    if(pathName.empty() )
    {
        cerr << "give the vacation pathname please" << endl;
        return -1;
    }

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
    vector<Point> hull1;
    vector<Point> hull2;
    vector<Point> hull3;
    double testshape = 100;
    int index;
    double temp_testshape = 100;
    Mat FrameSmall;




/// ------------------------------------------------------------------------ reading in Images-------------------------------------------------

   defaultPicture = imread(defaultPictureName, IMREAD_COLOR);

    // Check for invalid input
    if( defaultPicture.empty() )
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }
    // make the image smaller
    resize(defaultPicture, defaultPicture, Size(ImageWidth, ImageHeight));

    //read out webcam
    //cap 1 is the external camera
    VideoCapture cap(1);

    if(!cap.isOpened())
    {
        cout << "Cam could notbe opened" << endl;
        return -1;
    }
   /// ------------------------------------------------------------------------ --------------------------------------------------------------------

   //create a window with a trackbar for the threshold
    namedWindow("thresholds", WINDOW_AUTOSIZE);
    createTrackbar( "threshold", "thresholds", &thresholdSlider, 50, on_trackbar );
    imshow( "thresholds",defaultPicture);

    while (true)
    {
        // put the captured frame in Mat object
        cap >> frame;

        // Find the contour of the country in the center
        hull1 =  ContoursFrame_WholeMap(frame);
        if(!hull1.empty())
        {
            testshape = 100;

            // We iterate over every country contour and compare it with the contour of the country in the center
            for(int matchShapeCounter = 0; matchShapeCounter < CountryContours.size(); matchShapeCounter++)
            {
                // calculate similarity between 2 contours, how lower the result, how more simularity there is
                // rotation and scale invariant
                // based on hue moments
                // source: https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_contours/py_contours_more_functions/py_contours_more_functions.html
                temp_testshape = matchShapes(hull1, CountryContours.at(matchShapeCounter),1,1);
                cout << "match value: " <<temp_testshape << endl;

                // save the most similar country shape, and below certain threshold to ensure we have a country
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
        resize(frame, FrameSmall, Size(ImageWidth, ImageHeight));
        imshow( "thresholds",FrameSmall);

        // iterate over the images of the found country and show them
        for(int ImageCounter = 0; ImageCounter <picturesVector.at(index).size(); ImageCounter++)
        {
            string temp_string = "foto"+ImageCounter;
            if(testshape < thresholdSliderDouble)
            {
                imshow(temp_string, picturesVector.at(index).at(ImageCounter));

            }
            // if no country found, show default image
            else
            {
                imshow(temp_string, defaultPicture);
            }
           // imshow(temp_string, picturesVector.at(index).at(ImageCounter));
        }



        //When q is pressed, the program stops
        stop = (char) waitKey(30);
        if (stop == 'q')
        {
            break;
        }
    }


    return 0;
}




vector<string> getCountrylist(string pathName)
{
    vector<string> returnNames;
    vector<String> fileNames;
    String fullPathName = pathName + "/*.jpg";
    // read every filename in directory
    glob(fullPathName, fileNames, false);

    // we delete the.jpg and only save the name itself
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
    // This is a method based on Colour like Labo 2. This is because the first version used colour to detect a country.
    // The disadvantage was we could only show 1 contour of a country
    // vector filled with the contours of the countries
    vector<vector<Point>> contoursList;

    //variables
    Mat CountryImage;
    Mat CountryImage_hsv;
    Mat Total_Contour;
    //temporary contour vector for pushing in other vector
    vector<Point> contour;

    Point2f center;
    float radius;

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
            // only the biggest country to compare
            if(contourArea(Allcontours.at(j)) > 50000)
            {
                contoursList.push_back(Allcontours.at(j));
            }
        }

    }
    // drawcontours needs a vector of vectorpoints
    vector< vector<Point>> test;
    test.push_back(contoursList.at(1));

    Total_Contour = Mat::zeros(CountryImage.rows, CountryImage.cols, CV_8UC3);
    drawContours(Total_Contour, test, -1, 255, -1);

    // find the center of our contour
    Moments moment = moments((cv::Mat)test.at(0));
    double area = moment.m00;
    int x,y;
    x = moment.m10/area;
    y = moment.m01/area;
    circle( Total_Contour, Point(x,y), 7, Scalar( 0, 0, 255 ), FILLED, LINE_8 );
    resize(Total_Contour, Total_Contour, Size(ImageWidth, ImageHeight));
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
            // make them smaller
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
    //variables
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
    Mat ClaheSmall;
    resize(claheThreshold, ClaheSmall, Size(ImageWidth, ImageHeight));
    imshow("CLAHE ", ClaheSmall);

    ///contours
    // search for contours in the thresholded frame
    findContours(claheThreshold.clone(), contours,  RETR_EXTERNAL, CHAIN_APPROX_NONE);


    // We only want bigger contours. Smalls ones are difficult to compare
    for(int i=0; i<contours.size(); i++)
    {
        areaContour = contourArea(contours.at(i));

        if(areaContour > 40000)
        {
            bigContours.push_back(contours.at(i));
        }
    }

    //We draw the contours on a seperate image
    Mat ContoursTotal;
    ContoursTotal = Mat::zeros(image.rows, image.cols, CV_8UC3);
    drawContours(ContoursTotal, bigContours, -1, 255, -1);


    //We calculate the center of the contours and look which one is closest to the center of the frame
    for(int j = 0; j < bigContours.size(); j++)
    {
        // Image moments help you to calculate some features like center of mass of the object, area of the object etc.
        // source: https://docs.opencv.org/3.4.2/dd/d49/tutorial_py_contour_features.html.
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

    //draw the center on the total image
    circle( ContoursTotal, Point(xClosest,yClosest), 7, Scalar( 0, 0, 255 ), FILLED, LINE_8 );
    Mat ContourSmall;
    resize(ContoursTotal, ContourSmall, Size(ImageWidth, ImageHeight));
    imshow("ContoursTotal", ContourSmall);

    // return the center contour or an empty contour if nothing is found
    if(!bigContours.empty())
    {
        return bigContours.at(returnIndexbigContours);
    }
    else
    {
        return vector<Point>();
    }
}


