#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
using namespace std;
using namespace cv;

/**
 * @������
 */
int main(int argc, char** argv)
{

	/// ��ȡһ��ͼƬ�����ı�ͼƬ�������ɫ���ͣ��ö�ȡ��ʽΪDOS����ģʽ��
	Mat src = imread("17.jpg", 1);
	imshow("1", src);
	Mat distortion = src.clone();
	imshow("2", distortion);
	Mat camera_matrix = Mat(3, 3, CV_32FC1);
	Mat distortion_coefficients;


	//��������ڲκͻ���ϵ������
	FileStorage file_storage("out_camera_data.yml", FileStorage::READ);
	file_storage["Camera_Matrix"] >> camera_matrix;
	cout << camera_matrix << endl;
	file_storage["Distortion_Conefficients"] >> distortion_coefficients;
	cout << distortion_coefficients << endl;
	file_storage.release();

	//����
	undistort(src, distortion, camera_matrix, distortion_coefficients);

	imshow("img", src);
	imshow("undistort", distortion);
	imwrite("undistort.jpg", distortion);

	waitKey(0);
	return 0;
}

void calAngle(Mat cam, Mat dis, int x, int y)
{
    double fx = cam.at<double>(0, 0);
    double fy = cam.at<double>(1, 1);
    double cx = cam.at<double>(0, 2);
    double cy = cam.at<double>(1, 2);
    Point2f pnt;
    vector<cv::Point2f> in;
    vector<cv::Point2f> out;
    in.push_back(Point2f(x, y));
    //�����ص�ȥ����
    undistortPoints(in, out, cam, dis, noArray(), cam);
    pnt = out.front();
    //û��ȥ����ʱ�ı�ֵ
    double rx = (x - cx) / fx;
    double ry = (y - cy) / fy;
    //ȥ�����ı�ֵ
    double rxNew = (pnt.x - cx) / fx;
    double ryNew = (pnt.y - cy) / fy;
    //���ԭ����������ȥ�����������
    cout << "x: " << x << " xNew:" << pnt.x << endl;
    cout << "y: " << y << " yNew:" << pnt.y << endl;
    //���δȥ����ʱ��õĽǶȺ�ȥ������õĽǶ�
    cout << "angx: " << atan(rx) / CV_PI * 180 << " angleNew:" << atan(rxNew) / CV_PI * 180 << endl;
    cout << "angy: " << atan(ry) / CV_PI * 180 << " angleNew:" << atan(ryNew) / CV_PI * 180 << endl;
}
