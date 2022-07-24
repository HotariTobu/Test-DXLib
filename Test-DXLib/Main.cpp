#include<DxLib.h>
#include<math.h>
//#include"Henoheno.h"
//#include"CubeTest.h"
//#include"MousePoint.h"
//#include"YON.h"
#include"InputString.h"
//#include"Connect.h"

float cameraX, cameraY, cameraZ;
int distance = 200;
int ah, av;
int nmx, nmy;
int lmx, lmy;

void rotate(float *x, float *y, const int ang) {
	const float ox = *x, oy = *y;
	const double chnang = ang*DX_PI / 180;
	*x = (float)(ox * cos(chnang) - oy * sin(chnang));
	*y = (float)(ox * sin(chnang) + oy * cos(chnang));
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(false);
	ChangeWindowMode(true);
	SetMainWindowText("Test-DXLib");

	if (DxLib_Init() == -1) {
		return -1;
	}

	SetBackgroundColor(0, 0, 255);
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, (float)distance), VGet(0, 0, 0));
	GetMousePoint(&lmx, &lmy);

	//Henoheno_Init();
	//CubeTest_Init();
	//MousePoint_Init();
	//YON_Init();
	InputString_Init();
	//Connect_Init();

	while (!CheckHitKey(KEY_INPUT_ESCAPE) && ProcessMessage() == 0 && ClearDrawScreen()==0) {

		/*distance -= GetMouseWheelRotVol()*20;

		if (distance < 0) {
			distance = 0;
		}

		GetMousePoint(&nmx, &nmy);

		if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
			ah += (lmx - nmx);
			av += (lmy - nmy);

			ah = ah % 360;
			
			if (av < -90) {
				av = -90;
			}
			else if (av > 90) {
				av = 90;
			}

		}

		GetMousePoint(&lmx, &lmy);

		cameraX = 0;
		cameraY = 0;
		cameraZ = (float)distance;

		rotate(&cameraY, &cameraZ, av);
		rotate(&cameraZ, &cameraX, -ah);

		SetCameraPositionAndTarget_UpVecY(VGet(cameraX, cameraY, cameraZ), VGet(0, 0, 0));*/

		//Henoheno_Update();
		//CubeTest_Update();
		//MousePoint_Update();
		//YON_Update();
		InputString_Update();
		//Connect_Update();

		//Henoheno_Draw();
		//CubeTest_Draw();
		//MousePoint_Draw();
		//YON_Draw();
		InputString_Draw();
		//Connect_Draw();

		ScreenFlip();
	}

	//Henoheno_Dispose();
	//CubeTest_Dispose();
	//MousePoint_Dispose();
	//YON_Dispose();
	InputString_Dispose();
	//Connect_Dispose();

	DxLib_End();

	return 0;
}