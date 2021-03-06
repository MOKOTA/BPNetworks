#include<opencv\cv.h>
#include<stdlib.h>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;
IplImage* subnoise(IplImage* img)
{
	/*
	IplImage* binary = cvCreateImage(cvSize(img->width, img->height), img->depth, 1);
	cvThreshold(img, binary, 100, 255, CV_THRESH_BINARY);
	CvMemStorage* pcvMStorage = cvCreateMemStorage();
	CvSeq* pcvSeq = NULL;
	cvFindContours(binary, pcvMStorage, &pcvSeq, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));*/
	IplImage *pOutlineImage = cvCreateImage(cvSize(img->width,img->height), img->depth, 1);
	//int nLevels = 5;
	//cvRectangle(pOutlineImage, cvPoint(0, 0), cvPoint(pOutlineImage->width, pOutlineImage->height),
	//	CV_RGB(255, 255, 255), CV_FILLED);
	//cvDrawContours(pOutlineImage, pcvSeq, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), nLevels, 2);
	//cvReleaseImage(&result);
	//cvReleaseImage(&binary);
	cvSmooth(img, pOutlineImage, CV_MEDIAN, 0.5, 0.5);
	return pOutlineImage;


}
int main(int argc,char**argv)
{
	char* filename;
	if (argc <= 1)
		filename = "F:\\c++\\ConsoleApplication3\\Video.avi";
	else
		filename = argv[1];
    CvCapture* capture = cvCreateFileCapture(filename);
	IplImage *mframe,*current,*frg,*test;
	int *fg,*bg_bw,*rank_ind;
	double *w,*mean,*sd,*u_diff,*rank;
	int C,M,sd_init,i,j,k,m,rand_temp=0,rank_ind_temp=0,min_index=0,x=0,y=0,counter_frame=0;
	double D,alph,thresh,p,temp;
	CvRNG state;
	int match,height,width;
	mframe=cvQueryFrame(capture);
	frg = cvCreateImage(cvSize(mframe->width,mframe->height),IPL_DEPTH_8U,1);
	current = cvCreateImage(cvSize(mframe->width,mframe->height),IPL_DEPTH_8U,1);
	test = cvCreateImage(cvSize(mframe->width,mframe->height),IPL_DEPTH_8U,1);

	C=4;
	M=4;
	sd_init = 6;
	alph = 0.01;
	D=2.5;
	thresh = 0.25;
	p=alph/(1/C);
	height = current->height;width = current->width;
	fg = (int*)malloc(sizeof(int)*width*height);
	bg_bw = (int*)malloc(sizeof(int)*width*height);
	rank = (double*)malloc(sizeof(double)*1*C);
	w = (double*)malloc(sizeof(double)*width*height*C);
	mean = (double*)malloc(sizeof(double)*width*height*C);
	sd = (double*)malloc(sizeof(double)*width*height*C);
	u_diff = (double*)malloc(sizeof(double)*width*height*C);
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			for(k=0;k<C;k++)
			{
			mean[i*width*C+j*C+k] = cvRandReal(&state)*255;
			w[i*width*C+j*C+k]=(double)1/C;
			sd[i*width*C+j*C+k]  = sd_init;
			}
		}
	}
	while(1)
	{
		rank_ind = (int*)malloc(sizeof(int)*C);
		cvCvtColor(mframe,current,CV_BGR2GRAY);
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				for(m=0;m<C;m++)
				{
					u_diff[i*width*C+j*C+m] = abs((uchar)current->imageData[i*width+j]-mean[i*width*C+j*C+m]);

				}
			}
		}
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
			{
				match =0;
				temp = 0;
				for(k=0;k<C;k++)
				{
					if(abs(u_diff[i*width*C+j*C+k])<=D*sd[i*width*C+j*C+k])
					{
						match =1;
						w[i*width*C+j*C+k]=(1-alph)*w[i*width*C+j*C+k]+alph;

						p = alph/w[i*width*C+j*C+k];
						mean[i*width*C+j*C+k] = (1-p)*mean[i*width*C+j*C+k] + p*(uchar)current->imageData[i*width+j];
                         sd[i*width*C+j*C+k] =sqrt((1-p)*(sd[i*width*C+j*C+k]*sd[i*width*C+j*C+k]) + p*(pow((uchar)current->imageData[i*width+j] - mean[i*width*C+j*C+k],2)));

					}
					else
					{
					    w[i*width*C+j*C+k] = (1-alph)*w[i*width*C+j*C+k];           // weight slighly decreases
					}

                    temp += w[i*width*C+j*C+k];  				}
                     for(k=0;k<C;k++)
                {
                    w[i*width*C+j*C+k] = w[i*width*C+j*C+k]/temp;
                }

                temp = w[i*width*C+j*C];
                bg_bw[i*width+j] = 0;
                for (k=0;k<C;k++)
                {
                    bg_bw[i*width+j] = bg_bw[i*width+j] + mean[i*width*C+j*C+k]*w[i*width*C+j*C+k];
                    if (w[i*width*C+j*C+k]<=temp)
                    {
                        min_index = k;
                        temp = w[i*width*C+j*C+k];
                    }
                    rank_ind[k] = k;
                }

                test->imageData[i*width+j] = (uchar)bg_bw[i*width+j];

                //if no components match, create new component
                if (match == 0)
                {
                    mean[i*width*C+j*C+min_index] = (uchar)current->imageData[i*width+j];
                    //printf("%d ",(uchar)bg->imageData[i*width+j]);
                    sd[i*width*C+j*C+min_index] = sd_init;
                }
                for (k=0;k<C;k++)
                {
                    rank[k] = w[i*width*C+j*C+k]/sd[i*width*C+j*C+k];
                    //printf("%f ",w[i*width*C+j*C+k]);
                }

                //sort rank values
                for (k=1;k<C;k++)
                {
                    for (m=0;m<k;m++)
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
                match = 0;k = 0;
                //frg->imageData[i*width+j]=0;
                while ((match == 0)&&(k<M)){
                    if (w[i*width*C+j*C+rank_ind[k]] >= thresh)
                        if (abs(u_diff[i*width*C+j*C+rank_ind[k]]) <= D*sd[i*width*C+j*C+rank_ind[k]]){
                            frg->imageData[i*width+j] = 0;
                            match = 1;
                        }
                        else
                            frg->imageData[i*width+j] = (uchar)current->imageData[i*width+j];
                    k = k+1;
                }
            }
        }

        mframe = cvQueryFrame(capture);
		IplImage* car = subnoise(frg);
        cvShowImage("fore",frg);
        cvShowImage("back",test);
		cvShowImage("car", car);
        char s=cvWaitKey(1);
		cvReleaseImage(&car);
        if(s==27) break;
        free(rank_ind);
    }

	free(fg); free(w); free(mean); free(sd); free(u_diff); free(rank); 
	
	
    cvNamedWindow("back",0);
    cvNamedWindow("fore",0);
	cvReleaseImage(&frg);
	cvReleaseImage(&test);

    cvReleaseCapture(&capture);
    cvDestroyWindow("fore");
    cvDestroyWindow("back");
    return 0;

}
