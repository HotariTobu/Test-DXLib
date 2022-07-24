#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ModelHandle;
	VECTOR StartPos, EndPos;
	MV1_COLL_RESULT_POLY HitPoly;
	int Mx, My;

	ChangeWindowMode(true);

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら直ちに終了
		return -1;
	}

	// ３Ｄモデルの読み込み
	ModelHandle = MV1LoadModel("YON.mqo");

	// ３Ｄモデルを見える位置に移動する
	//MV1SetPosition(ModelHandle, VGet(320.0f, 300.0f, 600.0f));

	MV1SetPosition(ModelHandle, VGet(320.0f, 220.0f, -300.0f));
	MV1SetRotationXYZ(ModelHandle, VGet(-1.5, 0, 0));

	// 描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);

	// モデル全体のコリジョン情報を構築
	MV1SetupCollInfo(ModelHandle, -1, 8, 8, 8);

	// ウインドウが閉じられるか何かキーが押されるまでループ
	while (ProcessMessage() == 0 && CheckHitKeyAll() == 0)
	{
		// 画面をクリア
		ClearDrawScreen();

		// ３Ｄモデルの描画
		MV1DrawModel(ModelHandle);

		// マウスの座標を取得
		GetMousePoint(&Mx, &My);

		// マウスポインタがある画面上の座標に該当する３Ｄ空間上の Near 面の座標を取得
		StartPos = ConvScreenPosToWorldPos(VGet(Mx, My, 0.0f));

		// マウスポインタがある画面上の座標に該当する３Ｄ空間上の Far 面の座標を取得
		EndPos = ConvScreenPosToWorldPos(VGet(Mx, My, 1.0f));

		// モデルと線分との当たり判定
		HitPoly = MV1CollCheck_Line(ModelHandle, -1, StartPos, EndPos);

		// 当たったかどうかで処理を分岐
		if (HitPoly.HitFlag == 1)
		{
			// 当たった場合は衝突の情報を描画する

			// 当たったポリゴンを黄色で描画する
			DrawTriangle3D(HitPoly.Position[0], HitPoly.Position[1], HitPoly.Position[2], GetColor(255, 255, 0), TRUE);

			// 交差した座標を描画
			DrawFormatString(0, 0, GetColor(255, 255, 255), "Hit Pos   %f  %f  %f",
				HitPoly.HitPosition.x, HitPoly.HitPosition.y, HitPoly.HitPosition.z);

			// 当たったポリゴンが含まれるフレームの番号を描画
			DrawFormatString(0, 16, GetColor(255, 255, 255), "Frame     %d", HitPoly.FrameIndex);

			// 当たったポリゴンが使用しているマテリアルの番号を描画
			DrawFormatString(0, 32, GetColor(255, 255, 255), "Material  %d", HitPoly.MaterialIndex);

			// 当たったポリゴンを形成する三頂点の座標を描画
			DrawFormatString(0, 48, GetColor(255, 255, 255), "Position  %f  %f  %f",
				HitPoly.Position[0].x, HitPoly.Position[0].y, HitPoly.Position[0].z);
			DrawFormatString(0, 64, GetColor(255, 255, 255), "          %f  %f  %f",
				HitPoly.Position[1].x, HitPoly.Position[1].y, HitPoly.Position[1].z);
			DrawFormatString(0, 80, GetColor(255, 255, 255), "          %f  %f  %f",
				HitPoly.Position[2].x, HitPoly.Position[2].y, HitPoly.Position[2].z);

			// 当たったポリゴンの法線を描画
			DrawFormatString(0, 96, GetColor(255, 255, 255), "Normal    %f  %f  %f",
				HitPoly.Normal.x, HitPoly.Normal.y, HitPoly.Normal.z);
		}
		else
		{
			// 当たらなかった場合は衝突しなかった旨だけ描画する
			DrawString(0, 0, "NO HIT", GetColor(255, 255, 255));
		}

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