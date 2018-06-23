#include "dpm.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <stdio.h>
#include <iostream>
#include <fstream>



using namespace cv;
using namespace cv::dpm;
using namespace std;

const string model_path = "D:\\test\\dpm\\inriaperson.xml";

int save_results(const string id, const vector<DPMDetector::ObjectDetection> ds, ofstream &out);

static void help()
{
	cout << "\nThis example shows object detection on image sequences using \"Deformable Part-based Model (DPM) cascade detection API\n"
		"Call:\n"
		"./example_dpm_cascade_detect_sequence <model_path> <image_dir>\n"
		"The image names has to be provided in \"files.txt\" under <image_dir>.\n"
		<< endl;
}

static bool readImageLists(const string &file, vector<string> &imgFileList)
{
	ifstream in(file.c_str(), ios::binary);

	if (in.is_open())
	{
		while (in)
		{
			string line;
			getline(in, line);
			imgFileList.push_back(line);
		}
		return true;
	}
	else
	{
		cerr << "Invalid image index file: " << file << endl;
		return false;
	}
}

void drawBoxes(Mat &frame,
	vector<DPMDetector::ObjectDetection> ds,
	Scalar color,
	string text);

void storeBoxes(Mat &frame, \
	vector<DPMDetector::ObjectDetection> ds, 
	Scalar color, 
	string text, 
	size_t i);

int main(int argc, char** argv)
{
	
	waitKey(0);

	const char* keys =
	{
		"{@model_path    | | Path of the DPM cascade model}"
		"{@image_dir     | | Directory of the images      }"
	};

	CommandLineParser parser(argc, argv, keys);

	//args
	/*string model_path = "D:\\test\\dpm\\inriaperson.xml";
	string image_dir = "D:\\dataset";
	string image_list = "D:\\test\\files";*/
	
	string image_dir = argv[1];
	string image_list = argv[2];

	if (model_path.empty() || image_dir.empty())
	{
		help();
		return -1;
	}



	vector<string> imgFileList;
	if (!readImageLists(image_list, imgFileList))
		return -1;


	//for (int k = 0; k < imgFileList.size()-1; k++)
	//{
	//	cout << imgFileList[k].size() << ' ';
	//}
	//
	//cout << endl;
	//cout << (int)imgFileList[9].size() << endl;
#ifdef HAVE_TBB
	cout << "Running with TBB" << endl;
#else
#ifdef _OPENMP
	cout << "Running with OpenMP" << endl;
#else
	cout << "Running without OpenMP and without TBB" << endl;
#endif
#endif

	cout << "press any key to continue";
	
	cv::Ptr<DPMDetector> detector = \
		DPMDetector::create(vector<string>(1, model_path));
	getchar();


	//namedWindow("DPM Cascade Detection", 1);
	// the color of the rectangle
	Scalar color(0, 255, 255); // yellow
	Mat frame;

	for (size_t i = 0; i < imgFileList.size()-1; i++)
	{
		double t = (double)getTickCount();
		vector<DPMDetector::ObjectDetection> ds;

		string imageFile = image_dir + "\\" + imgFileList[i];
	
		//getchar();
		Mat image = imread(imageFile);
		/*if (image.empty()) {
			cerr << "\nInvalid image:\n" << imgFileList[i] << endl;
		}
		imshow("show", image);*/

		frame = image.clone();

		if (image.empty()) {
			cerr << "\nInvalid image:\n" << imgFileList[i] << endl;
			return -1;
		}

		// detection
		detector->detect(image, ds);
		// compute frame per second (fps)
		t = ((double)getTickCount() - t) / getTickFrequency();//elapsed time
		cout << t << endl;
		// draw boxes
		string text = format("%0.1f fps", 1.0 / t);
		//drawBoxes(frame, ds, color, text);
		storeBoxes(frame, ds, color, text, i);

	}
	system("pause");
	return 0;
}

void drawBoxes(Mat &frame, \
	vector<DPMDetector::ObjectDetection> ds, Scalar color, string text)
{
	for (unsigned int i = 0; i < ds.size(); i++)
	{
		rectangle(frame, ds[i].rect, color, 2);
		Mat dst = frame(ds[i].rect);
		imshow("cut", dst);//注意需要加waitkey，否则会只显示最后切割的图片

		waitKey(0);
	}

	// draw text on image
	Scalar textColor(0, 0, 250);
	putText(frame, text, Point(10, 50), FONT_HERSHEY_PLAIN, 2, textColor, 2);
}

void storeBoxes(Mat &frame, \
	vector<DPMDetector::ObjectDetection> ds, Scalar color, string text, size_t i)
{
	for (unsigned int j = 0; j < ds.size(); j++)
	{
		//rectangle(frame, ds[j].rect, color, 0);
		Mat dst = frame(ds[j].rect);

		char ImagePathName[100];
		sprintf_s(ImagePathName, "%s%zd_%d%s", "..\\getPedestrianimage\\image", i, j, ".jpg");   //指定保存路径
		imwrite(ImagePathName, dst);  //保存图像
	}

}