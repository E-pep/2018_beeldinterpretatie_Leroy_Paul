#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// HOGDescriptor visual_imagealizer
// adapted for arbitrary size of feature sets and training images

Mat get_hogdescriptor_visu(const Mat& color_origImg, vector<float>& descriptorValues, const Size & size )
{
    const int DIMX = size.width;
    const int DIMY = size.height;
    float zoomFac = 3;
    Mat visu;
    resize(color_origImg, visu, Size( (int)(color_origImg.cols*zoomFac), (int)(color_origImg.rows*zoomFac) ) );

    int cellSize        = 8;
    int gradientBinSize = 9;
    float radRangeForOneBin = (float)(CV_PI/(float)gradientBinSize); // dividing 180 into 9 bins, how large (in rad) is one bin?

    // prepare data structure: 9 orientation / gradient strenghts for each cell
    int cells_in_x_dir = DIMX / cellSize;
    int cells_in_y_dir = DIMY / cellSize;
    float*** gradientStrengths = new float**[cells_in_y_dir];
    int** cellUpdateCounter   = new int*[cells_in_y_dir];
    for (int y=0; y<cells_in_y_dir; y++)
    {
        gradientStrengths[y] = new float*[cells_in_x_dir];
        cellUpdateCounter[y] = new int[cells_in_x_dir];
        for (int x=0; x<cells_in_x_dir; x++)
        {
            gradientStrengths[y][x] = new float[gradientBinSize];
            cellUpdateCounter[y][x] = 0;

            for (int bin=0; bin<gradientBinSize; bin++)
                gradientStrengths[y][x][bin] = 0.0;
        }
    }

    // nr of blocks = nr of cells - 1
    // since there is a new block on each cell (overlapping blocks!) but the last one
    int blocks_in_x_dir = cells_in_x_dir - 1;
    int blocks_in_y_dir = cells_in_y_dir - 1;

    // compute gradient strengths per cell
    int descriptorDataIdx = 0;
    int cellx = 0;
    int celly = 0;

    for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
    {
        for (int blocky=0; blocky<blocks_in_y_dir; blocky++)
        {
            // 4 cells per block ...
            for (int cellNr=0; cellNr<4; cellNr++)
            {
                // compute corresponding cell nr
                cellx = blockx;
                celly = blocky;
                if (cellNr==1) celly++;
                if (cellNr==2) cellx++;
                if (cellNr==3)
                {
                    cellx++;
                    celly++;
                }

                for (int bin=0; bin<gradientBinSize; bin++)
                {
                    float gradientStrength = descriptorValues[ descriptorDataIdx ];
                    descriptorDataIdx++;

                    gradientStrengths[celly][cellx][bin] += gradientStrength;

                } // for (all bins)


                // note: overlapping blocks lead to multiple updates of this sum!
                // we therefore keep track how often a cell was updated,
                // to compute average gradient strengths
                cellUpdateCounter[celly][cellx]++;

            } // for (all cells)


        } // for (all block x pos)
    } // for (all block y pos)


    // compute average gradient strengths
    for (celly=0; celly<cells_in_y_dir; celly++)
    {
        for (cellx=0; cellx<cells_in_x_dir; cellx++)
        {

            float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

            // compute average gradient strenghts for each gradient bin direction
            for (int bin=0; bin<gradientBinSize; bin++)
            {
                gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
            }
        }
    }

    // draw cells
    for (celly=0; celly<cells_in_y_dir; celly++)
    {
        for (cellx=0; cellx<cells_in_x_dir; cellx++)
        {
            int drawX = cellx * cellSize;
            int drawY = celly * cellSize;

            int mx = drawX + cellSize/2;
            int my = drawY + cellSize/2;

            rectangle(visu, Point((int)(drawX*zoomFac), (int)(drawY*zoomFac)), Point((int)((drawX+cellSize)*zoomFac), (int)((drawY+cellSize)*zoomFac)), Scalar(100,100,100), 1);

            // draw in each cell all 9 gradient strengths
            for (int bin=0; bin<gradientBinSize; bin++)
            {
                float currentGradStrength = gradientStrengths[celly][cellx][bin];

                // no line to draw?
                if (currentGradStrength==0)
                    continue;

                float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;

                float dirVecX = cos( currRad );
                float dirVecY = sin( currRad );
                float maxVecLen = (float)(cellSize/2.f);
                float scale = 2.5; // just a visualization scale, to see the lines better

                // compute line coordinates
                float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
                float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
                float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
                float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

                // draw gradient visualization
                line(visu, Point((int)(x1*zoomFac),(int)(y1*zoomFac)), Point((int)(x2*zoomFac),(int)(y2*zoomFac)), Scalar(0,255,0), 1);

            } // for (all bins)

        } // for (cellx)
    } // for (celly)


    // don't forget to free memory allocated by helper data structures!
    for (int y=0; y<cells_in_y_dir; y++)
    {
        for (int x=0; x<cells_in_x_dir; x++)
        {
            delete[] gradientStrengths[y][x];
        }
        delete[] gradientStrengths[y];
        delete[] cellUpdateCounter[y];
    }
    delete[] gradientStrengths;
    delete[] cellUpdateCounter;

    return visu;

} // get_hogdescriptor_visu


void detectAndDisplay(Mat frame, HOGDescriptor hog)
{
    Mat frame_grijs;
    cvtColor(frame,frame_grijs,COLOR_BGR2GRAY);
    equalizeHist(frame_grijs, frame_grijs);


    //detect faces

    vector<Rect> found;
    vector<double> score;
    hog.detectMultiScale(frame, found,score);
    for ( size_t i = 0; i < found.size(); i++ )
    {
        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.

        rectangle(frame, found[i].tl(),found[i].br(), cv::Scalar(0, 255, 0), 2);
        putText(frame, to_string(score[i]),found[i].tl(), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255), 2 , 8 , false);
    }
    //-- Show what you got
    imshow( "gevonden persoon", frame );
}

int main(int argc,const char** argv)
{
    string ImageName1 = "test.png";
    Mat Image1;
    Mat Image1_gray;
    Mat detected_edges;
    int lowThreshold = 100;
    int ratio = 3;
    int kernel_size = 3;

    HOGDescriptor d1;

    ///Adding a little help option and command line parser input
    CommandLineParser parser(argc,argv,
        "{help h|  |show this message}"
        "{image_gray  ig|  |(required)}"
        "{image_color  ic|  |(required)}"
    );


   Image1 = imread(ImageName1, IMREAD_COLOR);



    if( Image1.empty() )                      /// Check for invalid input
    {
        cout <<  "Could not open or find image1" << std::endl ;
        return -1;
    }

    imshow( "Original image", Image1 );


    //We need the gray color

    cvtColor(Image1, Image1_gray, CV_BGR2GRAY);

    ///finding the contour

    // Reduce noise with a kernel 3x3
    blur( Image1_gray, detected_edges, Size(3,3) );

    // Canny detector
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    imshow( "Canny", detected_edges );
    resize(detected_edges, detected_edges, cv::Size(200,200));

    //Trying to compute HOG

    vector<float> descriptorsValues;
    vector<Point> locations;
    d1.compute( detected_edges, descriptorsValues, Size(0,0), Size(0,0), locations);

    cout << "HOG descriptor size is " << d1.getDescriptorSize() << endl;
    cout << "img dimensions: " << detected_edges.cols << " width x " << detected_edges.rows << "height" << endl;
    cout << "Found " << descriptorsValues.size() << " descriptor values" << endl;
    cout << "Nr of locations specified : " << locations.size() << endl;

    Mat r1 = get_hogdescriptor_visu(detected_edges, descriptorsValues, Size(200,200) );
    imshow( "R1", r1 );

    detectAndDisplay(Image1, d1);

    waitKey(0);
    return 0;
}



