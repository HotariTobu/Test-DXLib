#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ModelHandle;
	float VRotate, HRotate, TRotate;

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら直ちに終了
		return -1;
	}

	// モデルの読み込み
	ModelHandle = MV1LoadModel("samples/Kabe.mqo");

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラの回転値を初期化
	VRotate = 0.0f;
	HRotate = 0.0f;
	TRotate = 0.0f;

	// ＥＳＣキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面をクリア
		ClearDrawScreen();

		// 左シフトキーを押しているかどうかで処理を分岐
		if (CheckHitKey(KEY_INPUT_LSHIFT) == 1)
		{
			// 左右キーでカメラの捻り方向回転値を変更
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				TRotate -= DX_PI_F / 60.0f;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				TRotate += DX_PI_F / 60.0f;
			}
		}
		else
		{
			// 左右キーでカメラの水平方向回転値を変更
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				HRotate -= DX_PI_F / 60.0f;
			}
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				HRotate += DX_PI_F / 60.0f;
			}
		}

		// 上下キーでカメラの垂直方向回転値を変更
		if (CheckHitKey(KEY_INPUT_UP) == 1)
		{
			VRotate -= DX_PI_F / 60.0f;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) == 1)
		{
			VRotate += DX_PI_F / 60.0f;
		}

		// カメラの位置と回転値をセット、カメラの位置は原点
		SetCameraPositionAndAngle(VGet(0.0f, 0.0f, 0.0f), VRotate, HRotate, TRotate);

		// モデルをカメラを囲むように４個描画
		MV1SetPosition(ModelHandle, VGet(0.0f, 0.0f, 800.0f));
		MV1DrawModel(ModelHandle);

		MV1SetPosition(ModelHandle, VGet(0.0f, 0.0f, -800.0f));
		MV1DrawModel(ModelHandle);

		MV1SetPosition(ModelHandle, VGet(800.0f, 0.0f, 0.0f));
		MV1DrawModel(ModelHandle);

		MV1SetPosition(ModelHandle, VGet(-800.0f, 0.0f, 0.0f));
		MV1DrawModel(ModelHandle);

		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// モデルハンドルの削除
	MV1DeleteModel(ModelHandle);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}