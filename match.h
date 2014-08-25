#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#define Max 100000.0;


typedef struct BestPoint
{
	double mse;
	double angle;
	cv::Point point;
	
} BestPoint;



double MSE(cv::Mat* img_1, cv::Mat* img_2);
void rotate(cv::Mat* src, double angle, cv::Mat* dst);


