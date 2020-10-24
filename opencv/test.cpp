#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>    
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include<vector>
#define UNKNOWN_ARMOR -1
#define BIG_ARMOR 1 
#define SMALL_ARMOR 0
#define thresh_init 100
#define thresh_max 255
using namespace cv;
using namespace std;

double light_max_y_diff_ratio_ = 2.0;
double light_min_x_diff_ratio_ = 0.6;

double armor_min_aspect_ratio_ = 0.5;
double armor_max_aspect_ratio_ = 3;

class LightDescriptor
{
public:
	LightDescriptor() {};
	LightDescriptor(const cv::RotatedRect& light)
	{
		width = light.size.width;
		length = light.size.height;
		center = light.center;
		angle = light.angle;
		area = light.size.area();
	}
	const LightDescriptor& operator =(const LightDescriptor& ld)
	{
		this->width = ld.width;
		this->length = ld.length;
		this->center = ld.center;
		this->angle = ld.angle;
		this->area = ld.area;
		return *this;
	}
	RotatedRect rec() const
	{
		return RotatedRect(center, cv::Size2f(width, length), angle);
	}

public:
	float width;
	float length;
	cv::Point2f center;
	float angle;
	float area;
};
void deallight(vector<RotatedRect>lightInfos, Mat src);
void getlight(Mat src) {
	vector<Mat> channels;
	//ͨ������
	split(src, channels);

	//��ͨ������ͨ������ͨ���Ҷ�ͼ
	Mat image_gray = channels[0] - channels[2];

	Mat image_bright;
	//��ֵ��
	threshold(image_gray, image_bright, thresh_init, thresh_max, THRESH_BINARY);

	// ����
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));
	dilate(image_bright, image_bright, element);
	vector<vector<Point>> lightContours;
	vector<Vec4i> g_vHierarchy;
	//������
	findContours(image_bright, lightContours, g_vHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//��������
	vector<RotatedRect>lightInfos;
	for (const auto& contour : lightContours)
	{
		float lightContourArea = contourArea(contour);
		if (contour.size() <= 5 ||
			lightContourArea < 10) continue;
		RotatedRect lightRec = fitEllipse(contour);

		if (lightRec.size.width / lightRec.size.height >
			1.0 ||
			lightContourArea / lightRec.size.area() <
			0.5
			)continue;
		lightRec.size.width *= 1.1;
		lightRec.size.height *= 1.1;
		Rect lightRect = lightRec.boundingRect();
		const Rect srcBound(Point(0, 0), src.size());
		lightRect &= srcBound;
		lightInfos.push_back(lightRec);
	}
	deallight(lightInfos, src);            //���õ���������
}
void deallight(vector<RotatedRect>lightInfos, Mat src) {
	sort(lightInfos.begin(), lightInfos.end(), [](const LightDescriptor& ld1, const LightDescriptor& ld2)
		{
			return ld1.center.x < ld2.center.x;
		});
	for (size_t i = 0; i < lightInfos.size(); i++)
	{//�������е�������ƥ��
		for (size_t j = i + 1; (j < lightInfos.size()); j++)
		{
			const RotatedRect& leftLight = lightInfos[i];
			const RotatedRect& rightLight = lightInfos[j];
			//�ǲ�
			float angleDiff_ = abs(leftLight.angle - rightLight.angle);
			//���Ȳ����
			float LenDiff_ratio = abs(leftLight.size.height - rightLight.size.height) / max(leftLight.size.height, rightLight.size.height);
			//ɸѡ
			if (angleDiff_ > 5.5 ||
				LenDiff_ratio > 0.2)
			{
				continue;
			}
			Point2f v1[4];
			leftLight.points(v1);
			Point2f v2[4];
			rightLight.points(v2);
			//���ҵ���������
			float dis = abs(v1[3].x - v2[2].x);
			//���ҵ������ȵ�ƽ��ֵ
			float meanLen = (leftLight.size.height + rightLight.size.height) / 2;
			//���ҵ������ĵ�y�Ĳ�ֵ
			float yDiff = abs(leftLight.center.y - rightLight.center.y);
			//y�����
			float yDiff_ratio = yDiff / meanLen;
			//���ҵ������ĵ�x�Ĳ�ֵ
			float xDiff = abs(leftLight.center.x - rightLight.center.x);
			//x�����
			float xDiff_ratio = xDiff / meanLen;
			//��������������ȱ�ֵ
			float ratio = dis / meanLen;
			//ɸѡ
			if (yDiff_ratio > light_max_y_diff_ratio_ ||
				xDiff_ratio < light_min_x_diff_ratio_ ||
				ratio>1.70 && ratio < 1.90 ||
				ratio>armor_max_aspect_ratio_ ||
				ratio < armor_min_aspect_ratio_)
			{
				continue;
			}
			//�õ�ƥ���װ�װ�

			line(src, v1[0], v2[3], Scalar(0, 255, 0), 2);
			line(src, v1[0], v1[1], Scalar(0, 255, 0), 2);
			line(src, v1[1], v2[2], Scalar(0, 255, 0), 2);
			line(src, v2[2], v2[3], Scalar(0, 255, 0), 2);
		}
	}
}
int main()
{
	VideoCapture capture;
	capture.open("C:/Users/veen/Desktop/��̨�ز�����ǰ�Խ�-ev--3.MOV");
	while (1) {
		Mat src;
		capture >> src;
		if (src.empty()) break;
		getlight(src);
		imshow("  ", src);
		waitKey(15);
	}
}
