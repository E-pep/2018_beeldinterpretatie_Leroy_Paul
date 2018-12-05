#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


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
        TrainingDataForeground.at<float>(i,0) = H.at<float>(punten[i]);
        TrainingDataForeground.at<float>(i,1) = S.at<float>(punten[i]);
        TrainingDataForeground.at<float>(i,2)= V.at<float>(punten[i]);
    }

    cout << "einde foreground" << endl;
    //nu voor achtergrond


    Mat TrainingDataBackground(punten.size(),3, CV_32FC1);
    Mat labels_bg = Mat::zeros(punten.size(),1, CV_32FC1);



    for(int i = 0; i< punten_background.size(); i++)
    {
        TrainingDataBackground.at<float>(i,0) = H.at<float>(punten_background[i]);
        TrainingDataBackground.at<float>(i,1) = S.at<float>(punten_background[i]);
        TrainingDataBackground.at<float>(i,2)= V.at<float>(punten_background[i]);
    }

    //volledige data

    Mat TrainingData;
    Mat labels;

    vconcat(TrainingDataForeground,TrainingDataBackground, TrainingData);
    vconcat(labels_fg, labels_bg, labels);

    cout << "trainingsdata:" << TrainingData << endl;
    cout << "labels:" << labels << endl;

    ///opdracht 3 Train met deze data een K-Nearest-Neighbor classifier, een Normal Bayes Classifier en een Support Vector Machine

    //knn classifier


    waitKey(0);
    return 0;
}
