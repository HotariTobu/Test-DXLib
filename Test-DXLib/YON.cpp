#include<DxLib.h>

static int model=0;

/*static VECTOR sp, ep;
static MV1_COLL_RESULT_POLY hp;*/

void YON_Init() {
	model = MV1LoadModel("YON2.mqo");
	MV1SetupCollInfo(model);
}

void YON_Update() {
	/*scale += GetMouseWheelRotVolF() / 4;

	if (scale < 0.6) {
		scale = 0.6f;
	}
	else if (scale > 1.6) {
		scale = 1.6f;
	}*/

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
	}

	MV1RefreshCollInfo(model, -1);
}

void YON_Draw() {
	MV1DrawModel(model);

	/*sp = ConvScreenPosToWorldPos(VGet((float)mx, (float)my, 0.0f));
	ep = ConvScreenPosToWorldPos(VGet((float)mx, (float)my, 1.0f));

	hp = MV1CollCheck_Line(model, -1, sp, ep);

	if (hp.HitFlag == 1)
	{
		DrawTriangle3D(hp.Position[0], hp.Position[1], hp.Position[2], GetColor(255, 255, 0), TRUE);

		DrawFormatString(0, 0, GetColor(255, 255, 255), "Hit Pos   %f  %f  %f", hp.HitPosition.x, hp.HitPosition.y, hp.HitPosition.z);

		DrawFormatString(0, 16, GetColor(255, 255, 255), "Frame     %d", hp.FrameIndex);

		DrawFormatString(0, 32, GetColor(255, 255, 255), "Material  %d", hp.MaterialIndex);

		DrawFormatString(0, 48, GetColor(255, 255, 255), "Position  %f  %f  %f", hp.Position[0].x, hp.Position[0].y, hp.Position[0].z);
		DrawFormatString(0, 64, GetColor(255, 255, 255), "          %f  %f  %f", hp.Position[1].x, hp.Position[1].y, hp.Position[1].z);
		DrawFormatString(0, 80, GetColor(255, 255, 255), "          %f  %f  %f", hp.Position[2].x, hp.Position[2].y, hp.Position[2].z);

		DrawFormatString(0, 96, GetColor(255, 255, 255), "Normal    %f  %f  %f", hp.Normal.x, hp.Normal.y, hp.Normal.z);
	}*/
}

void YON_Dispose() {
	MV1DeleteModel(model);
}