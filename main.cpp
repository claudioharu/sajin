#include "match.h"
#include <pthread.h>


#define NTHREADS 3
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

BestPoint p;

char* query_path = "/home/junior/Desktop/imagens/query/minhaquery1.png";		
//char* query_path = "/home/junior/Desktop/imagens/query/009_coca_obj.png";
//char* target_path = "/home/junior/Desktop/imagens/query/009_coca_obj.png";
char* target_path = "/home/junior/Desktop/imagens/target/sem_ruido/009_coca_tgt.png";

void *thread_function(void*);

int main( int argc, char** argv )
{
	//Debug's flag
	short debug = atoi(argv[1]);
	
	if(atoi(argv[2]) == 1)
	{
		pthread_t thread_id[NTHREADS];

		for(int i = 0; i < NTHREADS; i++)
		{
			pthread_create( &thread_id[i], NULL, thread_function, NULL);
		}

		for(int i = 0; i < NTHREADS; i++)
		{
			pthread_join( thread_id[i], NULL);
		}
	}
	else
	{
		//Inputs
		cv::Mat img_query, img_target;
		double mse;
		
		p.mse = Max;
		//Load imgs
		img_query = cv::imread(query_path,0);
		img_target = cv::imread(target_path,0);
		cv::Mat img_of_interest;
		
		
		
		cv::namedWindow( "Img Query", cv::WINDOW_NORMAL );// Create a window for display.
		cv::imshow( "Img Query", img_query );                   // Show our image inside it.

		cv::namedWindow( "Img Target", cv::WINDOW_NORMAL );// Create a window for display.
		cv::imshow( "Img Target", img_target );                   // Show our image inside it.

		//rotate(&img_query, 0.0, &img_of_interest);

		
		//cv::namedWindow("Img of Interest", cv::WINDOW_NORMAL);
		//cv::imshow( "Img of Interest", img_of_interest );                   // Show our image inside it.
		/*
		char buffer[40];
		char name[100];	
		
		int k = 0;*/ 
		int h, w;
		for(int i  = 0; i < img_target.rows; i ++)
		{
			for(int j = 0; j < img_target.cols; j++)
			{
						
				if(j + img_query.cols > img_target.cols) break;//w = img_target.cols - j;
				else w = img_query.cols;
				if(i + img_query.rows > img_target.rows) break;//h = img_target.rows - i;
				else h = img_query.rows;
				
				img_of_interest = img_target(cv::Rect(j, i, w, h));
				mse = MSE(&img_query, &img_of_interest);
				
				if(debug == 1) std::cout << "MSE: " << mse << "\n";
				
				if(p.mse > mse)
				{
					p.point.x = j + img_query.cols/2; //Arrumar pra pegar soh do pedaço do target
					p.point.y = i + img_query.rows/2; //Arrumar pra pegar soh do pedaço do target
					p.mse = mse;
					if(debug == 1) std::cout << "BestPoint: " << p.point << "\n";
					if(debug == 1) std::cout << "MSE: " << p.mse << "\n";

				}
				//k ++;
			}
		}
	}
	
	FILE *pFile;
	pFile = fopen("myfile.txt", "w");
	if(debug == 1)	std::cout << "\n\n\nMSE: " << p.mse << " point: " << p.point << "\n";
	fprintf(pFile,"MSE: %f, X: %d, Y: %d\n", p.mse, p.point.x, p.point.y);
	fclose(pFile);
	
	cv::waitKey(0);
	
	return 0;
		
}


void *thread_function(void*)
{
   	cv::Mat img_query, img_target;
	img_query = cv::imread(query_path,0);
	img_target = cv::imread(target_path,0);
	cv::Mat img_of_interest;
	
	p.mse = Max;

	double mse;

    int h, w;
	for(int i  = 0; i < img_target.rows; i ++)
	{
		for(int j = 0; j < img_target.cols; j++)
		{					
			if(j + img_query.cols > img_target.cols) break;//w = img_target.cols - j;
			else w = img_query.cols;
			if(i + img_query.rows > img_target.rows) break;//h = img_target.rows - i;
			else h = img_query.rows;
			
			img_of_interest = img_target(cv::Rect(j, i, w, h));
			mse = MSE(&img_query, &img_of_interest);
			
			//if(debug == 1) std::cout << "MSE: " << mse << "\n";
			std::cout << "MSE: " << mse << "\n";
			std::cout << "P.MSE: " << p.mse << "\n";
			pthread_mutex_lock( &mutex1 );
			if(p.mse > mse)
			{
				
				p.point.x = j; //Arrumar pra pegar soh do pedaço do target
				p.point.y = i; //Arrumar pra pegar soh do pedaço do target
				p.mse = mse;
				//if(debug == 1) 
				std::cout << "BestPoint: " << p.point << "\n";
				//if(debug == 1) 
				std::cout << "MSE: " << p.mse << "\n";
				
			}
			pthread_mutex_unlock( &mutex1 );
		}
	}
	
   
}
