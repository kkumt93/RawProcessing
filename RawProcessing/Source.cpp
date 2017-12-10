#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

#define RAW_W 640
#define RAW_H 720

//Raw画像構造体
struct RawArray {
	int hsize = RAW_H;
	int wsize = RAW_W;
	unsigned short data[RAW_H][RAW_W] = { 0 };
};

//エンディアン変換16bit版
uint16_t swap(uint16_t dat)
{
	char out[2], *p = (char*)&dat;
	out[1] = *p++;
	out[0] = *p++;
	return *(uint16_t*)out;
}

//テストRaw画像作成
void createTestRaw()
{
	ofstream fout;
	fout.open("test.raw", ios::out | ios::binary | ios::trunc);

	if (!fout) {
		cout << "ファイル file.txt が開けません";
		exit(1);
	}

	unsigned short a;

	for (int i = 0; i<RAW_H; i++) {
		for (int j = 0; j < RAW_W; j++) {
			a = unsigned short((0xffff / 640) * j);
			a = swap(a);
			fout.write((char *)&a, sizeof(unsigned short));
		}
	}

	fout.close();
}

//Raw画像を配列に格納
void rawCvtArray(RawArray *img1,char *str)
{
	ifstream fin(str, ios::in | ios::binary);

	if (!fin) {
		cout << str << "can't open";
		exit(1);
	}

	unsigned short data;
	
	for (int i = 0; i < img1->hsize; i++) {
		for (int j = 0; j < img1->wsize; j++) {
			fin.read((char *)&data, sizeof(unsigned short));
			data = swap(data);
			img1->data[i][j] = data;
		}
	}
	
	fin.close();
}

//ArrayをMatに格納
void arrayCvtMat(RawArray *img1, Mat img2)
{

	for (int i = 0; i < img1->hsize; i++) {
		for (int j = 0; j < img1->wsize; j++) {
			img2.at<unsigned short>(i,j) = img1->data[i][j];
		}
	}


}

int main(int argc, const char* argv[]) {

	createTestRaw();

	RawArray img1;
	
	rawCvtArray(&img1,"test.raw");

	Mat img2(RAW_H, RAW_W, CV_16UC1);

	arrayCvtMat(&img1, img2);

	cv::namedWindow("Image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("Image", img2);


	waitKey(0);

	return 0;
}
