#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
//����Ŀ¼�µ�temp�ļ������.slnͬ����temp�ļ�
string writePath = "../temp/";
int main(int argc, char** argv) {
	VideoCapture capture(0);
	string name;
	namedWindow("hello", WINDOW_AUTOSIZE);
	int i = 0;
	while (1) {
		Mat frame;
		capture >> frame;
		if (32 == waitKey(20)) {			//�ո�����
			name = writePath + to_string(i) + ".jpg";
			imwrite(name, frame);
			cout << name << endl;
			i++;
		}
		if (97 == waitKey(10)) {			//'a'�˳�
			break;
		}
		imshow("hello", frame);

	}
	//waitKey(0);
}