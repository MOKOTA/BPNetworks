#include<iostream>
#include<stdlib.h>
#include<iterator>
#include<opencv2/opencv.hpp>
#include<eigen3/Eigen/Core>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
using namespace Eigen;

const char *pstrWindowsBinaryTitle = "二值图";
template<typename T1,typename T2> T1 randperm(const int &Index)
{

    T1 result = T1::Random(Index);
    T1 index(Index);
    for(int k=1;k<=Index;k++)
        index[k-1]=k;
    for(int j=0;j<Index-1;j++)
    {
        for(int i=0;i<Index-1;i++)
        {
            if(result[i+1]>result[i])
            {
                T2 temp;
                temp = result[i+1];
                int tempIndex;
                tempIndex = index[i+1];
                result[i+1] = result[i];
                index[i+1] = index[i];
                result[i] = temp;
                index[i] = tempIndex;
            }
        }
    }
    return index;
}

IplImage *TransToBinary(IplImage* img,int pos)
{
    IplImage *res=NULL;
    IplImage *gray=NULL;
    if(!img)
        return res;
    int depth = img->depth;
    int channel = 1;
    CvSize sz;
    sz.height = img->height;
    sz.width = img->width;
    gray = cvCreateImage(sz,depth,channel);
    res = cvCreateImage(sz,depth,channel);
    cvCvtColor(img,gray,CV_BGR2GRAY);
    cvThreshold(gray,res,100,255,CV_THRESH_BINARY);
    cvReleaseImage(&gray);
    return res;
}

int main(int argc,char** argv)
{
    double t = 0;
    t = (double)cvGetTickCount();
    char* filename = argv[1];
    IplImage *image;
    const char *pstrWindowsToolBarName = "二值图阈值";
    IplImage *binary;

    image = cvLoadImage(filename);


    cvNamedWindow(pstrWindowsBinaryTitle, 2);
    binary = TransToBinary(image,120);
    t = (double)cvGetTickCount()-t;
    cout<<"cost "<<t<<"ms\n";
    cvShowImage(pstrWindowsBinaryTitle,binary);
    cvWaitKey(0);
    cvDestroyAllWindows();
    cvReleaseImage(&binary);
    cvReleaseImage(&image);
    return 0;
}
