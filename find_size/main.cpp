#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

void Rotate_without_cropping(Mat* src, double angle, Mat* rot)
{
	Mat frame, frameRotated;
	int diagonal = (int)sqrt(src->cols*src->cols+src->rows*src->rows);
	int newWidth = diagonal;
	int newHeight =diagonal;

	int offsetX = (newWidth - src->cols) / 2;
	int offsetY = (newHeight - src->rows) / 2;
	
	Mat targetMat(newWidth, newHeight, src->type());
	Point2f src_center(targetMat.cols/2.0F, targetMat.rows/2.0F);

	src->copyTo(frame);
	double radians = angle * M_PI / 180.0;
	double sin = abs(std::sin(radians));
	double cos = abs(std::cos(radians));

	frame.copyTo(targetMat.rowRange(offsetY, offsetY + frame.rows).colRange(offsetX, offsetX + frame.cols));
	Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	warpAffine(targetMat, frameRotated, rot_mat, targetMat.size());
		
 	//Calculate bounding rect and for exact image
    	Rect bound_Rect(frame.cols,frame.rows,0,0);

    	int x1 = offsetX;
    	int x2 = offsetX+frame.cols;
    	int x3 = offsetX;
    	int x4 = offsetX+frame.cols;

    	int y1 = offsetY;
    	int y2 = offsetY;
    	int y3 = offsetY+frame.rows;
    	int y4 = offsetY+frame.rows;

    	Mat co_Ordinate = (Mat_<double>(3,4) << x1, x2, x3, x4,
                                            	y1, y2, y3, y4,
                                            	1,  1,  1,  1 );
    	Mat RotCo_Ordinate = rot_mat * co_Ordinate;

    	for(int i=0;i<4;i++){
       		if(RotCo_Ordinate.at<double>(0,i)<bound_Rect.x)
         		bound_Rect.x=(int)RotCo_Ordinate.at<double>(0,i); //access smallest 
       		if(RotCo_Ordinate.at<double>(1,i)<bound_Rect.y)
        		bound_Rect.y=RotCo_Ordinate.at<double>(1,i); //access smallest y
     	}

     	for(int i=0;i<4;i++){
       		if(RotCo_Ordinate.at<double>(0,i)>bound_Rect.width)
         		bound_Rect.width=(int)RotCo_Ordinate.at<double>(0,i); //access largest x
       		if(RotCo_Ordinate.at<double>(1,i)>bound_Rect.height)
        		bound_Rect.height=RotCo_Ordinate.at<double>(1,i); //access largest y
     	}

    	bound_Rect.width=bound_Rect.width-bound_Rect.x;
    	bound_Rect.height=bound_Rect.height-bound_Rect.y;

    	Mat ROI = frameRotated(bound_Rect);
    	ROI.copyTo(*rot);

    	//imwrite("rotated.jpg", *rot);
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

	double angle = atoi(argv[3]);
	int percent = atoi(argv[4]);
	
	// ROT --> RESIZE (sem ruído)
	Mat img_rot;
	Rotate_without_cropping(&img_query, angle, &img_rot);
	
	Mat img_resized;
	Resize_image(&img_rot, &img_resized, percent);	
	
	namedWindow("Result", CV_WINDOW_AUTOSIZE );
	imshow("Result", img_resized);
	
	namedWindow("TARGET", CV_WINDOW_AUTOSIZE );
	imshow("TARGET", img_target);
	
	waitKey(0);
    
	return 0;

}


