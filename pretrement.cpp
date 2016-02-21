#include<iostream>
#include<stdlib.h>
#include<eigen3/Eigen/Core>
using namespace std;
using namespace Eigen;

VectorXf randperm(const int &Index)
{

    VectorXf result = VectorXf::Random(Index);
    VectorXf index(Index);
    for(int k=1;k<=Index;k++)
        index[k-1]=k;
    for(int j=0;j<Index-1;j++)
    {
        for(int i=0;i<Index-1;i++)
        {
            if(result[i+1]>result[i])
            {
                float temp;
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
int main()
{
    VectorXf v =randperm(10);
    cout<<v<<endl;
    return 0;
}
