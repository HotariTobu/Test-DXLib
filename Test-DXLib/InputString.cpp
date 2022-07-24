#include<DxLib.h>

int key;

void InputString_Init() {
	key = MakeKeyInput(256, true, false, false, false, false);
	
	SetActiveKeyInput(key);
}

void InputString_Update() {

}

void InputString_Draw() {
	char text[256];

	switch (CheckKeyInput(key)) {
	case 0:
		DrawKeyInputString(0, 0, key);
		break;
	case 1:
		GetKeyInputString(text, key);
		DrawFormatString(0, 64, GetColor(255, 255, 255), "Width = %d", GetDrawStringWidth(text, GetStringLength(text)));
		break;
	case 2:
		DrawFormatString(0, 0, 0, "キャンセルされました。");
		break;
	case -1:
		DrawFormatString(0, 0, 0, "エラーが発生しました。");
		break;
	}
}

void InputString_Dispose() {
	DeleteKeyInput(key);
}