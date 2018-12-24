#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;
#include "main.h"


int main(int argc, const char** argv)
{
    Mat pagina;
    vector<Mat> images;
    int teller = 0;
    string paginastring;
    string start = "/pagina";
    string base = ".jpg";

    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{titel  bt|  |(required)}"
     //   "{image_2  im2|  |(required)}"
    //    "{image_rotated  im2|  |(required)}"
     //   "{image_4  im4|  |(required)}"
    );


    string titel(parser.get<string>("titel"));
    if(titel.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }



    while(1)
    {
        paginastring = titel+start + to_string(teller) + base;
        images[teller] = imread(paginastring,IMREAD_GRAYSCALE);

        if (images[teller].empty())
        {
            cout << "boek klaar met inlezen, aantal paginas : "<< teller << endl;
            break;
        }

        teller++;
    }

 ///this is only needed when you want to add new picture keypoints


  //  readkpFile(cover);

 ///


    return 0;
}
