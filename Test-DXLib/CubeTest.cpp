#include<DxLib.h>

static int model;

static float x = 320, y = 240, z = -300, scale = 20, ax, ay;
static int mx, my;
static int lmx, lmy;

static int move = 10;

void CubeTest_Init() {
	model = MV1LoadModel("CubeTest.mqo");

	GetMousePoint(&lmx, &lmy);
}

void CubeTest_Update() {
	scale += GetMouseWheelRotVol()*2;

	if (scale < 10) {
		scale = 10;
	}
	else if (scale > 50) {
		scale = 50;
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		GetMousePoint(&mx, &my);

		ay += (float)(lmx - mx) / 120;
		ax += (float)(lmy - my) / 120;
	}

	GetMousePoint(&lmx, &lmy);

	MV1SetPosition(model, VGet(x, y, z));
	MV1SetScale(model, VGet(scale, scale, scale));
	MV1SetRotationXYZ(model, VGet(ax,ay,0));
}

void CubeTest_Draw() {
	MV1DrawModel(model);
}

void CubeTest_Dispose() {
	MV1DeleteModel(model);
}