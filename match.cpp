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

void Rotate(Mat* src, double angle, Mat* dst)
{
    int len = max(src->cols, src->rows);
    Point2f pt(len/2., len/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(*src, *dst, r, Size(src->cols, src->rows));
}

//resize image
void Resize(Mat* src, int w, int h, Mat* dst)
{
	resize(*src, *dst, Size(w,h));
}

//resize image
void Resize_image(Mat* src, Mat* dst, int percent)
{
	// declare a destination Mat with correct size and channels
	*dst = Mat((int)((src->rows*percent)/100), (int)((src->cols*percent)/100), CV_8UC1);

	//use cv::resize to resize source to a destination image
	resize(*src,  *dst,  dst->size(), 0, 0, CV_INTER_LINEAR);	
}

// draw rectangle
void draw_rectangle(Mat target, int h, int w, Point corner, int thickness)
{
    int i, j, t;

    for(t = 0; t < thickness; t++){
    
    	//std::cout << "Entrei draw\n";

        // Calculates the rectangles vertex
        Point vertex1 = Point(corner.x-1, corner.y-1);
        Point vertex2 = Point(vertex1.x + w + 1, vertex1.y);
        Point vertex3 = Point(vertex2.x, vertex2.y + h + 1);
        Point vertex4 = Point(vertex3.x - w -1, vertex3.y);
        
        //std::cout << "Vertices\n";

        // Draw top line line
        for(j=vertex1.x; j<vertex2.x; j++){
            target.at<uchar>(vertex1.y, j) = 255;
        }
        //std::cout << "Top\n";

        // Bottom line
        for(j = vertex3.x; j > vertex4.x; j--){
            target.at<uchar>(vertex3.y, j) = 255;
        }
        //std::cout << "Bottom\n";

        // Right line
        for(i = vertex2.y; i < vertex3.y; i++){
            target.at<uchar>(i, vertex2.x) = 255;
        }
        //std::cout << "Right\n";

        // Left line
        for(i = vertex4.y; i > vertex1.y; i--){
            target.at<uchar>(i, vertex4.x) = 255;
        }
        //std::cout << "Left\n";

        corner.x--;
        corner.y--;
        h += 2;
        w += 2;

    }

}

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
	
	//std::cout << "\n\nLOW = " << *percent_lower << " " << "\nUPPER = " << *percent_upper << "\n\n";
}
