#include "match.h"

//char* query_path = "/home/junior/Desktop/imagens/query/minhaquery1.png";		
//char* query_path = "/home/junior/Desktop/imagens/query/009_coca_obj.png";
//char* target_path = "/home/junior/Desktop/imagens/target/sem_ruido/009_coca_tgt.png";


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
	cv::Mat img_of_interest, img_rot;
	
	/*
	cv::namedWindow( "Img Query", cv::WINDOW_NORMAL );// Create a window for display.
	cv::imshow( "Img Query", img_query );                   // Show our image inside it.

	cv::namedWindow( "Img Target", cv::WINDOW_NORMAL );// Create a window for display.
	cv::imshow( "Img Target", img_target );                   // Show our image inside it.
	*/
	
	int k = 0; 
	int h, w;
	for(double angl = 0.0; angl < 360.0; angl ++)
	{
		rotate(&img_query, angl, &img_rot);

		for(int i  = 0; i < img_target.rows; i ++)
		{
			for(int j = 0; j < img_target.cols; j++)
			{
						
				if(j + img_rot.cols > img_target.cols) break;//w = img_target.cols - j;
				else w = img_rot.cols;
				if(i + img_rot.rows > img_target.rows) break;//h = img_target.rows - i;
				else h = img_rot.rows;
				
				img_of_interest = img_target(cv::Rect(j, i, w, h));
				mse = MSE(&img_rot, &img_of_interest);
				
				if(debug == 1) std::cout << "MSE: " << mse << "\n";
				
				if(p.mse > mse)
				{
					p.point.x = j + img_query.cols/2; 
					p.point.y = i + img_query.rows/2; 
					p.angle = angl;
					p.mse = mse;
					if(debug == 1) std::cout << "BestPoint: " << p.point << "\n";
					if(debug == 1) std::cout << "MSE: " << p.mse << "\n";
					if(debug == 1) std::cout << "Angle: " << p.angle << "\n";

				}
			}
		}
		k++;
	}
	
	
	FILE *pFile;
	pFile = fopen("myfile.txt", "w");
	if(debug == 1)	std::cout << "\n\n\nMSE: " << p.mse << " point: " << p.point << "\n";
	fprintf(pFile,"MSE: %f, X: %d, Y: %d\n", p.mse, p.point.x, p.point.y);
	fclose(pFile);
	
	cv::waitKey(0);
	
	return 0;
		
}

