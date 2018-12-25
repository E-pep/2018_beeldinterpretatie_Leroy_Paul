#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



vector<vector<KeyPoint>> LeesKps(string titel);
vector<Mat> Leesfotos(string titel);
Mat TekenLijnen(vector<Point2f> obj, vector<Point2f> scene,Mat img_matches,Mat pagina, vector<Point2f> scene_corners, vector<Point2f> obj_corners);
vector< DMatch > Vindgoodmatches(Mat boek_desc,vector< DMatch > matches);

int Beslispagina(int good_matches,int huidige_pagina);


#endif // MAIN_H_INCLUDED
