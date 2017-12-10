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

struct RawImg {
	int h_size = 720;
	int w_size = 640;
	unsigned short data[720][640] = { 0 };
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
	//  ファイルを開く
	//  ios::out は書き込み専用（省略可）
	//  ios::binary はバイナリ形式で出力（省略するとアスキー形式で出力）
	//  ios::truncはファイルを新規作成（省略可）
	//  ios::addにすると追記になる


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

void rawCvtArray(RawImg *img1,char *str)
{
	ifstream fin(str, ios::in | ios::binary);

	if (!fin) {
		cout << str << "can't open";
		exit(1);
	}

	unsigned short data;
	
	for (int i = 0; i < img1->h_size; i++) {
		for (int j = 0; j < img1->w_size; j++) {
			fin.read((char *)&data, sizeof(unsigned short));
			data = swap(data);
			img1->data[i][j] = data;
		}
	}
	
	fin.close();
}


int main(int argc, const char* argv[]) {

	createTestRaw();

	RawImg img1;
	
	rawCvtArray(&img1,"test.raw");

	cout << img1.data[0][0] << endl;
	cout << img1.data[719][639] << endl;

	return 0;
}
