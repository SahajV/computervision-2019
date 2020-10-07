//Aadith Charugundla Period 3 Computer Vision
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv; 
using namespace std; 

int main(int argc, char** argv)
{
	if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
	
	Mat image, image_gray;
    image = imread( argv[1], 1 );
	
	 if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
	
	cvtColor( image, image_gray, CV_BGR2GRAY );
	
	
	GaussianBlur(image_gray, image_gray, Size(13, 13), 2, 2); 
	//GaussianBlur(image_gray, image_gray, Size(13, 13), 2, 2);
	//GaussianBlur(image_gray, image_gray, Size(13, 13), 2, 2);
	//GaussianBlur(image_gray, image_gray, Size(13, 13), 2, 2);	
	
	vector<Vec3f> hVec; 
	
	HoughCircles(image_gray, hVec, CV_HOUGH_GRADIENT, 1, 150, 120, 60, 0, 0 );
	
	int numCoins = hVec.size(); 
	
	cout << "The number of coins is: " << numCoins << "\n"; 
	
	for(int i = 0; i < hVec.size(); ++i)
	{
		Point center(cvRound(hVec[i][0]), cvRound(hVec[i][1]));
		int radius = cvRound(hVec[i][2]); 
		cout << "radius of " << radius << "\n"; 
		circle( image, center, 3, Scalar(0,255,255), -1, 8, 0 );
		circle( image, center, radius, Scalar(170,0,255), 3, 8, 0 );

	}
	
	imwrite("/afs/csl.tjhsst.edu/students/2021/2021acharugu/Computer Vision/coinDec_Image.jpg", image); 
	
	namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

    waitKey(0);

    return 0;
	
}