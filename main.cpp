#include "match.h"

/*
 * Inputs: debug's flag   [1]
 * 		   query_path 	  [2]
 * 		   target_path	  [3]
 */		

int main( int argc, char** argv )
{
	//Debug's flag
	short debug = atoi(argv[1]);
	BestPoint p;
	
	//Inputs
	cv::Mat img_query, img_target;
	double mse;
		
	p.mse = Max;
	
	char* query_path  = argv[2];
	char* target_path = argv[3];
	
	
	//Load imgs
	img_query = cv::imread(query_path,0);
	img_target = cv::imread(target_path,0);
	cv::Mat img_of_interest, img_rot, img_resized;
	
	// Define size variation
	int percent_lower, percent_upper;
	define_thresholds(&img_target, &img_query, &percent_lower, &percent_upper);
	if(debug == 1) std::cout << "Size varies from " << percent_lower << "% to " << percent_upper << "%\n";
	/*
	Resize_image(&img_query, &img_resized, percent_lower);
	cv::namedWindow( "LOW", cv::WINDOW_AUTOSIZE);
	cv::imshow( "LOW", img_resized );                   
	
	Resize_image(&img_query, &img_resized, percent_upper);
	cv::namedWindow( "UPPER", cv::WINDOW_AUTOSIZE);
	cv::imshow( "UPPER", img_resized );                  

	cv::namedWindow( "Target", cv::WINDOW_AUTOSIZE);
	cv::imshow( "Target", img_target );  */	
	
	/*
	cv::namedWindow( "Img Query", cv::WINDOW_NORMAL );// Create a window for display.
	cv::imshow( "Img Query", img_query );                   // Show our image inside it.

	cv::namedWindow( "Img Target", cv::WINDOW_NORMAL );// Create a window for display.
	cv::imshow( "Img Target", img_target );            // Show our image inside it.*/
	
	
	int h, w, i, j;
	int percent;	
	double angl;						
	for(angl = 0.0; angl < 360.0; angl += 30.0)
	{
		//angl = 30.0;
		Rotate_without_cropping(&img_query, angl, &img_rot);
		//cv::imshow( "ROT", img_rot);
				
		if(debug == 1)std::cout << "\n\nrodei\n";
				
		for(percent = percent_lower; percent <= percent_upper; percent+=PERCENTAGE_VARIATION)
		{
			//percent = 20;
			Resize_image(&img_rot, &img_resized, percent);
			//cv::imshow( "RES", img_resized);
					
			if(debug == 1) std::cout << "\n\nredimensionei " << percent << "%\n";
			
			for(i = 0; i < img_target.rows; i ++)
			{
				for(j = 0; j < img_target.cols; j++)
				{
					//w = img_target.cols - j;
					if(j + img_resized.cols > img_target.cols){if(debug == 1) std::cout << "Cols Out of tgt\n"; break;}
					else w = img_resized.cols;
					///h = img_target.rows - i;
					if(i + img_resized.rows > img_target.rows){if(debug == 1) std::cout << "Rows Out of tgt\n"; break;}
					else h = img_resized.rows;	
					
					img_of_interest = img_target(cv::Rect(j, i, w, h));
					mse = MSE(&img_resized, &img_of_interest);
				
					if(p.mse > mse)
					{
						p.point.x = j;
						p.point.y = i; 
						p.center.x = j + img_resized.cols/2; 
						p.center.y = i + img_resized.rows/2; 
						p.percent = percent;
						p.angle = angl;
						p.mse = mse;
						if(debug == 1) std::cout << "BestPoint: " << p.point << "\n";
						if(debug == 1) std::cout << "Center: " << p.center << "\n";
						if(debug == 1) std::cout << "MSE: " << p.mse << "\n";
						if(debug == 1) std::cout << "Angle: " << p.angle << "\n";
						if(debug == 1) std::cout << "Percent: " << percent << "\n";	
					}
					
					if(debug == 1){
						std::cout << "\nMSE: " << mse << "\n";
						std::cout << "[i,j] = " << i << "," << j << std::endl;
						std::cout << percent << "%" << "," << angl << std::endl;
					}		
				}
			}
		}
	}
	
	
	FILE *pFile;
	pFile = fopen("myfile.txt", "w");
	if(debug == 1)	std::cout << "\n\n\nMSE: " << p.mse << " point: " << p.point << " center: " << p.center;
	fprintf(pFile,"MSE: %f\nX: %d, Y: %d\ncenter X: %d, Y: %d\npercent: %d\n", p.mse, p.point.x, p.point.y, p.center.x, p.center.y, p.percent);
	fclose(pFile);
	
	// Draws rectangle on target image	
    	draw_rectangle(img_target, img_resized.rows, img_resized.cols, cv::Point(p.point.x,p.point.y), 5);
    	imwrite( "Result.png", img_target);
    	if(debug ==1) std::cout << "Draw\n"; 
    	cv::namedWindow( "Result", CV_WINDOW_NORMAL);
    	cv::imshow("Result", img_target);
    	cv::waitKey(0);
	
	cv::waitKey(0);
	
	return 0;
		
}

