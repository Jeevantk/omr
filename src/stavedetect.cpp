#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include <algorithm>

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
  threshold(src, img_bw,160, 255,1);
  namedWindow("Threshold", WINDOW_NORMAL );
  imshow("Threshold",img_bw);
  waitKey(0);
  cout<<img.cols<<endl;
  //Usage of Hough line Transform for staff line detection
  HoughLinesP(img_bw, lines, 1, CV_PI/180,200,cvRound(img_bw.cols/2),30);
  cout<<lines.size()<<endl;

  for(size_t i=0;i<lines.size();i++)
  {
  	line( img, Point(lines[i][0], lines[i][1]),
	Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
  }
  
  namedWindow("image", WINDOW_NORMAL );
  imshow("image",img);
  imwrite("Show.jpg",img);
  waitKey(0);
}