#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;


Mat Image1;
Mat Image_points;
vector<Point> punten;
vector<Point> punten_background;

static void callbackMouse1( int event, int x, int y, int flags, void *userdata)
{
    if( event == EVENT_LBUTTONDOWN )
    {
        punten.push_back(Point(x,y));
    }

    else if(event == EVENT_RBUTTONDOWN)
    {
        if(punten.size() == 0)
        {
            printf("niets meer verwijderbaar/r/n");
        }
        else
        {
            punten.pop_back();
        }
    }

}

static void callbackMouse2( int event, int x, int y, int flags, void *userdata)
{
    if( event == EVENT_LBUTTONDOWN )
    {
        punten_background.push_back(Point(x,y));
    }

    else if(event == EVENT_RBUTTONDOWN)
    {
        if(punten_background.size() == 0)
        {
            printf("niets meer verwijderbaar/r/n");
        }
        else
        {
            punten_background.pop_back();
        }
    }
}


int main(int argc,const char** argv)
{
    ///variabless
    string ImageName1;



    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_interface  if|  |(required)}"

    );

    if(parser.has("help"))
    {
        cout << "geef volledig path van foto's mee als argument" << endl;
        parser.printMessage();
        return 0;
    }


    ///Collect data from arguments

    string image_interface(parser.get<string>("image_interface"));
    if(image_interface.empty())
    {
        cout << "argument niet gevonden" << endl;
        parser.printMessage();
        return -1;
    }



    ///inlezen van afbeelding1

   Image1 = imread(image_interface, IMREAD_COLOR);
   GaussianBlur(Image1,Image1,Size(5,5),0,0);





    if( Image1.empty() )                      /// Check for invalid input
    {
        cout << "Could not open or find image1" << std::endl ;
        return -1;
    }


    /// Opdracht:1 Maak een interface waarop je op basis van mouse callbacks pixels kan aanklikken.

    imshow("interface", Image1);

    cout << "geef goede punten met aardbei, druk key om door te gaan" << endl;
    setMouseCallback("interface", callbackMouse1,0);
    waitKey();
    cout << "geef background punten" << endl;
    setMouseCallback("interface", callbackMouse2,0);

    waitKey();


    ///opdracht 2: Op basis van de geselecteerd punten bepaalje een descriptor, in dit geval de HSV representatie van de pixel.

    Mat image_HSV;

    cvtColor(Image1, image_HSV, COLOR_BGR2HSV);

    //prepare foreground training data

    Mat TrainingDataForeground(punten.size(),3, CV_32FC1);
    Mat labels_fg = Mat::ones(punten.size(),1, CV_32FC1);
    Mat gesplitst[3];
    split(image_HSV,gesplitst);

    Mat V = gesplitst[2];
    Mat S = gesplitst[1];
    Mat H = gesplitst[0];



    for(int i = 0; i< punten.size(); i++)
    {
        TrainingDataForeground.at<float>(i,0) = H.at<uchar>(punten[i]);
        TrainingDataForeground.at<float>(i,1) = S.at<uchar>(punten[i]);
        TrainingDataForeground.at<float>(i,2)= V.at<uchar>(punten[i]);
    }

    cout << "einde foreground" << endl;
    cout << "TrainingDataforeground:" << endl;

    for(int k = 0; k < TrainingDataForeground.cols; k++)
    {
          for(int l = 0; l < TrainingDataForeground.rows; l++)
            {
                cout << "trainingdataforeground" << (int) TrainingDataForeground.at<float>(k,l) << endl;
            }
    }

    //nu voor achtergrond


    Mat TrainingDataBackground(punten.size(),3, CV_32FC1);
    Mat labels_bg = Mat::zeros(punten.size(),1, CV_32FC1);



    for(int i = 0; i< punten_background.size(); i++)
    {
        TrainingDataBackground.at<float>(i,0) = H.at<uchar>(punten_background[i]);
        TrainingDataBackground.at<float>(i,1) = S.at<uchar>(punten_background[i]);
        TrainingDataBackground.at<float>(i,2)= V.at<uchar>(punten_background[i]);
    }
    cout << "einde background" << endl;
    //volledige data

    Mat TrainingData;
    Mat labels;

    vconcat(TrainingDataForeground,TrainingDataBackground, TrainingData);
    vconcat(labels_fg, labels_bg, labels);

    cout << "einde samenvoegen" << labels << endl;
    cout << "labels" <<labels << endl;

    for(int k = 0; k < TrainingData.cols; k++)
    {
          for(int l = 0; l < TrainingData.rows; l++)
            {
                cout << "trainingdata" << (int) TrainingData.at<float>(k,l) << endl;
            }
    }


    ///opdracht 3 Train met deze data een K-Nearest-Neighbor classifier, een Normal Bayes Classifier en een Support Vector Machine

    //K-Nearest-Neighbor classifier

    Ptr<KNearest> knn = KNearest::create();
    Ptr<TrainData>  TrainingData_knn = TrainData::create(TrainingData,ROW_SAMPLE, labels);
    knn->setIsClassifier(true);
    knn->setAlgorithmType(KNearest::BRUTE_FORCE);
    knn->setDefaultK(3);
    knn->train(TrainingData_knn);

    //Normal Bayes Classifier
    Ptr<NormalBayesClassifier> normalbayes = NormalBayesClassifier::create();
    normalbayes->train(TrainingData, ROW_SAMPLE, labels);

    //Support Vector Machine

    cout << "einde trainen classifiers" << labels << endl;
    ///Opdracht 4: Classificeer de pixels van de afbeelding met deze technieken en visualiseer met een masker de resterende pixels.


    Mat mask_knn = Mat::zeros(Image1.rows, Image1.cols, CV_8UC1);
    Mat mask_normalbayes = Mat::zeros(Image1.rows, Image1.cols, CV_8UC1);
    Mat labels_knn;
    Mat labels_normalbayes;
    Mat data_bins(1,3,CV_32FC1);
    Vec3b pixel;

    for( int i = 0; i < Image1.rows; i++ ){
        for( int j = 0; j < Image1.cols; j++ ){

            //huidige waarden
            data_bins.at<float>(0,0) = H.at<uchar>(i,j);
            data_bins.at<float>(0,1) = S.at<uchar>(i,j);
            data_bins.at<float>(0,2) = V.at<uchar>(i,j);


            knn->findNearest(data_bins, knn->getDefaultK(), labels_knn);
            normalbayes->predict(data_bins, labels_normalbayes);


            mask_knn.at<uchar>(i,j) = labels_knn.at<float>(0,0);
            mask_normalbayes.at<uchar>(i,j) = labels_normalbayes.at<float>(0,0);


        }
    }


    // maskers  1*255 voor duidelijk verschil
    mask_knn *= 255;
    mask_normalbayes *=255;
    // resultaat laten zien

    imshow("masker knn", mask_knn);
    imshow("masker bayes", mask_normalbayes);
    waitKey(0);
    return 0;
}
