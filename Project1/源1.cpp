//#include "dpm.hpp"
//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//
//#include <stdio.h>
//#include <iostream>
//#include <fstream>
//
//using namespace cv;
//using namespace cv::dpm;
//using namespace std;
//
//int main()
//{
//	Mat image = imread("D:\\dataset\\2.jpg");  //存放自己图像的路径 
//	imshow("显示图像", image);
//	
//	waitKey();
//	system("pause");
//	return 0;
//}

//#include<highgui.hpp>  
//#include<iostream>  
//using namespace cv;
//
//int main(void)
//{
//	IplImage* img1 = cvLoadImage("3.jpg");//对应的路径下保存有此图片  
//	cvShowImage("p", img1);
//
//	//从文件中读入图像  
//	Mat img = imread("3.jpg");
//
//	//如果读入图像失败  
//	if (img.empty())
//	{
//		std::cout << "Can not load image" << "\n";
//		return -1;
//	}
//	//显示图像  
//	imshow("image", img);
//	waitKey();
//	return 0;
//}