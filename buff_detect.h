#pragma once
#include <opencv2/opencv.hpp>
#include "drawRect.h"
#include "opencv2/ml.hpp"

using namespace cv;
using namespace std;
using namespace ml;

class buff_detect
{
public:

	void color_buff(Mat& image, Mat& dst, int threshValue, int color);
	
	//能量机关总任务
	int BuffDetectTask(Mat& image, Mat& dst, float& angle, struct logo& center_R);
		
	//画出目标
	void drawtarget(vector<RotatedRect>& target, Mat& image);
	
	//识别目标并得到扇叶的角度
	void buff_detection(Mat& img, Mat& dst, vector<RotatedRect>& target_R, vector<RotatedRect>& target_armor);
	
	//机器学习识别R
	void machineLearning(Mat image, vector<RotatedRect> R, vector<RotatedRect>& final_R, Ptr<SVM> svm);

	void machineLearningF(Mat image, RotatedRect father, vector<RotatedRect>&blade, Ptr<SVM> svm);

	//得到能量机关中心R
	void get_energy_center(Mat image, RotatedRect father, RotatedRect son, vector<RotatedRect> rect, vector<RotatedRect>& R);

	void read_xml(int color, int& threshValue)
	{
		FileStorage fs1("test.xml", FileStorage::READ);

		if (color == 0)
			fs1["thresh0"] >> threshValue;//蓝色
		else
			fs1["thresh1"] >> threshValue;//红色

		fs1.release();
	}

private:
	string modelpath = "./svmR_blade16.xml";
	Ptr<SVM> svm = StatModel::load<SVM>(modelpath);//加载寻找扇叶的xml

	string modelpath1 = "./svmR25.xml";
	Ptr<SVM> svm1 = StatModel::load<SVM>(modelpath1);//加载寻找R的xml

	drawbox box;	
	int svmWidth = 8;
	int svmHeight = 16;
	bool get_R = false;
};
	
struct logo
{
	float x = 0;
	float y = 0;
	float r = 0;
};
