#include<iostream>
//#include"highgui.h"
//#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
//#include<opencv2\video\tracking.hpp>
//#include<opencv2\video\background_segm.hpp>
//#include<opencv2\video\video.hpp>
#include"cvaux.h"

CvScalar fcWhite = cvRealScalar(255);
CvScalar fcBlack = cvRealScalar(0);
using namespace std;
using namespace cv;

bool help(int argc, char **argv)
{
	if (argv[1])
	{
		cout << "This programm is used to detected cars and trace them" << endl;
		cout << "The test video is " << argv[0] << '\t' << "right?" << endl;
		cout << "we are going to show the cars in the video " << endl;
		return true;
	}
	else
	{
		cout << "pls enter the filename!" << endl;
		return false;
	}
}
void setCom(const Mat&img,Mat dst)
{
	int niters = 2;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat temp;
	
	//IplConvKernel element;
	//cvThreshold(mask, mask, 10, 255, CV_THRESH_BINARY);
	//dilate(mask, temp, Mat(), Point(-1, -1), niters);
	erode(temp, img,  Mat());
	//dilate(temp, temp, Mat(), Point(-1, -1), niters);

	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	dst = Mat::zeros(img.size(), CV_8UC3);
	if (contours.size() == 0)return;
//	int index = 0, largestCon = 0;
	//double LargestArea = 0;
	/*for (; index >= 0; index = hierarchy[index][0])
	{
		const vector<Point>& a = contours[index];
		double area = fabs(contourArea(Mat(a)));
		if (area > LargestArea)
		{
			LargestArea = area;
			largestCon = index;
		}
	}*/
	Scalar color(0, 255, 0);
	int index = 0;
	for (; index >= 0; index = hierarchy[index][0])
	{
		drawContours(dst, contours, index, color, CV_FILLED, 8, hierarchy);
	}
}
int main(int argc,char **argv)
{
	//if(!help(argc, argv)) return 0;
	CvFont fWarnning;
	IplImage *pFrameOri = NULL;
	//IplImage *pFrame = NULL;
	IplImage *pFrImg = NULL;
	IplImage *pBkImg = NULL;
	//IplImage *dst = NULL;
	CvMat* pFrameMat = NULL;
	CvMat* pFrMat = NULL;
	CvMat* pBkMat = NULL;

	//CvCapture *pCapture = NULL;

	int nFrmNum = 0;// 桢记数器
	cvInitFont(&fWarnning, CV_FONT_HERSHEY_SIMPLEX, 2, 2, 0, 6, 8);

	
	Mat mat1;

	mat1 = imread("result.png",0);
	cout << mat1.rows;

	int nr = mat1.rows;
	int nc = mat1.cols;
	Mat mat2(nr, nc, CV_8UC1);
	if (mat1.isContinuous())cout << "yes" << endl;
	else
		return 0;
	for (int j = 0; j<nr; j++)
	{
		for (int i = 0; i<nc; i++)
		{
			//cout << mat1.at<uchar>(j, i);
			uchar a = mat2.at<uchar>(j, nc - i);
			mat2.at<uchar>(j, i) = a;
		}
	}
	imshow("1", mat1);
	imshow("2",  mat2);
	waitKey(0);
	return 0;
	// 创建窗口
	cvNamedWindow("video", 1);
	cvNamedWindow("background", 1);
	//cvNamedWindow("foreground", 1);
	//cvNamedWindow("Trace", 1);
	// 排列窗口
	cvMoveWindow("video", 15, 80);
	cvMoveWindow("background", 345, 80);
	//cvMoveWindow("foreground", 675, 80);
	//cvMoveWindow("Trace", 345, 200);
	CvCapture *capture = 0;
	//cvNamedWindow("image", 1);
	char *filename;
	filename = "video.avi";
	capture = cvCaptureFromAVI(filename);
	//capture = cvCaptureFromAVI(argv[1]);
	//VideoCapture video;
	if (!capture)
	{
		cout << "empty video file!" << endl;
		return 0;
	}
	//if (!cvGrabFrame(capture))cout << "akfhw" << endl;
	while (1) {
		do
		{
			pFrameOri = cvQueryFrame(capture);
		} while (!pFrameOri);
		
		nFrmNum++;
		
		// 缩放图象
		//pFrame = cvCreateImage(cvSize(360, 288), IPL_DEPTH_8U, 3);
		//pFrame->origin = 0;// 反转图象存储
		cvResize(pFrameOri, pFrameOri, CV_INTER_LINEAR);
		//dst = cvCreateImage(cvGetSize(pFrameOri), 8, 3);
		// 如果是第一帧，申请内存并初始化
		if (nFrmNum == 1) {

			pBkImg = cvCreateImage(cvSize(pFrameOri->width, pFrameOri->height), IPL_DEPTH_8U, 1);
			pFrImg = cvCreateImage(cvSize(pFrameOri->width, pFrameOri->height), IPL_DEPTH_8U, 1);

			// 上下反转图象存储
			pBkImg->origin = 0;
			pFrImg->origin = 0;

			pBkMat = cvCreateMat(pFrameOri->height, pFrameOri->width, CV_32FC1);
			pFrMat = cvCreateMat(pFrameOri->height, pFrameOri->width, CV_32FC1);
			pFrameMat = cvCreateMat(pFrameOri->height, pFrameOri->width, CV_32FC1);

			// 原始图象转化成单通道图像再处理
			cvCvtColor(pFrameOri, pBkImg, CV_BGR2GRAY);
			cvCvtColor(pFrameOri, pFrImg, CV_BGR2GRAY);

			// 图象转换为象素矩阵
			cvConvert(pFrImg, pFrameMat);
			cvConvert(pFrImg, pFrMat);
			cvConvert(pFrImg, pBkMat);

		}
		else {

			// 单通道转换
			cvCvtColor(pFrameOri, pFrImg, CV_BGR2GRAY);
			cvConvert(pFrImg, pFrameMat);

			// 高斯滤波，以平滑图像
			//cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

			if (nFrmNum >= 400) {

				// 当前帧跟背景图相减
				cvAbsDiff(pFrameMat, pBkMat, pFrMat);

				// 二值化前景图
				cvThreshold(pFrMat, pFrImg, 40, 255.0, CV_THRESH_BINARY);
				//findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
				// 进行形态学滤波，闭运算，突出车体
				//cvDilate(pFrImg, pFrImg, 0, 2);// 膨胀
				//cvErode(pFrImg, pFrImg, 0, 2);// 腐蚀
				CvSeq *contours=NULL;
				//vector<Vec4i> hierarchy;
				//Mat temp;
				CvMemStorage *storage = cvCreateMemStorage(0);
				cvFindContours(pFrImg, storage, &contours, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
				cvDilate(pFrImg, pFrImg, 0, 2);// 膨胀
				cvErode(pFrImg, pFrImg, 0, 1);// 腐蚀
				int cnt = 0;
				double area = 0;
				//IplImage *dst = cvCreateImage(cvGetSize(pFrImg), 8, 3);
				//cvShowImage("ee", dst);
				cvFindContours(pFrImg, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
				for (; contours != 0; contours = contours->h_next)
				{
					cnt++;
					area = fabs(cvContourArea(contours, CV_WHOLE_SEQ)); //获取当前轮廓面积像素
					if (area>100 && area<1000000)                              // 设10k<area<100k
					{
						CvScalar color = CV_RGB(rand() & 255, rand() & 255, rand() & 255);
						cvDrawContours(pFrImg, contours, color, color, 0, 16, CV_FILLED, cvPoint(0, 0));
						CvRect rect = cvBoundingRect(contours, 0);
						cvRectangle(pFrameOri, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), CV_RGB(255, 0, 0), 1, 8, 0);
						CvBox2D a = cvMinAreaRect2(contours, 0);
						///CvPoint2D32f point = a.center;
						//CvPoint point1;
						//point1.x = point.x;
						//point1.y = point.y;
						//cvCircle(dst, point1, 2, CV_RGB(255, 0, 0), 8);
						//cvShowImage("dg", dst);
					}
				}
				cvReleaseMemStorage(&storage);
				//imshow("dst", dst);
			}
			else if (nFrmNum % 30 <= 20)
			{
				cvPutText(pFrameOri, "PLEASE WAIT WHILE EXTRACTING ...", cvPoint(pFrameOri->width/24, pFrameOri->height / 2), &fWarnning, fcWhite);
				//cvPutText(dst, "PLEASE WAIT WHILE EXTRACTING ...", cvPoint(40, dst->height / 2), &fWarnning, fcWhite);
			}

			// 更新背景
			cvRunningAvg(pFrameMat, pBkMat, 0.004, 0);
			// 将背景转化为图像格式，用以显示
			cvConvert(pBkMat, pBkImg);

			// 显示图像
			cvShowImage("video", pFrameOri);
			cvShowImage("background", pBkImg);
			//cvReleaseImage(&pFrImg);
			//cvReleaseImage(&pBkImg);
			//	cvReleaseImage(&dst);
			//cvReleaseMat(&pFrameMat);
			//cvReleaseMat(&pFrMat);
			//cvReleaseMat(&pBkMat);
			//cvShowImage("foreground", pFrImg);
			//cvShowImage("Trace", dst);
			// 如果有按键事件，则跳出循环
			// 此等待也为cvShowImage函数提供时间完成显示
			// 等待时间可以根据CPU速度调整
			if (cvWaitKey(2) >= 0)
				break;

		}

		//cvReleaseImage(&pFrame);
		//cvReleaseImage(&dst);
	}


	// 销毁窗口
	cvDestroyAllWindows();

	// 释放图像和矩阵
//	cvReleaseImage(&pFrame);
	cvReleaseImage(&pFrImg);
	cvReleaseImage(&pBkImg);
//	cvReleaseImage(&dst);
	cvReleaseMat(&pFrameMat);
	cvReleaseMat(&pFrMat);
	cvReleaseMat(&pBkMat);
	
	cvReleaseCapture(&capture);

	return 0;

}