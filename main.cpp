#include<opencv.hpp>
#include "get_omega.h"

#define COLOR 0  // 0 Ϊʶ����ɫ�������أ�1 Ϊʶ���ɫ��������

buff_detect buff;
get_omega GetOmega;

void energy(VideoCapture capture);

int main()
{
	VideoCapture capture("C:\\Users\\86184\\Desktop\\��Ƶ\\��������\\blue1.mov");
	GetOmega.frame_center_y = capture.get(CAP_PROP_FRAME_HEIGHT) / 2;
	GetOmega.frame_center_x = capture.get(CAP_PROP_FRAME_WIDTH) / 2;

	energy(capture);

	waitKey(0);

}

void energy(VideoCapture capture)
{


	Mat frame, image, dst;
	int target_num = 0;
	int threshValue = 0;
	int show_num = 0;
	float angle = 0;
	logo center_R;

	buff.read_xml(COLOR, threshValue);

	capture >> frame;
	dst.create(frame.size(), CV_8UC1);

	namedWindow("Ч��");
	createTrackbar("��ֵ", "Ч��", &threshValue, 255);

	while (1)
	{
		capture >> frame;
		if (frame.empty())
		{
			break;
		}
		frame.copyTo(image);

		//ֱ��ȡ����ɫͨ������ֵ��
		buff.color_buff(image, dst, threshValue, COLOR);

		//ִ��ͼ�����������ص�ʶ������		image ԭͼ��    dst��ɫͨ�����
		target_num = buff.BuffDetectTask(image, dst, angle, center_R);

		if (target_num != 0 && show_num == 1)
		{
			GetOmega.time0 = static_cast<double>(getTickCount());//��ʼ��ʱ
		}

		//Ԥ����һ���λ��
		GetOmega.forecast(image, show_num, angle, center_R);


		imshow("Ч��", image);
		imshow("��ֵͼ", dst);

		waitKey(1);

		show_num++;
	}
}

