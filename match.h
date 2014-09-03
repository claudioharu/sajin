#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#define Max 100000.0;
#define MAXIMAL_OCCUPATION 25.00 // object occupies less than 25% of target image
#define MINIMAL_OCCUPATION 1.00 // object occupies at least 1% of target image
#define PERCENTAGE_VARIATION 5 // percentage incremented by 5


typedef struct BestPoint
{
	double mse;
	double angle;
	int percent;
	cv::Point point;
	cv::Point center; 
} BestPoint;



double MSE(cv::Mat* img_1, cv::Mat* img_2);
void Rotate(cv::Mat* src, double angle, cv::Mat* dst);
void Resize(cv::Mat* src, int w, int h, cv::Mat* dst);
void draw_rectangle(cv::Mat target, int h, int w, cv::Point corner, int thickness);
void Rotate_without_cropping(cv::Mat* src, double angle, cv::Mat* rot);
void Resize_image(cv::Mat* src, cv::Mat* dst, int percent);
void define_thresholds(cv::Mat* img_target, cv::Mat* img_query, int *percent_lower, int *percent_upper);

