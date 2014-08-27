#include "match.h"

using namespace cv;

double MSE(Mat* img_1, Mat* img_2)
{
	/*
	if(img_1->rows != img_2->rows) return Max;
	if(img_1->cols != img_2->cols) return Max;
	*/
	Scalar intensity1, intensity2;
	double val = 0.0;
	for(int i = 0; i < img_2->rows; i++)
	{	
		for(int j = 0; j < img_2->cols; j++)
		{
			intensity1 = img_1->at<uchar>(Point(j,i));	
			intensity2 = img_2->at<uchar>(Point(j,i));
			val += pow(double(intensity1.val[0]-intensity2.val[0]), 2.0);	
		}
	}
	
	//std::cout << val << "\n";
	//std::cout << (1.0/(img_1->rows*img_2->cols)) << "\n";
	double mse = (1.0/(img_2->rows*img_2->cols))*val;
	//std::cout << mse << "\n";
	return mse;
}

void rotate(Mat* src, double angle, Mat* dst)
{
    int len = max(src->cols, src->rows);
    Point2f pt(len/2., len/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(*src, *dst, r, Size(src->cols, src->rows));
}
