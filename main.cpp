#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"pretrement.cpp"
using namespace std;
using namespace cv;

int main(int argc,char** argv)
{
	int input_layer_size = 400;
	int hidden_layer_size = 25;
	int num_lables = 10;
	int m;
	char *filename = argv[1];
	Mat pic1 = imread(filename);
	Mat pic2;
	cout<<pic1.size();
	resize(pic1,pic2,Size(20,20),0,0,CV_INTER_LINEAR);
	VectorXf v1 = randperm<VectorXf,float>(100);
	v1 = v1.segment(0,100);



	cout<<"Loading and Visualizing Data"<<endl;
	return 0;
}
