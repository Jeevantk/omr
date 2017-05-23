#include "opencv2/highgui/highgui.hpp"
#include "stdio.h"
#include "fstream"
#include "iostream"


using namespace std;
using namespace cv;
int i,x1,x2,p1,y2;


void CallBackFunc(int event,int x,int y,int flags,void*userdata)
{
	
	if(event==EVENT_LBUTTONDOWN)
	{
		if(x1==0)
		{
			x1=x;
			p1=y;
		}
		else if(x2==0)
		{
			x2=x;
			y2=y;
		}
		
	}
	
}
int main()
{

	Mat img;
	ifstream x;
	x.open("../data/music_notation/list.txt");
	string output;
	int counter=0;

	ifstream save;
	save.open("../data/music_notation/names.txt");

	string savename;

	getline(save,savename);


	while(getline(x,output))
	{
		output="../data/music_notation/" +output;
		getline(save,savename);
		x1=0,p1=0,x2=0,y2=0;
		counter++;
		img=imread(output.c_str());
		namedWindow("Cropping",1);
		setMouseCallback("Cropping",CallBackFunc,NULL);
		imshow("Cropping",img);
		printf("%d\n", counter);
		waitKey(0);
		Mat image=Mat::zeros(img.rows,img.cols,CV_8UC3);
		//printf("%d %d %d %d",x1,p1,x2,y2);
		Mat croppedImage = img(Rect(x1, p1, x2 - x1, y2 - p1));
		destroyWindow("Cropping");
		savename="../data/music_notation/" +savename;
		imwrite(savename.c_str(),croppedImage);
		//imwrite("test.jpg",croppedImage);
		
	}
	x.close();

}

