#include "DxLib.h"
#include <math.h>

#define BLOCK_SIZE		1000.0f		// ブロックのサイズ

#define BLOCK_NUM_X		16		// Ｘ方向のブロック数
#define BLOCK_NUM_Z		16		// Ｚ方向のブロック数

#define CAMERA_Y		500.0f		// カメラの高さ

#define MOVE_FRAME		30		// 移動や旋回に掛けるフレーム数

// マップ( 1:道  0:壁 )
char Map[BLOCK_NUM_Z][BLOCK_NUM_X] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,0,1,1,1,0,1,1,0,1,1,0,
	0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,
	0,1,1,1,1,1,1,0,1,0,0,1,1,1,1,0,
	0,1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,
	0,0,0,1,1,1,0,1,0,0,0,1,0,0,1,0,
	0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,
	0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,
	0,0,0,1,1,1,1,1,0,0,0,1,0,0,1,0,
	0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,
	0,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,
	0,1,0,1,1,1,0,0,0,1,1,0,1,0,0,0,
	0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int KabeModel;		// 壁モデル
	int x, z;		// 位置
	int movx, movz;	// 移動先の座標
	int Muki;		// 向き( 0:x軸プラス方向  1:z軸マイナス方向  2:x軸マイナス方向  3:z軸プラス方向 )
	int NowInput;		// 現在のフレームの入力
	int FrameNo;		// フレーム番号
	int State;		// 状態( 0:入力待ち 1:前進中 2:後退中 3:左旋回中 4:右旋回中 )
	int Count;		// 汎用カウンタ
	int i, j;		// 汎用変数
	float f;		// 汎用変数
	VECTOR CamPos;		// カメラの座標
	VECTOR CamDir;		// カメラの向いている方向
	VECTOR CamTarg;	// カメラの注視点

					// ウインドウモードで起動
	ChangeWindowMode(TRUE);

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0) return -1;

	// 壁モデルの読みこみ
	KabeModel = MV1LoadModel("samples/Kabe.mqo");

	// 位置と向きの初期化
	x = 1;
	z = 1;
	Muki = 0;

	// カメラの座標と向きと注視点をセットする
	CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
	CamDir = VGet(1.0f, 0.0f, 0.0f);
	CamTarg = VAdd(CamPos, CamDir);
	SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

	// 状態の初期化
	State = 0;

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// メインループ
	// エスケープキーが押されるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面をクリアする
		ClearDrawScreen();

		// 現在の入力を取得する
		NowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// 状態によって処理を分岐
		switch (State)
		{
		case 0:	// 入力待ち状態

				// 上が押されたら向いている方向に移動する状態に移行する
			if ((NowInput & PAD_INPUT_UP) != 0)
			{
				// 向きによって移動方向が変わる
				switch (Muki)
				{
				case 0: movx = 1; movz = 0; break;		// Ｘ軸プラス方向
				case 1: movx = 0; movz = -1; break;		// Ｚ軸マイナス方向
				case 2: movx = -1; movz = 0; break;		// Ｘ軸マイナス方向
				case 3: movx = 0; movz = 1; break;		// Ｚ軸プラス方向
				}

				// 移動先のマスが道だったら移動する
				if (Map[z + movz][x + movx] == 1)
				{
					// 状態を前進中にする
					State = 1;
					Count = 0;
				}
			}

			// 下が押されたら向いている方向と逆方向に移動する
			if ((NowInput & PAD_INPUT_DOWN) != 0)
			{
				// 向きによって移動方向が変わる
				switch (Muki)
				{
				case 0: movx = -1; movz = 0; break;		// Ｘ軸プラス方向
				case 1: movx = 0; movz = 1; break;		// Ｚ軸マイナス方向
				case 2: movx = 1; movz = 0; break;		// Ｘ軸マイナス方向
				case 3: movx = 0; movz = -1; break;		// Ｚ軸プラス方向
				}

				// 移動先のマスが道だったら移動する
				if (Map[z + movz][x + movx] == 1)
				{
					// 状態を後退中にする
					State = 2;
					Count = 0;
				}
			}

			// 左が押されていたら向いている方向を左に９０度変更する
			if ((NowInput & PAD_INPUT_LEFT) != 0)
			{
				// 状態を左旋回中にする
				State = 3;
				Count = 0;
			}

			// 右が押されていたら向いている方向を右に９０度変更する
			if ((NowInput & PAD_INPUT_RIGHT) != 0)
			{
				// 状態を右旋回中にする
				State = 4;
				Count = 0;
			}

			break;

		case 1:	// 前進中状態
				// カウントを進める
			Count++;

			// カメラの座標を移動途中の座標にする
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VAdd(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 2:	// 後退中状態
				// カウントを進める
			Count++;

			// カメラの座標を移動途中の座標にする
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VSub(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 3:	// 左旋回中状態
				// カウントを進める
			Count++;

			// 向いている方向を旋回途中の方向にする
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// Ｘプラス方向
			case 1: f = -DX_PI_F / 2.0f; break;		// Ｚマイナス方向
			case 2: f = DX_PI_F; break;		// Ｘマイナス方向
			case 3: f = DX_PI_F / 2.0f; break;		// Ｚプラス方向
			}
			f += DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				if (Muki == 0)
				{
					Muki = 3;
				}
				else
				{
					Muki--;
				}

				State = 0;
				Count = 0;
			}
			break;

		case 4:	// 右旋回中状態
				// カウントを進める
			Count++;

			// 向いている方向を旋回途中の方向にする
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// Ｘプラス方向
			case 1: f = -DX_PI_F / 2.0f; break;		// Ｚマイナス方向
			case 2: f = DX_PI_F; break;		// Ｘマイナス方向
			case 3: f = DX_PI_F / 2.0f; break;		// Ｚプラス方向
			}
			f -= DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
			if (Count == MOVE_FRAME)
			{
				if (Muki == 3)
				{
					Muki = 0;
				}
				else
				{
					Muki++;
				}

				State = 0;
				Count = 0;
			}
			break;
		}

		// カメラの位置と向きをセットする
		SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

		// マップを描画する
		for (i = 0; i < BLOCK_NUM_Z; i++)
		{
			for (j = 0; j < BLOCK_NUM_X; j++)
			{
				// 道ではないところは描画しない
				if (Map[i][j] == 0) continue;

				// 壁モデルの座標を変更する
				MV1SetPosition(KabeModel, VGet(j * BLOCK_SIZE, 0.0f, i * BLOCK_SIZE));

				// ４方の壁の状態で描画するフレーム番号を変更する
				FrameNo = 0;
				if (Map[i][j + 1] == 0) FrameNo += 1;
				if (Map[i][j - 1] == 0) FrameNo += 2;
				if (Map[i + 1][j] == 0) FrameNo += 4;
				if (Map[i - 1][j] == 0) FrameNo += 8;

				// 割り出した番号のフレームを描画する
				MV1DrawFrame(KabeModel, FrameNo);
			}
		}

		// 裏画面の内容を表画面に反映する
		ScreenFlip();
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}