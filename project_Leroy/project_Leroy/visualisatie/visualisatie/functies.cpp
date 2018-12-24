#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;
#include "main.h"

Ptr<ORB> orb = ORB::create();
vector<vector<KeyPoint>> kp;

vector<vector<KeyPoint>> LeesKps(string titel)
{
    int i = 1;

    //string van file
    string filename;
    string fileend = ".yml";
    filename = titel + fileend;

    //string nodes in file
    vector<KeyPoint> temp;
    string nodename;
    string base = "pagina";


    FileStorage fs(filename, FileStorage::READ);
    while(1)
    {
        nodename = base  + to_string(i);
        cout << nodename <<endl;
        FileNode kptFileNode = fs[nodename];
        cout << kptFileNode.empty()<< endl;
        if(kptFileNode.empty())
        {
            cout << "test" <<endl;
           fs.release();
           break;
        }
        read(kptFileNode, temp);
        kp.push_back(temp);
        i++;
    }
    cout << "size:" <<kp.size()<< endl;

    return kp;
}
