#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
  int a=1;
  if (argc != 2)
  {
      cout << "Please give the location of source sheet Music as ./test /path/to/MusicSheet" <<endl;
      return -1;
  }
  Mat img =imread(argv[1]);
  if (img.empty())
  {
      cout << "No Image found found at the given location,please try again" <<endl;
      return -1;
  }
  Mat gray;
  cvtColor(img, gray, CV_BGR2GRAY);
  Size size(int((400.0/img.rows)*img.cols),400);

  resize(img,img,size);
  resize(gray,gray,size);

  //adaptiveThreshold(img,img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10);
  //threshold(img,img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  Mat dst;
  //imwrite("0.jpg",img);
  bitwise_not (gray,gray);
  //cout<<"Enter the time signature"<<endl;
  //cin >>a;
  namedWindow("Initial Image",WINDOW_NORMAL);
  imshow("Initial Image",img);
  Canny(img, dst, 50, 200, 3);
  vector<Vec2f> lines;
  HoughLines(dst, lines, 1, CV_PI/180,110, 0, 0 );
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Mat test=Mat::zeros(img.size(),CV_8UC1);
  Mat detected=Mat::zeros(img.size(),CV_8UC1);
  for( size_t i = 0; i < lines.size(); i++ )
  {
      float rho = lines[i][0], theta = lines[i][1];
      if (theta>1.5 && theta <2.0)
      {
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line(test, pt1, pt2, 255, 3, CV_AA);
      }
  }
  namedWindow("detected lines",WINDOW_NORMAL);
  imshow("detected lines",test);
  findContours(test, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
  vector<Rect> boundRect( contours.size() );
  vector<vector<Point> > contours_poly( contours.size() );
  for( int i = 0; i< contours.size(); i++ )
  {
    drawContours(detected, contours, i,255,3, 8, hierarchy, 0, Point() );
    //approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    //imshow("Individual Stave",img(boundingRect[i]));
    //waitKey(0);
  }
  namedWindow("Contours_filles",WINDOW_NORMAL);
  imshow("Contours_filles",detected);
  //floodFill(test,Point(0,0),255);
  imwrite("output.jpg",test);
  waitKey(0);
}
