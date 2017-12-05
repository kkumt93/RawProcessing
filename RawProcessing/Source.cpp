
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//エンディアン変換16bit版
uint16_t swap(uint16_t dat)
{
	char out[2], *p = (char*)&dat;
	out[1] = *p++;
	out[0] = *p++;
	return *(uint16_t*)out;
}

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


	for (int i = 0; i<480; i++) {
		for (int j = 0; j < 640; j++) {
			a = unsigned short((0xffff / 640) * j);
			a = swap(a);
			fout.write((char *)&a, sizeof(unsigned short));
		}
	}

	fout.close();
}

int main(int argc, const char* argv[])
{

	createTestRaw();

	return 0;
}