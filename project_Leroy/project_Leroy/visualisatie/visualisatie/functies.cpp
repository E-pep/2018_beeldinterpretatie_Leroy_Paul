#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;
#include "main.h"


vector<vector<KeyPoint>> LeesKps(string titel)
{
    vector<vector<KeyPoint>> kp;
    int i = 0;

    //string van file
    string filename;
    string fileend = ".yml";
    filename = titel+ "_kp"+ fileend;

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
           kp.pop_back();
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





vector<Mat> Leesfotos(string titel)
{
    vector<Mat> temp;
    Mat tempi;
    string nodename;
    string base = "pagina";
    int i = 0;
    while(1)
    {
        nodename = titel +"/" + base  + to_string(i) + ".jpg";
        cout << nodename <<endl;
        tempi = imread(nodename, IMREAD_GRAYSCALE);   // Read the file
        if( tempi.empty() )                              // Check for invalid input
        {
            break;
        }
        temp.push_back(tempi);
        i++;
    }


    return temp;

}
