#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <time.h>

using namespace cv;
using namespace cv::ml;
using namespace std;

void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector );
void draw_locations( Mat & img, const vector< Rect > & locations, const Scalar & color );


void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector )
{
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);

    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
               (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    hog_detector.clear();

    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;
}

void draw_locations( Mat & img, const vector< Rect > & locations, const Scalar & color )
{
    if( !locations.empty() )
    {
        vector< Rect >::const_iterator loc = locations.begin();
        vector< Rect >::const_iterator end = locations.end();
        int i=1;
        for( ; loc != end ; ++loc )
        {	
        	cout<<i<<endl;
        	i++;
            rectangle( img, *loc, color, 2 );
        }
    }
    else
    {
    	cout<<"No locations detected"<<endl;
    }
}



int main(int argc, char** argv)
{
	Mat img,draw;
	Ptr<SVM> svm;
	Size size=Size(32,64);
	Scalar trained( 0, 0, 255 );
	img=imread(argv[1]);
    HOGDescriptor my_hog;
    my_hog.winSize = size;
    vector< Rect > locations;
    svm = StatModel::load<SVM>( "../data/trainedModels/bassCleffDetect.yml" );
    vector< float > hog_detector;

    get_svm_detector( svm, hog_detector );
    my_hog.setSVMDetector( hog_detector );

    draw = img.clone();
    locations.clear();

    my_hog.detectMultiScale( img, locations );
    draw_locations( draw, locations, trained );
    namedWindow("Detected Clefs",CV_WINDOW_NORMAL);
    imshow("Detected Clefs",draw);
    string output= "clefdetect.png";
    imwrite(output.c_str(),draw);
    waitKey(0);


}