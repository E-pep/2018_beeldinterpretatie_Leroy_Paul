#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "main.h"


int main(int argc, const char** argv)
{

    vector<vector<KeyPoint>> boek_kp;
    vector<Mat> boek_desc;

    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{titel bk|  |(required)}"
    );

    if(parser.has("help"))
    {
        cout << "geef de titel van het boek mee dat je wil lezen als argument" << endl;
        parser.printMessage();
        return 0;
    }

    ///Collect data from arguments

    string titel_boek(parser.get<string>("titel"));
    if(titel_boek.empty())
    {
        cout << "argument titel van boek niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }

    cout << "gevonden titel boek:" <<  titel_boek << endl;


    /// inlezen van kp van alle pagina's (opgeslagen in yml (detecteren)

    boek_kp = LeesKps(titel_boek);

    /// descriptor aan toevoegen (compute)

    boek_desc = LeesDesc()

    return 0;
}
