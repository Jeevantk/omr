#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <vector>
#include <opencv2/text.hpp>

using namespace cv;
using namespace std;
using namespace tesseract;
using namespace cv::text;

Mat img;
vector <int> box_heights;
//This function calculates the number of text lines in the code.The number of detected lines includes false detection also,majorly that including 'i' or 'l' from
// This function is actually not needed anymore :P
int number_of_text_lines(vector<Rect> boxes,vector<float>  confidences,vector<string> words)
{
  int current_y_coordinate=0;
  int no_of_lines=0;
  for(int i=0;i<boxes.size();i++)
  {
    if (words[i].find_first_not_of(' ') != std::string::npos && confidences[i]>70.0)
    {
      if((boxes[i].y-current_y_coordinate)>boxes[i].height/2)
      {
        current_y_coordinate=boxes[i].y;
        no_of_lines++;
      }
    }
  }
  return no_of_lines;

}
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
    //ocr.char_whitelist("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    ocr->run(gray, output, &boxes, &words, &confidences, OCR_LEVEL_WORD);
    for(int i=0;i<boxes.size();i++)
    {
      //rectangle(img, boxes[i].tl(), boxes[i].br(),Scalar(255,0,0), 2, 8, 0 );
    }
    //imshow("simply tesseract",img);
    //waitKey(0);
    Scalar color=Scalar(255,0,0);
    for (int i=0;i<boxes.size();i++)
    {
      if (words[i].find_first_not_of(' ') != std::string::npos && confidences[i]>70.0)
      {
        //imshow("individual Boxes",gray(boxes[i]));
        rectangle(img, boxes[i].tl(), boxes[i].br(),color, 2, 8, 0 );
        //namedWindow("image",WINDOW_NORMAL);
        //imshow("image",img);
        //cout<<words[i]<<endl;
        //cout<<confidences[i]<<endl;
        //cout<<boxes[i].y<<endl;
        //waitKey(0);
      }


      //rectangle(img, boxes[i].tl(), boxes[i].br(), color, 2, 8, 0 );
    }
    //cout<<number_of_text_lines(boxes,confidences,words)<<endl;

    vector<int> y_coords=y_coordinates_of_various_lines_of_text(boxes,confidences,words);
    cout <<"Number of lines of text detected is "<<y_coords.size()<<endl;
    //cout <<y_coords.size()<<endl;
    for(int i=0;i<y_coords.size();i++)
    {
      //cout<<y_coords[i]<<endl;
      //line(img,Point(0,y_coords[i]),Point(img.cols-1,y_coords[i]),Scalar(255,0,0),4,8,0);
      rectangle(img,Point(0,y_coords[i]-box_heights[i]/4),Point(img.cols,y_coords[i]+3*box_heights[i]/2),Scalar(255,255,255),-1,8);
    }
    threshold(gray,gray, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    // TessBaseAPI ocr;
    // ocr.Init(NULL, "eng",OEM_DEFAULT);
    // ocr.SetPageSegMode(PSM_SINGLE_BLOCK);
    // ocr.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

    // char* out = ocr.GetUTF8Text();

    // cout << out << endl;
    
    // threshold(gray,gray, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    // ocr.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);
    // out = ocr.GetUTF8Text();
    //cout << out << endl;
    //cout << output <<endl;
    imwrite("output1.jpg",img);
    waitKey(0);
    namedWindow("lines detected",CV_WINDOW_NORMAL);
    imshow("lines detected",img);
    imwrite("lines.jpg",img);
    waitKey(0);
    return 0;
}
