#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include <algorithm>
#include "fstream"

using namespace cv;
using namespace std;

int main(int argc,char** argv)
{


	if (argc != 2)
  {
    cout << "Please give the location of source sheet Music" <<endl;
    return -1;
  }

  vector<Vec4i> lines;
  Mat src,img_bw,img;

  img=imread(argv[1]);
  src=imread(argv[1],0);
  vector<int> y_coordinates;
  threshold(src, img_bw, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
  namedWindow("Threshold", WINDOW_NORMAL );
  


  HoughLinesP(img_bw, lines, 1, CV_PI/180,200,cvRound(img_bw.cols/2),30);

  cout<<"Number of lines detected = "<<lines.size()<<endl;

  for(size_t i=0;i<lines.size();i++)
  {
  	line( img_bw, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]),0,1, 8 );
  	//line( img, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]),Scalar(0,0,255),1, 8 );

  	y_coordinates.push_back((lines[i][1]+lines[i][3])/2);
  	//cout<<y_coordinates[i]<<endl;
  }

  sort(y_coordinates.begin(),y_coordinates.end());

  int topline=y_coordinates[0];
  int bottomline=y_coordinates[y_coordinates.size()-1];

  int lineDistance=(bottomline-topline)/4;

  float max_area=2*lineDistance*lineDistance;
  float min_area=0.5*lineDistance*lineDistance;

  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  //findContours( img_bw, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );


  dilate(img_bw, img_bw, Mat(), Point(-1, -1), 1, 1, 1);
  //dilate(img_bw, img_bw, Mat(), Point(-1, -1), 1, 1, 1);

  erode(img_bw, img_bw, Mat(), Point(-1, -1), 1, 1, 1);
  //erode(img_bw, img_bw, Mat(), Point(-1, -1), 1, 1, 1);

  //Mat dst;
  //distanceTransform(img_bw,dist, CV_DIST_L2, 3);

  

  imshow("Threshold",img_bw);
  imwrite("correct.jpg",img_bw);
  waitKey(0);


  vector<Vec4i> small_lines;
  HoughLinesP(img_bw,small_lines,1,CV_PI/180,200,cvRound(2*lineDistance),10);

  // vector<Vec2f> small_lines;

  // HoughLines(img_bw, small_lines, 1, CV_PI/180,100,0,0);

  cout<<"Removing vertical Lines"<<endl;

  for(size_t i=0;i<small_lines.size();i++)
  {
  	line( img, Point(small_lines[i][0], small_lines[i][1]),Point(small_lines[i][2], small_lines[i][3]),Scalar(0,0,255),1, 8 );
  	//line( img, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]),Scalar(0,0,255),1, 8 );
  }

  
  imshow("Just notes",img_bw);

 findContours( img_bw, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
 //    for(size_t i=0;i<contours.size();i++)
 //  {
 //  	// drawContours( img, contours, i, Scalar(0,255,0), 2, 8, hierarchy, 0, Point() );
 //  	// cout<<contourArea(contours[i])<<endl;
 //  	// imshow("Detected noteheads",img);
 //  	// waitKey(0);

 //  	drawContours( img, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
	// cout<<contourArea(contours[i])<<endl;
	// imshow("Detected noteheads",img);
	// waitKey(0);
 //  	// if(contourArea(contours[i])<max_area&&contourArea(contours[i])>min_area)
 //  	// {

 //  	// }
 //  }

  imshow("image",img);
  waitKey(0);

}
