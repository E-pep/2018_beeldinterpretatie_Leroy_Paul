
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

int main(int argc,const char** argv)
{
    ///variabless
    Mat Image1;
    Mat Image1_object;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_1  im1|  |(required)}"
        "{image_2  im2|  |(required)}"
    //    "{image_rotated  im2|  |(required)}"
     //   "{image_4  im4|  |(required)}"
    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string image_1(parser.get<string>("image_1"));
    if(image_1.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }

    string image_2(parser.get<string>("image_2"));
    if(image_2.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }


    ///inlezen van afbeeldingen

   Image1 = imread(image_1, IMREAD_GRAYSCALE);
   Image1_object = imread(image_2, IMREAD_GRAYSCALE);

    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    if( Image1_object.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }


///Opdracht 1: Bepaal op de inputbeelden en op de te zoeken templates keypoints, gebaseerd op de volgende keypoint types: ORB, BRISK en AKAZE.

//ORB start

    Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> keypoints_1_ORB, keypoints_2_ORB;

    orb->detect( Image1_object, keypoints_2_ORB );
    orb->detect( Image1, keypoints_1_ORB );



    //Draw keypoints
    Mat Image1_kp_ORB;
    Mat Image1_object_kp_ORB;

    drawKeypoints( Image1, keypoints_1_ORB, Image1_kp_ORB, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( Image1_object, keypoints_2_ORB, Image1_object_kp_ORB, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //show keypoints
    imshow("ORB:Keypoints image1", Image1_kp_ORB );
    imshow("ORB:Keypoints object 1", Image1_object_kp_ORB );

//ORB end


//start BRISK

    Ptr<BRISK> brisk = BRISK::create();
    vector<KeyPoint> keypoints_1_BRISK, keypoints_2_BRISK;


    brisk->detect( Image1, keypoints_1_BRISK );
    brisk->detect( Image1_object, keypoints_2_BRISK );


    //Draw keypoints
    Mat Image1_kp_BRISK;
    Mat Image1_object_kp_BRISK;

    drawKeypoints( Image1, keypoints_1_BRISK, Image1_kp_BRISK, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( Image1_object, keypoints_2_BRISK, Image1_object_kp_BRISK, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //show keypoints
    imshow("BRISK:Keypoints image1 ", Image1_kp_BRISK );
    imshow("BRISK:Keypoints object 1", Image1_object_kp_BRISK );

//BRISK end


//start AKAZE

    Ptr<AKAZE> akaze = AKAZE::create();
    vector<KeyPoint> keypoints_1_AKAZE, keypoints_2_AKAZE;


    akaze->detect( Image1, keypoints_1_AKAZE );
    akaze->detect( Image1_object, keypoints_2_AKAZE );


    //Draw keypoints
    Mat Image1_kp_AKAZE;
    Mat Image1_object_kp_AKAZE;

    drawKeypoints( Image1, keypoints_1_AKAZE, Image1_kp_AKAZE, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( Image1_object, keypoints_2_AKAZE, Image1_object_kp_AKAZE, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //show keypoints
    imshow("AKAZE:Keypoints image1 ", Image1_kp_AKAZE );
    imshow("AKAZE:Keypoints object 1", Image1_object_kp_AKAZE );

//AKAZE end




///Opdracht 2: Kies 1 van bovenstaande keypoint detectoren, bepaal descriptoren van de gegenereerde keypoints en doe een brute force matching tussen beide sets.

    Mat descriptors_1, descriptors_2;


 //   orb->detect( Image1, keypoints_1_ORB );
 //   orb->detect( Image1_object, keypoints_2_ORB );
    orb->compute(Image1_object, keypoints_2_ORB, descriptors_2);
    orb->compute(Image1, keypoints_1_ORB, descriptors_1);


    //-- Step 2: Matching descriptor vectors with a brute force matcher
    BFMatcher matcher(NORM_L2);
    vector< DMatch > matches;
    matcher.match( descriptors_2, descriptors_1, matches );

    //-- Draw matches
    Mat img_matches;
    drawMatches( Image1_object, keypoints_2_ORB, Image1, keypoints_1_ORB, matches, img_matches );
    //-- Show detected matches
    imshow("Matches", img_matches );


///Opdracht 3: Pas RANSAC toe om enkel de keypoint matches over te houden die op iets slaan. Gebruik de resulterende matches om een homography uit te rekenen en het object in uw doelbeeld te vinden.


  double max_dist = 0; double min_dist = 100;
  //-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < descriptors_2.rows; i++ )
  {
      double dist = matches[i].distance;
    if (dist == 0)
        {
            continue;
        }
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }


  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );


  //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
  std::vector< DMatch > good_matches;
  for( int i = 0; i < descriptors_2.rows; i++ )
  { if( matches[i].distance <= 3*min_dist )
     { good_matches.push_back( matches[i]); }
  }

  Mat img_matches2;
  drawMatches( Image1_object, keypoints_2_ORB, Image1, keypoints_1_ORB,
               good_matches, img_matches2, Scalar::all(-1), Scalar::all(-1),
               std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

  //-- Localize the object
  //-- Localize the object
  vector<Point2f> obj;
  vector<Point2f> scene;
  for( size_t i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    obj.push_back( keypoints_2_ORB[ good_matches[i].queryIdx ].pt );
    scene.push_back( keypoints_1_ORB[ good_matches[i].trainIdx ].pt );
  }
  Mat H = findHomography( obj, scene, RANSAC );

  //-- Get the corners from the image_1 ( the object to be "detected" )
  vector<Point2f> obj_corners(4);
  obj_corners[0] = cvPoint(0,0);
  obj_corners[1] = cvPoint( Image1_object.cols, 0 );
  obj_corners[2] = cvPoint( Image1_object.cols, Image1_object.rows );
  obj_corners[3] = cvPoint( 0, Image1_object.rows );
  std::vector<Point2f> scene_corners(4);

  perspectiveTransform( obj_corners, scene_corners, H);

  cout << good_matches.size() << endl;
  cout << obj_corners << endl;
  cout << scene_corners << endl;

  //-- Draw lines between the corners (the mapped object in the scene - image_2 )
  line( img_matches2, scene_corners[0] + Point2f( Image1_object.cols, 0), scene_corners[1] + Point2f( Image1_object.cols, 0), Scalar(0, 255, 0), 4 );
  line( img_matches2, scene_corners[1] + Point2f( Image1_object.cols, 0), scene_corners[2] + Point2f( Image1_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches2, scene_corners[2] + Point2f( Image1_object.cols, 0), scene_corners[3] + Point2f( Image1_object.cols, 0), Scalar( 0, 255, 0), 4 );
  line( img_matches2, scene_corners[3] + Point2f( Image1_object.cols, 0), scene_corners[0] + Point2f( Image1_object.cols, 0), Scalar( 0, 255, 0), 4 );
  //-- Show detected matches
  imshow( "Good Matches & Object detection", img_matches2 );

  waitKey(0);
  return 0;
  }
