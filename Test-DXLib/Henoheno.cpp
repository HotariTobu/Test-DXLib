#include<DxLib.h>

static int img;

static int x, y;

static int move = 10;

void Henoheno_Init() {
	img=LoadGraph("�ւ̂ւ́H.png");
}

void Henoheno_Update() {
	if (CheckHitKey(KEY_INPUT_UP)) {
		y -= move;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN)) {
		y += move;
	}


	if (CheckHitKey(KEY_INPUT_LEFT)) {
		x -= move;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT)) {
		x += move;
	}
}

void Henoheno_Draw() {
	DrawGraph(x, y, img, true);
}

void Henoheno_Dispose() {
	DeleteGraph(img);
}