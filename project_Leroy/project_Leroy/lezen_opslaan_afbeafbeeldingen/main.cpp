#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
#include "main.h"


int main(int argc, const char** argv)
{
    Mat frame;
    Mat cover;
    string NodeBook;

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

 ///this is only needed when you want to add new picture keypoints


  //  readkpFile(cover);

 ///


    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout << "Cam could not be opened" << endl;
        return -1;
    }

    /// initialization

        //reading all front pages known
        ReadFPkps();
        cout << "laat de cover zien van het boek aan de webcam" << endl;
        //find which cover is on the webcam
        while(1)
        {
            cap >> frame;

            NodeBook = FindCover(frame);

            if(frame.empty())
            {
                cout << "Video finished" << endl;
                break;
            }
        }



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
