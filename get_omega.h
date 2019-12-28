#pragma once
#include <opencv2/opencv.hpp>
#include "buff_detect.h"



using namespace cv;
using namespace std;



class get_omega
{
public:
	
	int frame_center_y = 0;
	int frame_center_x = 0;
	float time0 = 0;

	void forecast(Mat& image, int show_num, float angle, struct logo final_center_R);
	Point forecast_point(float angle, float omega, struct logo R);
	void drawgraph(float point_x, float point_y, int show_num);

private:

	float average_omega[5];
	float target_angle1 = -1;
	float target_angle2 = 0;
	float dangle = 0;
	float omega = -1;
	float omega_ = -1;
	int direction = 0;
	int omega_num = 0;
	
	Mat graph_x, graph_y;
	vector<Point> point1;
	vector<Point> point2;
	Point2f point;

};

