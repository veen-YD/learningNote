#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
//工程目录下的temp文件夹里，与.sln同级的temp文件
string writePath = "../temp/";
int main(int argc, char** argv) {
	VideoCapture capture(0);
	string name;
	namedWindow("hello", WINDOW_AUTOSIZE);
	int i = 0;
	while (1) {
		Mat frame;
		capture >> frame;
		if (32 == waitKey(20)) {			//空格拍照
			name = writePath + to_string(i) + ".jpg";
			imwrite(name, frame);
			cout << name << endl;
			i++;
		}
		if (97 == waitKey(10)) {			//'a'退出
			break;
		}
		imshow("hello", frame);

	}
	//waitKey(0);
}