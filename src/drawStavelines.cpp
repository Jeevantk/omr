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
  imshow("Threshold",img_bw);

  HoughLinesP(img_bw, lines, 1, CV_PI/180,200,cvRound(img_bw.cols/2),30);

  cout<<"Number of lines detected = "<<lines.size()<<endl;

  for(size_t i=0;i<lines.size();i++)
  {
  	line( img, Point(lines[i][0], lines[i][1]),Point(lines[i][2], lines[i][3]),Scalar(0,0,255),1, 8 );
  	y_coordinates.push_back((lines[i][1]+lines[i][3])/2);
  	//cout<<y_coordinates[i]<<endl;
  }
  cout<<CV_PI<<endl;
  imshow("image",img);
  waitKey(0);

}
