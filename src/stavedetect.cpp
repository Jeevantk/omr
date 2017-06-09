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

  Mat drawing=Mat::zeros(img.rows,img.cols,CV_8UC1);
   //threshold(src, img_bw,180, 255,1);
  threshold(src, img_bw, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
  namedWindow("Threshold", WINDOW_NORMAL );
  imshow("Threshold",img_bw);
  //waitKey(0);
  //cout<<img.cols<<endl;
  //Usage of Hough line Transform for staff line detection
  HoughLinesP(img_bw, lines, 1, CV_PI/180,200,cvRound(img_bw.cols/2),30);
  cout<<"Number of lines detected = "<<lines.size()<<endl;
  vector<int> y_coordinates;

  namedWindow("image", WINDOW_NORMAL);


  cout<<"Starting to draw lines"<<endl;

  //Drawing the lines and saving their y_coordinates to a file

  for(size_t i=0;i<lines.size();i++)
  {
  	line( drawing, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]),255,15, 8 );
  	y_coordinates.push_back((lines[i][1]+lines[i][3])/2);
  	//cout<<y_coordinates[i]<<endl;
  }
  
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  findContours( drawing, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );

  for( size_t i = 0; i < contours.size(); i++ )
  {
    approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    boundRect[i] = boundingRect( Mat(contours_poly[i]) );
  }

  vector<int> stave_heights;

  for(size_t i=0;i<contours.size();i++)
  {
  	Scalar color = Scalar(255,0,0);
    //rectangle( img, boundRect[i].tl(), boundRect[i].br(), color, 3, 8, 0 );
    stave_heights.push_back(boundRect[i].height);
    // cout<<boundRect[i].width<<endl;
    // cout<<boundRect[i].height<<endl;
  }

  // Clef Detection can be used here to verify that the detected staves are accurate.
  // To be done later after the initial model of OMR is done.





  cout<<"Number of staves detected = " <<contours.size()<<endl;

  // sort(y_coordinates.begin(),y_coordinates.end());
  // for(size_t i=0;i<y_coordinates.size();i++)
  // {
  // 	cout<<y_coordinates[i]<<endl;
  // }

  //vector<int> y_coordinates;
  // for(size_t i=0;i<lines.size();i++)
  // {
  // 	y_coordinates.push_back((lines[i][1]+lines[i][3])/2);
  // 	cout<<y_coordinates[i]<<endl;
  // }


//The image should be straight by the time it reaches this part of the program. All the work in straightening 
// has to be done before this




  cout<<"Saving individal staves as images "<<endl;

  ofstream outfile;

  imshow("image",img);
  
  for (int i=0;i<contours.size();i++)
  {
  	Rect roi;
  	Point top;
  	Point bottom;
  	top=boundRect[i].tl();
  	bottom=boundRect[i].br();
  	int height=boundRect[i].height;
  	int width=boundRect[i].width;
  	top.x=(top.x-height)>0 ? top.x-height : 0 ;
  	top.y=(top.y-height/2);
  	bottom.x=(bottom.x+height)<img.cols ? bottom.x+height: img.cols-1;
  	bottom.y=bottom.y+height/2;
  	roi.x=top.x;
  	roi.y=top.y;
  	roi.width=bottom.x-top.x;
  	roi.height=bottom.y-top.y;
  	cout<<top.y<<endl;
  	Mat stave=img(roi);
  	imshow("Cropped",stave);
  	ostringstream convert;
  	convert << i; 
  	string name=convert.str()+".jpg";
  	imwrite(name.c_str(),stave);
  	waitKey(0);
  }
  
  imwrite("Show.jpg",img);
  waitKey(0);
}