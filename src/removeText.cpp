#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
//#include <tesseract/baseapi.h>
#include <iostream>
#include <vector>
#include <opencv2/text.hpp>

using namespace cv;
using namespace std;
using namespace cv::text;

Mat img;
vector <int> box_heights;


vector<int> y_coordinates_of_various_lines_of_text(vector<Rect> boxes,vector<float> confidences,vector<string> words)
{
  vector <int> y_coordinates;
  int current_y_coordinate=0;
  int current_height=0;
  //int sorted_y_coords[boxes.size()];
  //int labels[boxes.size()];
  string newword="";
  for(int i=0;i<boxes.size();i++)
  {
    if (words[i].find_first_not_of(' ') != std::string::npos && confidences[i]>70.0)
    {

      //imshow("image",img);
      //imshow("Cropped ",img(boxes[i]));

      if((boxes[i].y-current_y_coordinate)>boxes[i].height/1.5)
      {
        int flag=0;
        //section of the code for eliminating false detection inside sheet music.It is based on the assumption that most
        //of the detection inside staves are either 1,i,I ,l,or L
        if (newword.length()<3)flag=1;
        int count=0;
        for(int j=0;j<newword.length();j++)
        {
          if(newword[j]!='i'&&newword[j]!='I'&&newword[j]!='l'&&newword[j]!='L'&&newword[j]!='1')
          {
            count++;
          }
        }
        if (count<3)flag=1;

        y_coordinates.push_back(current_y_coordinate);
        box_heights.push_back(current_height);
        current_y_coordinate=boxes[i].y;
        current_height=boxes[i].height;
        //cout<<i<<endl;
        if (flag==1)
        {
          //cout <<newword<<endl;
          y_coordinates.pop_back();
          box_heights.pop_back();
        }
        //circle( img, Point(boxes[i].x,boxes[i].y), 16.0, Scalar( 0, 0, 255 ), -1, 8 );
        //cout<<newword<<endl;
        newword="";
      }
      newword=newword+words[i];
      //cout<<words[i]<<" ";
      //waitKey(0);
    }
  }
  y_coordinates.push_back(current_y_coordinate);
  box_heights.push_back(current_height);
  //y_coordinates.erase (y_coordinates.begin());
  return y_coordinates;

}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "Please give the location of source sheet Music as ./test /path/to/MusicSheet" <<endl;
        return -1;
    }
    img =imread(argv[1]);
    if (img.empty())
    {
        cout << "No Image found found at the given location,please try again" <<endl;
        return -1;
    }

    Mat gray;
    namedWindow("Original Image",CV_WINDOW_NORMAL);
    imshow("Original Image",img);
    cvtColor(img, gray, CV_BGR2GRAY);
    Size size(2200/img.rows*img.cols,2200);
    resize(img,img,size);
    resize(gray,gray,size);
    //namedWindow("resized",WINDOW_NORMAL);
    //imshow("resized",gray);
    string output;

    vector<Rect>   boxes;
    vector<string> words;
    vector<float>  confidences;

    //NULL,NULL,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXY",3,3
    Ptr<OCRTesseract> ocr = OCRTesseract::create();
    
    ocr->run(gray, output, &boxes, &words, &confidences, OCR_LEVEL_WORD);

    Scalar color=Scalar(255,0,0);

    vector<int> y_coords=y_coordinates_of_various_lines_of_text(boxes,confidences,words);
    cout <<"Number of lines of text detected is "<<y_coords.size()<<endl;
    //cout <<y_coords.size()<<endl;
    for(int i=0;i<y_coords.size();i++)
    {

      rectangle(img,Point(0,y_coords[i]-box_heights[i]/4),Point(img.cols,y_coords[i]+3*box_heights[i]/2),Scalar(255,255,255),-1,8);
    }

    namedWindow("lines detected",CV_WINDOW_NORMAL);
    imshow("lines detected",img);
    imwrite("lines.jpg",img);
    waitKey(0);
    //return 0;
}