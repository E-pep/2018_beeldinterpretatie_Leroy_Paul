#include <iostream>
#include <opencv2/opencv.hpp>

//hulp: https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame, HOGDescriptor hog);

int main(int argc,const char** argv)
{
    ///variables
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{input_video  iv|  |(required)}"

    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string input_video_string(parser.get<string>("input_video"));
    cout << input_video_string << endl;


    //videocapture aanmaken

    VideoCapture input_video(input_video_string);
    Mat frame;

    if( !input_video.isOpened() )
    {
        cout << "video error" << endl;
    }

    while(1)
    {
        input_video >> frame;
        if(frame.empty())
        {
            break;
        }

        detectAndDisplay(frame, hog);

        waitKey(25);
    }

    waitKey(0);                                     /// Wait for a keystroke in the window

    return 0;
}

void detectAndDisplay(Mat frame, HOGDescriptor hog)
{
    Mat frame_grijs;
    cvtColor(frame,frame_grijs,COLOR_BGR2GRAY);
    equalizeHist(frame_grijs, frame_grijs);


    //detect faces

    vector<Rect> found;
    vector<double> score;
    hog.detectMultiScale(frame, found,score);
    for ( size_t i = 0; i < found.size(); i++ )
    {
        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.

        rectangle(frame, found[i].tl(),found[i].br(), cv::Scalar(0, 255, 0), 2);
        putText(frame, to_string(score[i]),found[i].tl(), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255), 2 , 8 , false);
    }
    //-- Show what you got
    imshow( "gevonden persoon", frame );
}


