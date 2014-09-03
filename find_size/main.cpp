#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

void Rotate(Mat* src, double angle, Mat* dst)
{
    int len = max(src->cols, src->rows);
    Point2f pt(len/2., len/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(*src, *dst, r, Size(src->cols, src->rows));
}

void Resize_image(Mat* src, Mat* dst, int percent)
{
	// declare a destination Mat with correct size and channels
	*dst = Mat((int)((src->rows*percent)/100), (int)((src->cols*percent)/100), CV_8UC1);

	//use cvResize to resize source to a destination image
	resize(*src,  *dst,  dst->size(), 0, 0, CV_INTER_LINEAR);	
}


// ./find_size image_obj.png img_target angl percent
int main( int argc, char** argv )
{  
	// Create an image 
	Mat img_query = imread(argv[1],0);
	Mat img_target = imread(argv[2],0);

	// Here we retrieve a percentage value to a integer
	int percent = atoi(argv[4]);
	
	// Here we retrieve a percentage value to a integer
	double angle = atoi(argv[3]);
	
	// declare a destination Mat with correct size and channels
	Mat img_resized;
	
	Resize_image(&img_query, &img_resized, percent);
	
	Mat img_rot;
	Rotate(&img_resized, angle, &img_rot);
	
	
	namedWindow("Result", CV_WINDOW_AUTOSIZE );
	imshow("Result", img_rot);
	
	namedWindow("TARGET", CV_WINDOW_AUTOSIZE );
	imshow("TARGET", img_target);
	
	waitKey(0);
    
	return 0;

}


