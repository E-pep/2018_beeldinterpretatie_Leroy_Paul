#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

extern Ptr<ORB> orb;
extern vector<vector<KeyPoint>> kp_FrontPages;
void readKpFrontPage(Mat image);
void readkpFile(Mat image);



#endif // MAIN_H_INCLUDED
