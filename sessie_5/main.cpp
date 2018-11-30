#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat Image1;
Mat Image_points;
vector<Point> punten;
vector<Point> punten_background;

static void callbackMouse1( int event, int x, int y, int flags, void *userdata)
{
    cout << "callback1" << endl;
    if( event == EVENT_LBUTTONDOWN )
    {
        punten.push_back(Point(x,y));
    }

    else if(event == EVENT_RBUTTONDOWN)
    {
        if(punten.size() == 0)
        {
            printf("niets meer verwijderbaar/r/n");
        }
        else
        {
            punten.pop_back();
        }
    }

}

static void callbackMouse2( int event, int x, int y, int flags, void *userdata)
{
    cout << "callback2" << endl;
    if( event == EVENT_LBUTTONDOWN )
    {
        punten_background.push_back(Point(x,y));
    }

    else if(event == EVENT_RBUTTONDOWN)
    {
        if(punten_background.size() == 0)
        {
            printf("niets meer verwijderbaar/r/n");
        }
        else
        {
            punten_background.pop_back();
        }
    }
}


int main(int argc,const char** argv)
{
    ///variabless
    string ImageName1;



    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_interface  if|  |(required)}"

    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string image_interface(parser.get<string>("image_interface"));
    if(image_interface.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }



    ///inlezen van afbeelding1

   Image1 = imread(image_interface, IMREAD_COLOR);
   GaussianBlur(Image1,Image1,Size(5,5),0,0);





    if( Image1.empty() )                      /// Check for invalid input
    {
        cout << "Could not open or find image1" << std::endl ;
        return -1;
    }


    /// Opdracht:1 interface -------------------------------------------------------------------
    imshow("interface", Image1);

    setMouseCallback("interface", callbackMouse1,0);
    waitKey();

    setMouseCallback("interface", callbackMouse2,0);

    waitKey();


    /// Opdracht:2

    Mat image_HSV;

    cvtColor(Image1, image_HSV, COLOR_BGR2HSV);

    //prepare foreground training data

    Mat TrainingDataForeground(punten,3, CV_32FC1);
    Mat labels_fg = Mat::ones(punten,1, CV_32FC1);

    for(int i = 0; i< punten.size(); i++)
    {
        Vec3b =

    }

    ///opdracht 3

    //linear niet gevoelig aan overfitten


    waitKey(0);
    return 0;
}
