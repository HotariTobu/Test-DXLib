#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ModelHandle;
	int i;
	MV1_REF_POLYGONLIST RefPoly;

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら直ちに終了
		return -1;
	}

	// ３Ｄモデルの読み込み
	ModelHandle = MV1LoadModel("YON.mqo");

	// ３Ｄモデルを見える位置に移動する
	MV1SetPosition(ModelHandle, VGet(320.0f, 220.0f, -300.0f));

	// モデル全体の参照用メッシュを構築
	MV1SetupReferenceMesh(ModelHandle, -1, TRUE);

	// 参照用メッシュ情報の取得
	RefPoly = MV1GetReferenceMesh(ModelHandle, -1, TRUE);

	// ポリゴンの数だけ繰り返し
	for (i = 0; i < RefPoly.PolygonNum; i++)
	{
		// ポリゴンを形成する三頂点を使用してワイヤーフレームを描画する
		DrawLine3D(
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[1]].Position,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[2]].Position,
			RefPoly.Vertexs[RefPoly.Polygons[i].VIndex[0]].Position,
			GetColor(255, 255, 0));
	}

	// モデルハンドルの削除
	MV1DeleteModel(ModelHandle);

	// キー入力待ち
	WaitKey();

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}