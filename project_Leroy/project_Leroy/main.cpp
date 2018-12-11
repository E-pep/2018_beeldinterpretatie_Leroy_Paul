#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
#include "main.h"


int main(int argc, const char** argv)
{
    Mat frame;
    Mat cover;

    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{book_cover1  bc1|  |(required)}"
     //   "{image_2  im2|  |(required)}"
    //    "{image_rotated  im2|  |(required)}"
     //   "{image_4  im4|  |(required)}"
    );


    string bookcover(parser.get<string>("book_cover1"));
    if(bookcover.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }

    cover = imread(bookcover, IMREAD_GRAYSCALE);
    cout << "test" << endl;
    //readKpFrontPage(cover);
    readkpFile(cover);
    //read out webcam
    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout << "Cam could not be opened" << endl;
        return -1;
    }

    /// initialization





    // press q to quit
    while(1)
    {
        cap >> frame;


        if(frame.empty())
        {
            cout << "Video finished" << endl;
            break;
        }
    }

    return 0;
}
