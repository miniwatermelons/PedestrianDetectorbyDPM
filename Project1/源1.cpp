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
//	Mat image = imread("D:\\dataset\\2.jpg");  //����Լ�ͼ���·�� 
//	imshow("��ʾͼ��", image);
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
//	IplImage* img1 = cvLoadImage("3.jpg");//��Ӧ��·���±����д�ͼƬ  
//	cvShowImage("p", img1);
//
//	//���ļ��ж���ͼ��  
//	Mat img = imread("3.jpg");
//
//	//�������ͼ��ʧ��  
//	if (img.empty())
//	{
//		std::cout << "Can not load image" << "\n";
//		return -1;
//	}
//	//��ʾͼ��  
//	imshow("image", img);
//	waitKey();
//	return 0;
//}