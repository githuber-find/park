#include "get_omega.h"

#define PI 3.1415927
#define DELAY 0.5
#define DISTENCE 8000.0
//#define DEBUG

void get_omega::forecast(Mat& image, int show_num, float angle, struct logo final_center_R)
{
	//每隔10帧取一次角度差
	if (show_num % 10 == 0 && show_num != 0)
	{
		time0 = ((double)getTickCount() - time0) / getTickFrequency();//停止计时

		if (time0 < 0.3)//有时候time0计时不准
		{
			//得到角度差
			if (target_angle1 != -1)
			{
				target_angle2 = target_angle1;//令target_angle2等于target_angle1上次的数据
			}
			target_angle1 = angle;

			dangle = target_angle1 - target_angle2;

			if (abs(dangle) < 15)//当角度差较大说明换扇叶了
			{
				//得到方向
				//if (omega == -1)
				//{
				if (dangle > 0)
				{

					direction = 1;
				}
				else if (dangle < 0)
				{
					direction = -1;
				}
				//}

				//得到角速度
				omega = abs(dangle) / time0;
				average_omega[omega_num] = omega;
				omega_num++;

				if (omega_num > 4)
				{
					omega_num = 0;

					float min = average_omega[0];
					float max = average_omega[0];

					for (int i = 1; i < 5; i++)
					{
						if (min > average_omega[i])
						{
							min = average_omega[i];
						}
						if (max < average_omega[i])
						{
							max = average_omega[i];
						}
						omega_ = omega_ + average_omega[i];
					}
					omega_ = (omega_ - min - max) / 3;
				}
			}
			else
			{
				omega_num = 0;
				for (int i = 0; i < 5; i++)
				{
					average_omega[i] = 0;
				}
			}
		}

		time0 = static_cast<double>(getTickCount());//重新开始计时	
	}

	if (omega_ != -1)
	{
		Point2f point0(0, 0);
		point0 = forecast_point(angle, omega_, final_center_R);//预测点

		float delta_x0 = abs(point0.x - final_center_R.x);
		float delta_y0 = abs(point0.y - final_center_R.y);
		float distance = sqrt(powf(delta_x0, 2) + powf(delta_y0, 2));
		if (distance / final_center_R.r > 0.9 || distance / final_center_R.r < 1.1)
		{
			point = point0;
		}

		circle(image, point, 1, Scalar(0, 255, 0), 10);

		//得到摄像头中心到达预测点所需要的角度
		float delta_x1 = point.x - frame_center_x;//得到x方向上的象素距离
		float delta_y1 = -(point.y - frame_center_y);
		float distance_x = delta_x1 / final_center_R.r * 800;//得到x方向的实际距离
		float distance_y = delta_y1 / final_center_R.r * 800;

		float yaw = atan(distance_x / DISTENCE) / PI * 180;
		float pitch = atan(distance_y / DISTENCE) / PI * 180;

#ifdef DEBUG

		//画出曲线图
		drawgraph(yaw, pitch, show_num);

		imshow("graph_x", graph_x);
		imshow("graph_y", graph_y);

#endif // DEBUG

	}
}

Point get_omega::forecast_point(float angle, float omega, struct logo R)
{
	Point2f point;

	if (direction == 1)//顺时针情况
	{
		point.x = R.x + R.r * sin((angle + omega * DELAY) / 180 * PI);
		point.y = R.y - R.r * cos((angle + omega * DELAY) / 180 * PI);

	}
	else//逆时针情况
	{
		point.x = R.x + R.r * sin((angle - omega * DELAY) / 180 * PI);
		point.y = R.y - R.r * cos((angle - omega * DELAY) / 180 * PI);
	}
	return point;
}

void get_omega::drawgraph(float point_x, float point_y, int show_num)
{
	graph_x.create(Size(1400, 480), CV_8UC3);
	graph_y.create(Size(1400, 480), CV_8UC3);

	//Point point_1(show_num, 480 - point_x * 480 / (frame_center_x * 2));//point.x
	//Point point_2(show_num, 480 - point_y * 480 / (frame_center_y * 2));//point.y

	//Point point_1(show_num, 480 - point_x * 480 / 360);//angle

	Point point_1(show_num, 100 - point_x);//yaw
	Point point_2(show_num, 100 - point_y);//pitch

	point1.push_back(point_1);
	point2.push_back(point_2);

	if (point1.size() > 1)
	{
		line(graph_x, point1[point1.size() - 2], point1[point1.size() - 1], Scalar(0, 0, 255));
		line(graph_y, point2[point2.size() - 2], point2[point2.size() - 1], Scalar(0, 255, 0));
	}
}
