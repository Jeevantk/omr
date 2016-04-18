#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/text.hpp"
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;
Mat src,img_bw;


void selectSort(int arr[], int n)
{
	int pos_min,temp;
  for (int i=0; i < n-1; i++)
	{
	  pos_min = i;
		for (int j=i+1; j < n; j++)
		{
      if (arr[j] < arr[pos_min])
        pos_min=j;
    }
    if (pos_min != i)
    {
      temp = arr[i];
      arr[i] = arr[pos_min];
      arr[pos_min] = temp;
    }
	}
}



vector<int> x_coords_using_vertical_line_detection(vector<Vec2f> lines)
{
  int count=0;
  vector<int> x_coords;

  for( size_t i = 0; i < lines.size(); i++ )
  {
      float rho = lines[i][0], theta = lines[i][1];
      Point pt1, pt2;
      if (theta>-0.1 and theta <0.1)
      {
        int x_avg;
        count++;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        x_avg=(pt1.x+pt2.x)/2;
        x_coords.push_back(x_avg);
      }
  }
	int x_lines[x_coords.size()];
	for (int i=0;i<x_coords.size();i++)
	{
		x_lines[i]=x_coords[i];
	}
  selectSort(x_lines,x_coords.size());
	int size=x_coords.size();
  x_coords.clear();
	int current_x_coordinate=0;
	for (int i=1;i<size;i++)
	{
		if((x_lines[i]-current_x_coordinate)>10)
		{
			x_coords.push_back((current_x_coordinate+x_lines[i-1])/2);
			current_x_coordinate=x_lines[i];
		}
	}
	x_coords.push_back(current_x_coordinate);
	x_coords.erase(x_coords.begin());
	for(int i=0;i<x_coords.size();i++)
	{
		cout<<x_coords[i]<<endl;

	}
	cout<<"Total number of vertical lines ="<<x_coords.size()<<endl;
  return x_coords;

}






vector<int> barline_detection_using_dist_transform(vector<int> x_coordinates)
{
		Mat dist;
		vector<int> bars;
		Mat drawing=Mat::zeros(img_bw.rows,img_bw.cols,CV_8UC1);
		Mat final=Mat::zeros(img_bw.rows,img_bw.cols,CV_8UC1);
		int x_starting=0;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		for(int i=0;i<x_coordinates.size()-1;i++)
		{
			Mat cropped=img_bw(Rect(x_starting,0,((x_coordinates[i]+x_coordinates[i+1])/2-x_starting),img_bw.rows-1));
			findContours(cropped, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
			Mat temp=Mat::zeros(cropped.rows,cropped.cols,CV_8UC1);
			for(int j=0;j<contours.size();j++)
			{
				drawContours(temp, contours,j,255, -1, 8, hierarchy, 0, Point() );
			}
			temp.copyTo(drawing(Rect(x_starting,0,((x_coordinates[i]+x_coordinates[i+1])/2-x_starting),img_bw.rows-1)));
			x_starting=(x_coordinates[i]+x_coordinates[i+1])/2;
		}
		Mat cropped=img_bw(Rect(x_starting,0,img_bw.cols-x_starting,img_bw.rows-1));
		findContours(cropped, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		Mat temp=Mat::zeros(cropped.rows,cropped.cols,CV_8UC1);
		for(int j=0;j<contours.size();j++)
		{
			drawContours(temp, contours,j,255, -1, 8, hierarchy, 0, Point() );
		}
		temp.copyTo(drawing(Rect(x_starting,0,img_bw.cols-x_starting,img_bw.rows-1)));
		distanceTransform(drawing,dist, CV_DIST_L2, 3);
	  normalize(dist,dist, 0.0, 1.0, NORM_MINMAX);
		imshow("distance transformed image",dist);
		threshold(dist,final, .5, 1., CV_THRESH_BINARY);
		imshow("only the peaks",final);
		x_starting=0;
		for(int i=0;i<x_coordinates.size()-1;i++)
		{

			Mat cropped=final(Rect(x_starting,0,((x_coordinates[i]+x_coordinates[i+1])/2-x_starting),img_bw.rows-1));
			if (countNonZero(cropped) < 1)
			{
				bars.push_back(x_coordinates[i]);
			}
			x_starting=(x_coordinates[i]+x_coordinates[i+1])/2;
		}
		bars.push_back(x_coordinates[x_coordinates.size()-1]);
		cout<<"Giving the coordinates of the bar lines"<<endl;
		for(int i=0;i<bars.size();i++)
		{
			cout<<bars[i]<<endl;
		}
		return bars;
}



//
int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout << "Please give the location of source sheet Music as ./test /path/to/MusicSheet" <<endl;
    return -1;
  }


  src =imread(argv[1]);
  imshow("original image",src);
  Mat img=imread(argv[1],0);
  if (src.empty())
  {
    cout << "No Image found found at the given location,please try again" <<endl;
    return -1;
  }
  vector<Vec2f> lines;
  threshold(img, img_bw, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  bitwise_not(img_bw,img_bw);
  Mat grad_x,abs_grad_x;
  Sobel(img_bw, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );
  //imshow("horizontal edge detection",abs_grad_x);
  HoughLines(abs_grad_x, lines, 1, CV_PI/180,18, 0, 0 );

  vector <int> x_coordinates=x_coords_using_vertical_line_detection(lines);
	vector <int> bar_lines=barline_detection_using_dist_transform(x_coordinates);



	for(int i=0;i<bar_lines.size();i++)
	{
		line(src,Point(bar_lines[i],0),Point(bar_lines[i],(src.rows-1)), Scalar(0,0,255), 1, CV_AA);
	}
  imshow("Bar lines Detected",src);
	imwrite("bars.jpg",src);
	//imshow("binary image",img_bw);

  //imwrite("horixontal.jpg",abs_grad_x);


  waitKey(0);
}
