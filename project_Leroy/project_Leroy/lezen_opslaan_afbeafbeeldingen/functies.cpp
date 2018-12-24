#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;
#include "main.h"

Ptr<ORB> orb = ORB::create();
vector<vector<KeyPoint>> kp_FrontPages;
vector<vector<KeyPoint>> kp_Book1Pages;

void Addboek(vector<Mat> images, String titel)
{
    vector<KeyPoint> kp;
    Mat dp;

    string nodename;
    string base = "pagina";


    string fs1string;
    string fs2string;
    string base1 = "_kp.yml";
    string base2 = "_dp.yml";

    fs1string = titel + base1;
    fs2string = titel + base2;

    FileStorage fs(fs1string, FileStorage::APPEND);
    FileStorage fs2(fs2string, FileStorage::APPEND);

    for(int i = 0; i< images.size(); i++)
    {
        orb->detect( images[i], kp );
        orb->compute(images[i], kp, dp);
        nodename = base  + to_string(i);
        write( fs , nodename, kp );
        write( fs2 , nodename, dp );
    }

    fs.release();
    fs2.release();

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
