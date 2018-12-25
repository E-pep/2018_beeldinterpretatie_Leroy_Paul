#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "main.h"


int main(int argc, const char** argv)
{
    vector<Mat> paginas;
    vector<vector<KeyPoint>> boek_kp;
    Mat boek_desc;
    Ptr<DescriptorMatcher> matcher;
    matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
    Mat frame;
    Mat img_matches;
    Ptr<Feature2D> detector;
    detector = ORB::create();


    vector<KeyPoint> keypoints_frame;
    Mat descriptors_frame;
    vector< DMatch > matches;
    vector< DMatch > good_matches;


    int huidige_pagina = 0;

    vector<Point2f> obj;
    vector<Point2f> scene;
    vector<Point2f> swapvector;
    Mat H;
    vector<Point2f> scene_corners(4);



    ///hoeken van pagina's
    vector<Point2f> obj_corners(4);


    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{titel bk|  |(required)}"
        "{foto f|   |(required)}"
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


    ///inlezen van de foto's, nodig voor descriptors aan te maken
    paginas = Leesfotos(titel_boek);


    ///hoekwaardes ingeven

    obj_corners[0] = cvPoint(0,0);
    obj_corners[1] = cvPoint( paginas[0].cols, 0 );
    obj_corners[2] = cvPoint( paginas[0].cols, paginas[0].rows );
    obj_corners[3] = cvPoint( 0, paginas[0].rows );


    /// inlezen van kp van alle pagina's (opgeslagen in yml (detecteren)

    boek_kp = LeesKps(titel_boek);


    /// maken van descriptors (compute)

    cout<< "test:" << boek_kp.size() << endl;
    Mat temp;
    /// openen van camera
    VideoCapture cap(0);

    if(!cap.isOpened())
    {
        cout << "kan camera niet openen" << endl;
        return -1;
    }

    Mat img_keypoints;





    ///ONEINDIGE LUS
    huidige_pagina = 0;

    while(cap.isOpened())
    {
        cap >> frame;

        if(frame.empty())
        {
            cout << "Video finished" << endl;
            break;
        }

        ///detecteren keypoints in frame
        detector->detect( frame, keypoints_frame );
        detector->compute(frame, keypoints_frame, descriptors_frame);

        ///vergelijken van descriptors beide afbeeldingen en goeie eruithalen
        Mat testp = paginas[huidige_pagina];
        vector<KeyPoint> kp = boek_kp[huidige_pagina];

        detector->compute(testp,kp,boek_desc);
        matcher->match( boek_desc, descriptors_frame, matches);


        ///alleen de goede matches overhouden
        good_matches = Vindgoodmatches(boek_desc, matches);



        huidige_pagina = Beslispagina(good_matches.size(), huidige_pagina);

        ///nieuwe pagina zoeken
        if(huidige_pagina == -1)
        {
            int houbij = 0;
            for(int newpagelus = 0; newpagelus < 5; newpagelus++)
            {
                Mat searchpage = paginas[newpagelus];
                vector<KeyPoint> kps = boek_kp[newpagelus];
                detector->compute(searchpage,kps,boek_desc);
                matcher->match( boek_desc, descriptors_frame, matches);

                good_matches = Vindgoodmatches(boek_desc, matches);
                if(good_matches.size() > houbij)
                {
                    houbij = good_matches.size();
                    huidige_pagina = newpagelus;
                }
            }
        }



                drawMatches( paginas[huidige_pagina], boek_kp[huidige_pagina], frame, keypoints_frame,good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
                for( size_t i = 0; i < good_matches.size(); i++ )
                {
                    //-- Get the keypoints from the good matches
                    obj.push_back( boek_kp[huidige_pagina][ good_matches[i].queryIdx ].pt );
                    scene.push_back( keypoints_frame[ good_matches[i].trainIdx ].pt );
                }




                if(good_matches.size() > 0)
                {

                    img_matches = TekenLijnen(obj, scene, img_matches,paginas[0], scene_corners, obj_corners);

                }



        imshow( "comparing", img_matches );
        good_matches.clear();
        obj.clear();
        scene.clear();
        scene_corners.clear();
        waitKey(25);
    }


    return 0;
}
