#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ModelHandle, MeshNum, i;
	VECTOR Position;

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら直ちに終了
		return -1;
	}

	// ３Ｄモデルの読み込み
	ModelHandle = MV1LoadModel("YON3.mqo");

	// モデルに含まれるメッシュの数を取得する
	MeshNum = MV1GetMeshNum(ModelHandle);

	// メッシュの数だけループ
	for (i = 0; i < MeshNum; i++)
	{
		// 画面をクリア
		ClearDrawScreen();

		// メッシュが使用しているマテリアルの描画
		DrawFormatString(0, 0, GetColor(255, 255, 255), "Material              %d", MV1GetMeshMaterial(ModelHandle, i));

		// メッシュに含まれる三角形ポリゴンの数を描画
		DrawFormatString(0, 16, GetColor(255, 255, 255), "Triangle Polygon Num  %d", MV1GetMeshTriangleNum(ModelHandle, i));

		// メッシュの表示・非表示状態を描画
		DrawFormatString(0, 32, GetColor(255, 255, 255), "Visible               %d", MV1GetMeshVisible(ModelHandle, i));

		// メッシュに半透明要素があるかどうかを描画
		DrawFormatString(0, 48, GetColor(255, 255, 255), "Semi Trans State      %d", MV1GetMeshSemiTransState(ModelHandle, i));

		// メッシュがバックカリングを行うかどうかを描画
		DrawFormatString(0, 64, GetColor(255, 255, 255), "Back Culling          %d", MV1GetMeshBackCulling(ModelHandle, i));

		// メッシュの頂点ディフューズカラーを使用するかどうかを描画
		DrawFormatString(0, 80, GetColor(255, 255, 255), "Use Vertex Dif Color  %d", MV1GetMeshUseVertDifColor(ModelHandle, i));

		// メッシュの頂点スペキュラカラーを使用するかどうかを描画
		DrawFormatString(0, 96, GetColor(255, 255, 255), "Use Vertex Spc Color  %d", MV1GetMeshUseVertSpcColor(ModelHandle, i));

		// メッシュに含まれる頂点のローカル座標の最大座標値を描画
		Position = MV1GetMeshMaxPosition(ModelHandle, i);
		DrawFormatString(0, 112, GetColor(255, 255, 255), "Max Position          x %f  y %f  z %f", Position.x, Position.y, Position.z);

		// メッシュに含まれる頂点のローカル座標の最小座標値を描画
		Position = MV1GetMeshMinPosition(ModelHandle, i);
		DrawFormatString(0, 128, GetColor(255, 255, 255), "Min Position          x %f  y %f  z %f", Position.x, Position.y, Position.z);

		// キーの入力待ち
		WaitKey();
	}

	// モデルハンドルの削除
	MV1DeleteModel(ModelHandle);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}