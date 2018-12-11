#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
#include "main.h"

Ptr<ORB> orb = ORB::create();
vector<vector<KeyPoint>> kp_FrontPages;
vector<vector<KeyPoint>> kp_Book1Pages;

void readKpFrontPage(Mat image)
{
    vector<KeyPoint> kp;
    orb->detect( image, kp );

    cout << "in readKpFrontPages" << endl;
    FileStorage fs("kp_frontpages.yml", FileStorage::APPEND);
    write( fs , "kpfrontpage", kp );

    fs.release();
    return;
}

void readkpFile(Mat image)
{
    cout << "in readkpFile" << endl;
    vector<KeyPoint> kp;
    Mat Imkp;
    FileStorage fs("kp_frontpages.yml", FileStorage::READ);
    FileNode kptFileNode = fs["kpfrontpage"];
    read( kptFileNode, kp );
    fs.release();
    drawKeypoints( image, kp, Imkp, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    imshow("image",Imkp);
    waitKey();
    return;
}
