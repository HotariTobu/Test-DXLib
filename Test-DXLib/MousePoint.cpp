#include<DxLib.h>
#include<string>

using namespace std;

int x = 320, y = 240;
int mx, my;
string str;

void MousePoint_Init() {

}

void MousePoint_Update() {
	GetMousePoint(&mx, &my);

	str = to_string(mx) + " : " + to_string(my);

	int lc;
	GetDrawStringSize(&x, &y, &lc, str.c_str(), GetStringLength(str.c_str()));

	x = (640 - x) / 2;
	y = (480 - y) / 2;
}

void MousePoint_Draw() {
	DrawString(x, y, str.c_str(), GetColor(255, 255, 255), GetColor(0, 0, 0));
}

void MousePoint_Dispose() {

}