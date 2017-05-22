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
  vector<Vec2f> lines;
  Mat src,img_bw,img;

  img=imread(argv[1]);
  src=imread(argv[1],0);
  //imshow("original image",src);
  Mat grad_x,abs_grad_x;
  threshold(src, img_bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  Sobel(img_bw, grad_x, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  

  HoughLines(abs_grad_x, lines, 1, CV_PI/180,30,30,10);
  cout<<lines.size()<<endl;

  for(size_t i=0;i<lines.size();i++)
  {
  	float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        if(theta>1.56 && theta <1.60)
        {
	        pt1.x = 0;
	        pt1.y = cvRound(y0 + 1000*(a));
	        pt2.x = src.cols-1;
	        pt2.y = cvRound(y0 - 1000*(a));
	        line( img, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
    	}	
  }
  namedWindow("gradient", WINDOW_NORMAL );
  imshow("gradient",abs_grad_x);

  namedWindow("image", WINDOW_NORMAL );
  imshow("image",img);
  //imwrite("lines.jpg",img);
  waitKey(0);
}