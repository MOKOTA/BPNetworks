#include<iostream>
#include<stdlib.h>
#include<math.h>

using namespace std;

class Bpnode	//神经网络节点
{
	private:
		double parm;
		int num;
	public:
		Bpnode();

};


class Bplayer:public Bpnode	//神经网络层
{

};


class Bpnet:public Bplayer	//神经网络
{

};
