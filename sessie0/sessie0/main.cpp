#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc,const char** argv)
{
    cout << "Hello world!" << endl;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help |  |show this message}"
        "{image_gray  |  |(required)}"
        "{image_color  |  |(required)}"
    );

    if(parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    ///Collect data from arguments
    string image1(parser.get<string>("image_gray"));
    if(image1.empty())
    {
        cout << "image1 niet meegegeven" << endl;
        parser.printMessage();
        return -1;
    }

    string image2(parser.get<string>("image_color"));
    if(image2.empty())
    {
        cout << "image 2 niet meegegeven" << endl;
        parser.printMessage();
        return -1;
    }

    ///inlezen van afbeelding
   /// Mat img = imgread(parameter);



    return 0;
}
