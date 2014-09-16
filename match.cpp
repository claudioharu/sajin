#include "match.h"

using namespace cv;

//Calculating the mse
double MSE(Mat* img_1, Mat* img_2)
{
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
	
	double mse = (1.0/(img_2->rows*img_2->cols))*val;

	return mse;
}

//Rotating an image
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
}

//Resizing an image
void Resize_image(Mat* src, Mat* dst, int percent)
{
	// declare a destination Mat with correct size and channels
	*dst = Mat((int)((src->rows*percent)/100), (int)((src->cols*percent)/100), CV_8UC1);

	//use cv::resize to resize source to a destination image
	resize(*src,  *dst,  dst->size(), 0, 0, CV_INTER_LINEAR);	
}

//Drawing a rectangle around of a spot
void draw_rectangle(Mat target, int h, int w, Point corner, int thickness)
{
    int i, j, t;

    for(t = 0; t < thickness; t++){

        // Calculates the rectangles vertex
        Point vertex1 = Point(corner.x-1, corner.y-1);
        Point vertex2 = Point(vertex1.x + w + 1, vertex1.y);
        Point vertex3 = Point(vertex2.x, vertex2.y + h + 1);
        Point vertex4 = Point(vertex3.x - w -1, vertex3.y);
        
        // Draw top line 
        for(j=vertex1.x; j<vertex2.x; j++){
            target.at<uchar>(vertex1.y, j) = 255;
        }

        // Bottom line
        for(j = vertex3.x; j > vertex4.x; j--){
            target.at<uchar>(vertex3.y, j) = 255;
        }

        // Right line
        for(i = vertex2.y; i < vertex3.y; i++){
            target.at<uchar>(i, vertex2.x) = 255;
        }

        // Left line
        for(i = vertex4.y; i > vertex1.y; i--){
            target.at<uchar>(i, vertex4.x) = 255;
        }

        corner.x--;
        corner.y--;
        h += 2;
        w += 2;

    }

}

// Defining thresholds based on the query's minimal and maximal area occupation of target
void define_thresholds(Mat* img_target, Mat* img_query, int *percent_lower, int *percent_upper)
{
	int tgt_area = img_target->rows*img_target->cols;
	double occupied_area = 0.00;
	int percent = 0;

	*percent_lower = 0; 
	*percent_upper = 0;
	while(occupied_area < MAXIMAL_OCCUPATION){
	
		occupied_area = (double) ( (img_query->rows*percent/100) * (img_query->cols*percent/100) * 100 )/tgt_area;
		
		if(occupied_area > MINIMAL_OCCUPATION && *percent_lower == 0) {
			*percent_lower = percent;
		}
		
		if(occupied_area > MAXIMAL_OCCUPATION) {
			*percent_upper = percent - 5;
			if(*percent_upper > 100) *percent_upper = 100;
			return;
		}
		
		percent+=5;	
	};
}
