#include<iostream>
#include<opencv2\opencv.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\video\tracking.hpp>
#include<opencv2\video\background_segm.hpp>
#include<opencv2\video\video.hpp>
#include"cvaux.h"

using namespace std;
using namespace cv;

bool help(int argc,char **argv)
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
CvCapture *ReadVideo(char *filename)
{
	CvCapture *capture = 0;
	capture = cvCaptureFromAVI(filename);
	if (!capture)
		return 0;
	else
		return capture;
}
void setCom(const Mat&img, const Mat &mask, Mat &dst)
{
	int niters = 2;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat temp;
	//cvThreshold(mask, mask, 10, 255, CV_THRESH_BINARY);
	dilate(mask, temp, Mat(), Point(-1, -1), niters);
	erode(temp, temp,Mat(), Point(-1, -1), niters * 2);
	dilate(temp, temp, Mat(), Point(-1, -1), niters);

	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	dst = Mat::zeros(img.size(), CV_8UC3);
	if (contours.size() == 0)return;
	int index = 0,largestCon = 0;
	double LargestArea = 0;
	for (; index >= 0; index = hierarchy[index][0])
	{
		const vector<Point>& a = contours[index];
		double area = fabs(contourArea(Mat(a)));
		if (area > LargestArea)
		{
			LargestArea = area;
			largestCon = index;
		}
	}
	Scalar color(0, 255, 0);
	drawContours(img, contours, largestCon, color, CV_FILLED, 8, hierarchy);
}

int main(int argc, char **argv)
{
	//if (!help(argc, argv))
	//return 0;

	CvCapture *capture = 0;
	//cvNamedWindow("image", 1);
	capture = cvCaptureFromAVI("CAM054_20150107072255.avi");
	//VideoCapture video;
	if (!capture)return 0;
	Mat frame;//, mask, thresholdImage, output;
	//while ((frame.empty()))
	//{
		//if (!cvGrabFrame(capture))break;
		//frame = cvRetrieveFrame(capture);
		
	//	if ((frame.empty()))
	//}
	//IplImage *test,*frg;
	//IplImage Iframe = IplImage(frame);
	//test = cvCreateImage(cvSize(Iframe.width, Iframe.height), IPL_DEPTH_8U, 1);
	CvGaussBGModel *gmm = NULL;
	/*int C, M, sd_init, m;
	int *fg, *bg_bw, *rank_ind, min_index = 0,rand_temp=0,rank_ind_temp=0,x=0,y=0,frameCount=0;
	double alph, D, thresh, p, temp;
	double *w, *mean, *sd, *u_diff, *rank;
	CvRNG state;
	int i, j, k, match, width, height;;
	C = 4;                      //number of gaussian components (typically 3-5)  
	M = 4;                      //number of background components  
	sd_init = 6;                //initial standard deviation (for new components) var = 36 in paper  
	alph = 0.01;                //learning rate (between 0 and 1) (from paper 0.01)  
	D = 2.5;                    //positive deviation threshold  
	thresh = 0.25;              //foreground threshold (0.25 or 0.75 in paper)  
	p = alph / (1 / C);      //initial p variable (used to update mean and sd)  
	width = Iframe.widthStep;
	height = Iframe.height;
	fg = (int *)malloc(sizeof(int)*height*width);
	bg_bw = (int *)malloc(sizeof(int)*height*width);
	rank = (double *)malloc(sizeof(double) * 1 * C);
	w = (double *)malloc(sizeof(double)*height*width*C);            //weights array  
	mean = (double *)malloc(sizeof(double)*height*width*C);         //pixel means  
	sd = (double *)malloc(sizeof(double)*height*width*C);           //pixel standard deviations  
	u_diff = (double *)malloc(sizeof(double)*height*width*C);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			for (k = 0; k < C; k++)
			{
				mean[i*width*C + j*C + k] = cvRandReal(&state) * 255;
				w[i*width*C + j*C + k] = (double)1 / C;
				sd[i*width*C + j*C + k] = sd_init;
			}
		}
	}
	if (!cvGrabFrame(capture))cout << "akfhw" << endl;
	while (1){
		rank_ind = (int *)malloc(sizeof(int)*C);
		Mat mframe;
		//mframe.convertTo(mframe, CV_32FC3);
		//frame.convertTo(frame, CV_32F);
		//cout << frame.channels() << endl;
		//cout << mframe.channels() << endl;
		//cvtColor(mframe, frame, CV_BGR2GRAY);
		// calculate difference of pixel values from mean
		mframe = frame.clone();
		cvtColor(mframe, frame, CV_BGR2GRAY);
		cout << mframe.channels() << endl;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				for (m = 0; m < C; m++)
				{
					u_diff[i*width*C + j*C + m] = abs((uchar)Iframe.imageData[i*width + j] - mean[i*width*C + j*C + m]);
				}
			}
		}
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				match = 0;
				temp = 0;
				for (k = 0; k < C; k++)
				{
					if (abs(u_diff[i*width*C + j*C + k]) <= D*sd[i*width*C + j*C + k])      //pixel matches component  
					{
						match = 1;                                                 // variable to signal component match  

						//update weights, mean, sd, p  
						w[i*width*C + j*C + k] = (1 - alph)*w[i*width*C + j*C + k] + alph;
						p = alph / w[i*width*C + j*C + k];
						mean[i*width*C + j*C + k] = (1 - p)*mean[i*width*C + j*C + k] + p*(uchar)Iframe.imageData[i*width + j];
						sd[i*width*C + j*C + k] = sqrt((1 - p)*(sd[i*width*C + j*C + k] * sd[i*width*C + j*C + k]) + p*(pow((uchar)Iframe.imageData[i*width + j] - mean[i*width*C + j*C + k], 2)));
					}
					else{
						w[i*width*C + j*C + k] = (1 - alph)*w[i*width*C + j*C + k];           // weight slighly decreases  
					}
					temp += w[i*width*C + j*C + k];
				}

				for (k = 0; k < C; k++)
				{
					w[i*width*C + j*C + k] = w[i*width*C + j*C + k] / temp;
				}

				temp = w[i*width*C + j*C];
				bg_bw[i*width + j] = 0;
				for (k = 0; k < C; k++)
				{
					bg_bw[i*width + j] = bg_bw[i*width + j] + mean[i*width*C + j*C + k] * w[i*width*C + j*C + k];
					if (w[i*width*C + j*C + k] <= temp)
					{
						min_index = k;
						temp = w[i*width*C + j*C + k];
					}
					rank_ind[k] = k;
				}

				test->imageData[i*width + j] = (uchar)bg_bw[i*width + j];
				if (match == 0)
				{
					mean[i*width*C + j*C + min_index] = (uchar)Iframe.imageData[i*width + j];
					//printf("%d ",(uchar)bg->imageData[i*width+j]);  
					sd[i*width*C + j*C + min_index] = sd_init;
				}
				for (k = 0; k < C; k++)
				{
					rank[k] = w[i*width*C + j*C + k] / sd[i*width*C + j*C + k];
					//printf("%f ",w[i*width*C+j*C+k]);  
				}

				//sort rank values  
				for (k = 1; k < C; k++)
				{
					for (m = 0; m<k; m++)
					{
						if (rank[k] > rank[m])
						{
							//swap max values  
							rand_temp = rank[m];
							rank[m] = rank[k];
							rank[k] = rand_temp;

							//swap max index values  
							rank_ind_temp = rank_ind[m];
							rank_ind[m] = rank_ind[k];
							rank_ind[k] = rank_ind_temp;
						}
					}
				}

				//calculate foreground  
				match = 0; k = 0;
				//frg->imageData[i*width+j]=0;  
				while ((match == 0) && (k < M)){
					if (w[i*width*C + j*C + rank_ind[k]] >= thresh)
						if (abs(u_diff[i*width*C + j*C + rank_ind[k]]) <= D*sd[i*width*C + j*C + rank_ind[k]]){
							frg->imageData[i*width + j] = 0;
							match = 1;
						}
						else
							frg->imageData[i*width + j] = (uchar)Iframe.imageData[i*width + j];
					k = k + 1;
				}
			}
		}

		mframe = cvQueryFrame(capture);
		cvShowImage("fore", frg);
		cvShowImage("back", test);
		char s = cvWaitKey(33);
		if (s == 27) break;
		free(rank_ind);
	}
	*/

	//gmm = (CvGaussBGModel*)cvCreateGaussianBGModel(pFrame, 0);
	//gmm.nmixtures = 3;
	//gmm.bShadowDetection = false;
	int framecount = 0;
	bool test = true;
	while (1)
	{
		if (!cvGrabFrame(capture))break;
		frame = cvRetrieveFrame(capture);
		if ((frame.empty()))break;
		framecount++;
		if (framecount >= 20)test = false;
		//gmm.operator()(frame, mask, test? 0.0001:0);
	
		gmm.getBackgroundImage(output);
		//output = cvCreateMat(mask.width, mask.height, CV_8UC1);
		//mask.convertTo(output, CV_8UC1);
		//if (mask.empty())continue;
		setCom(frame, mask, thresholdImage);
		
		
		if (!(output.empty()))imshow("image", output);
		char c=waitKey(1);
		if (c == 27)
			break;;
	}
	free(fg); free(w); free(mean); free(sd); free(u_diff); free(rank);
	cvNamedWindow("back", 0);
	cvNamedWindow("fore", 0);
	cvReleaseCapture(&capture);
	cvDestroyWindow("fore");
	cvDestroyWindow("back");
	return 0;
	/*cvReleaseCapture(&capture);
	cvDestroyAllWindows();
	waitKey(0);
	return 0;*/
}