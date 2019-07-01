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
        tempi = imread(nodename, IMREAD_COLOR);   // Read the file
        if( tempi.empty() )                              // Check for invalid input
        {
            break;
        }
        temp.push_back(tempi);
        i++;
    }


    return temp;

}


Mat TekenLijnen(vector<Point2f> obj, vector<Point2f> scene,Mat img_matches,Mat pagina, vector<Point2f> scene_corners, vector<Point2f> obj_corners)
{
    Mat H;

    H = findHomography( obj, scene, RANSAC );

    if(!H.empty())
    {
        perspectiveTransform( obj_corners, scene_corners, H);
        line( img_matches, scene_corners[0] + Point2f( pagina.cols, 0), scene_corners[1] + Point2f( pagina.cols, 0), Scalar(0, 255, 0), 4 );
        line( img_matches, scene_corners[1] + Point2f( pagina.cols, 0), scene_corners[2] + Point2f( pagina.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[2] + Point2f( pagina.cols, 0), scene_corners[3] + Point2f( pagina.cols, 0), Scalar( 0, 255, 0), 4 );
        line( img_matches, scene_corners[3] + Point2f( pagina.cols, 0), scene_corners[0] + Point2f( pagina.cols, 0), Scalar( 0, 255, 0), 4 );
    }

    return img_matches;
}

vector< DMatch > Vindgoodmatches(Mat boek_desc,vector< DMatch > matches)
{
    //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist voor similariteit of deze goed genoeg is
    int min_dist_int = 100;
    double min_dist = 100;
    double max_dist = 0;
    double dist;
    vector< DMatch > good_matches;

            for( int i = 0; i < boek_desc.rows; i++)
            {
                dist = matches[i].distance;
                if (dist == 0)
                {
                    continue;
                }
                if( dist < min_dist_int ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }



            for( int i = 0; i < boek_desc.rows; i++ )
            {
                if( matches[i].distance <= 3*min_dist )
                {
                    good_matches.push_back( matches[i]);
                }
            }
    return good_matches;
}


int Beslispagina(int good_matches,int huidige_pagina)
{
    static int counter = 0;
    bool startcount = false;
    int aantal_kleiner = 0;
    bool andere_pagina = false;
    static int bestematchwaarde = 0;


    ///hoogste waarde deze pagina is het waar schijnlijk
    if(good_matches > bestematchwaarde)
    {
        bestematchwaarde = good_matches;
        cout  << "gevonden pagina "<< huidige_pagina<< endl;
    }

   // cout << good_matches << endl;

    ///bij langdurige drop van features, waarschijnlijk volgt een andere pagina => bestematchwaarde op 0 zetten en pagina met beste match zoeken
    if(good_matches < 150)
    {
        counter++;

        if(counter > 20)
        {
            //we moeten de pagina dus veranderen
            cout << "pagina moet verandert worden" << endl;
            huidige_pagina = -1;
            counter = 0;
            bestematchwaarde = 0;
        }


    }
    else                //bij korte drop
    {
        counter = 0;
    }


    return huidige_pagina;

}

