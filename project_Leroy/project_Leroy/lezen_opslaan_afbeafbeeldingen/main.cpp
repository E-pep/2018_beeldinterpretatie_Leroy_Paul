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
    Mat testimage;
    int teller = 0;
    string paginastring;
    string start = "/pagina";
    string base = ".jpg";

    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{titel  bk|  |(required)}"
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
    cout << "gevonden titel:" << titel << endl;


    while(1)
    {
        paginastring = titel+start + to_string(teller) + base;
        cout << "inlezen pagina" <<paginastring << endl;
        images.push_back(imread(paginastring,IMREAD_GRAYSCALE));

        if (images[teller].empty())
        {
            images.pop_back();
            break;
        }

        teller++;
    }
    cout << "boek klaar met inlezen, aantal paginas : "<< images.size() << endl;

    Addboek(images, titel);
    readkpFile(images[0]);


    return 0;
}
