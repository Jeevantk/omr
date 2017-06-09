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

  Mat src,img_bw,img;

  vector<Vec4i> lines;
  vector<int> y_coordinates;

  img=imread(argv[1]);
  src=imread(argv[1],0);
  Mat edge,draw;
  Canny( src, edge, 50, 150, 3);
  edge.convertTo(draw, CV_8U);
  threshold(src, img_bw, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
  HoughLinesP(img_bw, lines, 1, CV_PI/180,200,cvRound(img_bw.cols/2),30);

  namedWindow("Threshold", WINDOW_NORMAL );


  imshow("Threshold",draw);


  for(size_t i=0;i<lines.size();i++)
  {
  	y_coordinates.push_back((lines[i][1]+lines[i][3])/2);
  }

  sort(y_coordinates.begin(),y_coordinates.end());

  int topline=y_coordinates[0];
  int bottomline=y_coordinates[y_coordinates.size()-1];

  int lineDistance=(bottomline-topline)/4;

  float max_area=2*lineDistance*lineDistance;
  float min_area=0.5*lineDistance*lineDistance;


  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  findContours( img_bw, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  cout<<"line width = "<<lineDistance<<endl;
  namedWindow("Detected noteheads",WINDOW_NORMAL);

  cout<<"Minimum Area Calculated = "<<min_area<<endl;
  cout<<"Maximum Area Calculated = "<<max_area<<endl; 
  
  for(size_t i=0;i<contours.size();i++)
  {
  	// drawContours( img, contours, i, Scalar(0,255,0), 2, 8, hierarchy, 0, Point() );
  	// cout<<contourArea(contours[i])<<endl;
  	// imshow("Detected noteheads",img);
  	// waitKey(0);


  	if(contourArea(contours[i])<max_area&&contourArea(contours[i])>min_area)
  	{
  		drawContours( img, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
  		cout<<contourArea(contours[i])<<endl;
  		imshow("Detected noteheads",img);
  		//waitKey(0);
  	}
  }

  
  
  imshow("Detected noteheads",img);
  waitKey(0);

}


