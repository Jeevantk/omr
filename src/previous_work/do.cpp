#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
int main()
{
  Mat im_in = imread("1.jpg", IMREAD_GRAYSCALE);
  Mat im_th;

  threshold(im_in, im_th, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  bitwise_not(im_th,im_th);
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(im_th, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  //Mat drawing=Mat::zeros(im_th.rows,im_th.cols,CV_8UC1);
  Mat drawing=Mat::zeros(im_th.rows,im_th.cols,CV_8UC1);
  for(int i=0;i<contours.size();i++)
  {
    //double a=contourArea( contours[i],false);
    //if(a<100)

    drawContours( drawing, contours, i,255, -1, 8, hierarchy, 0, Point() );
    imshow("filled contours",drawing);
    waitKey(0);
  }
  imwrite("show.jpg",drawing);
  /*Mat im_floodfill = im_th.clone();
  imshow("before filling",im_floodfill);
  floodFill(im_floodfill,Point(0,0), Scalar(255));
  imshow("flood fill",im_floodfill);*/

}
