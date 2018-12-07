#include <iostream>
#include <opencv2/opencv.hpp>

//hulp: https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame, CascadeClassifier face_cascade);
void detectAndDisplaylbp(Mat frame, CascadeClassifier face_cascade);

int main(int argc,const char** argv)
{
    ///variables
    CascadeClassifier face_cascade;
    CascadeClassifier lbp_cascade;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{input_video  iv|  |(required)}"
        "{face_cascade  fc|  |(required)}"
        "{lbp_cascade  lc|  |(required)}"
    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string input_video_string(parser.get<string>("input_video"));
    string face_cascade_string(parser.get<string>("face_cascade"));
    string lbp_cascade_string(parser.get<string>("lbp_cascade"));

    ///Opdracht 1: Maak een interface waarin je de fases.mp4 video kan afspelen, daarna gebruik je het Viola en Jones detectie algoritme om met het HAAR face model gezichten
    ///            in elk frame te vinden. Duid deze aan met een cirkel en een detectie score.

        //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_string ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };

    if( !lbp_cascade.load( lbp_cascade_string ) )
    {
        cout << "--(!)Error loading lbp cascade\n";
        return -1;
    };

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
        ///Opdracht 1

        detectAndDisplay(frame, face_cascade);

        ///Opdracht 2: Detecteer nu niet enkel met het HAAR face model, maar ook het LBP model. Wat merk je van verschillen?

        detectAndDisplaylbp(frame, lbp_cascade);

        waitKey(25);
    }

    waitKey(0);                                     /// Wait for a keystroke in the window

    return 0;
}



void detectAndDisplay(Mat frame, CascadeClassifier face_cascade)
{
    Mat frame_grijs;
    cvtColor(frame,frame_grijs,COLOR_BGR2GRAY);
    equalizeHist(frame_grijs, frame_grijs);


    //detect faces

    vector<Rect> faces;
    vector<int> score;
    face_cascade.detectMultiScale(frame_grijs, faces,score,1.05,3);


    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        putText(frame, to_string(score[i]),center, FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255), 2 , 8 , false);
    }
    //-- Show what you got
    imshow( "gevonden gezichten", frame );
}

void detectAndDisplaylbp(Mat frame, CascadeClassifier face_cascade)
{
    Mat frame_grijs;
    cvtColor(frame,frame_grijs,COLOR_BGR2GRAY);
    equalizeHist(frame_grijs, frame_grijs);


    //detect faces

    vector<Rect> faces;
    vector<int> score;
    face_cascade.detectMultiScale(frame_grijs, faces,score,1.05,3);


    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4 );
        putText(frame, to_string(score[i]),center, FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255), 2 , 8 , false);
    }
    //-- Show what you got
    imshow( "gevonden gezichten lbp", frame );
}
