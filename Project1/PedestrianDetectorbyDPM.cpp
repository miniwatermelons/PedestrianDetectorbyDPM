#include "dpm.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include<vector>

#include <io.h>
#include <string>
#include <sstream>


using namespace cv;
using namespace cv::dpm;
using namespace std;

const string model_path = "D:\\Code\\Library\\external_library\\dpm\\inriaperson.xml";


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
	size_t i,
	string output_dir);

bool isInside(Rect rect1, Rect rect2);

void getFiles(string path, vector<string>& files);

vector<string> getFilesCompatible(string cate_dir);

int main(int argc, char** argv)
{
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

	string image_list = argv[1];

	string output_dir = argv[2];
	cout <<"input dir is： "<< image_dir << endl;
	cout <<"output dir is: "<<output_dir << endl;

	if (model_path.empty() || image_dir.empty())
	{
		help();
		return -1;
	}



	//vector<string> imgFileList;
	//if (!readImageLists(image_list, imgFileList))
	//	return -1;

	//string imgfile = "..\\srcImg";
	vector<string>imgFileList;
	/*getFiles(image_list, imgFileList);
	for (int i = 0; i<imgFileList.size(); i++) {
		cout << imgFileList.at(i) << endl;
	}*/
	
	imgFileList = getFilesCompatible(image_dir);
	for (int t = 0; t<imgFileList.size(); t++)
	{
		cout << imgFileList[t] << endl;
	}

	cout << "**********************************" << endl;
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

	
	
	cv::Ptr<DPMDetector> detector = \
		DPMDetector::create(vector<string>(1, model_path));


	//namedWindow("DPM Cascade Detection", 1);
	// the color of the rectangle
	Scalar color(0, 255, 255); // yellow
	Mat frame;

	for (size_t i = 0; i < imgFileList.size()-1; i++)
	{
		double t = (double)getTickCount();
		vector<DPMDetector::ObjectDetection> ds;

		//gstring imageFile = image_dir + "\\" + imgFileList[i];
		string imageFile = imgFileList[i];
	
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
		storeBoxes(frame, ds, color, text, i, output_dir);

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
	vector<DPMDetector::ObjectDetection> ds, Scalar color, string text, size_t i, string output_dir)
{
	for (unsigned int j = 0; j < ds.size(); j++)
	{
		//rectangle(frame, ds[j].rect, color, 0);
		Rect frame_rect(0, 0, frame.cols, frame.rows);
		if (isInside(ds[j].rect, frame_rect))
		{
			Mat dst = frame(ds[j].rect);

			char ImagePathName[100];
			sprintf_s(ImagePathName, "%s\\%zd_%d%s", output_dir.c_str(), i, j, ".jpg");   //指定保存路径
			cout << "generate image:" << ImagePathName << endl;
			imwrite(ImagePathName, dst);  //保存图像
		}

		
	}

}

//OpenCV里貌似没有判断rect1是否在rect2里面的功能，所以自己写一个吧,引用自别的代码
bool isInside(Rect rect1, Rect rect2)
{
	return (rect1 == (rect1&rect2));
}


void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	intptr_t  hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

vector<string> getFilesCompatible(string cate_dir)
{
	vector<string> files;//存放文件名  
	cout << "^^^^^^^^^^^^^^^^^" << endl;
#ifdef _WIN64  
	struct _finddata_t file;
	intptr_t lf=0;
	//输入文件夹路径  
	string p;
	if ((lf = _findfirst(p.assign(cate_dir).append("\\*").c_str(), &file)) == -1) {
		cout << cate_dir << " not found!!!" << endl;
	}
	else {

		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((file.attrib &  _A_SUBDIR))
			{
				if (strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0)
					getFiles(p.assign(cate_dir).append("\\").append(file.name), files);
			}
			else
			{
				files.push_back(p.assign(cate_dir).append("\\").append(file.name));
			}
		} while (_findnext(lf, &file) == 0);
	}
	_findclose(lf);
#endif  

#ifdef linux  
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir(cate_dir.c_str())) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir  
			continue;
		else if (ptr->d_type == 8)    ///file  
									  //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
			files.push_back(ptr->d_name);
		else if (ptr->d_type == 10)    ///link file  
									   //printf("d_name:%s/%s\n",basePath,ptr->d_name);  
			continue;
		else if (ptr->d_type == 4)    ///dir  
		{
			files.push_back(ptr->d_name);
			/*
			memset(base,'\0',sizeof(base));
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_nSame);
			readFileList(base);
			*/
		}
	}
	closedir(dir);
#endif  

	//排序，按从小到大排序  
	sort(files.begin(), files.end());
	return files;
}