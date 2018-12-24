#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;
#include "main.h"

Ptr<ORB> orb = ORB::create();
vector<vector<KeyPoint>> kp_FrontPages;
vector<vector<KeyPoint>> kp_Book1Pages;

void AdKpFrontPage(Mat image,String nodename)
{
    vector<KeyPoint> kp;
    orb->detect( image, kp );

    cout << "in readKpFrontPages" << endl;
    FileStorage fs("kp_frontpages.yml", FileStorage::APPEND);
    write( fs , nodename, kp );

    fs.release();
    return;
}




void readkpFile(Mat image)
{
    cout << "in readkpFile" << endl;
    vector<KeyPoint> kp;
    Mat Imkp;
    FileStorage fs("kp_frontpages.yml", FileStorage::READ);
    FileNode kptFileNode = fs["kpfrontpage1"];
    read( kptFileNode, kp );
    cout << "kp value" << kp.empty() << endl;
    fs.release();
    drawKeypoints( image, kp, Imkp, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    imshow("image",Imkp);
    waitKey();
    return;
}

void ReadFPkps()
{
    int i = 1;
    vector<KeyPoint> temp;
    string nodename;
    string base = "kpfrontpage";
    FileStorage fs("kp_frontpages.yml", FileStorage::READ);
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
        kp_FrontPages.push_back(temp);
        i++;
    }
  cout << "size:" <<kp_FrontPages.size()<< endl;
  return;
}


string FindCover(Mat image)
{
    cout << "in FindCover" << endl;


}
