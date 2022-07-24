// �L�����N�^�[�ɕt���Ă����J����
//
// �����쁄
// �����L�[�F�L�����N�^�[���f���ړ�
// Z,C�L�[�F�J�����̐����p�x��ύX
// S,X�L�[�F�J�����̐����p�x��ύX

#include "DxLib.h"
#include <math.h>

// �ړ����x
#define MOVESPEED			10.0f

// DxChara.x �ł̑���A�j���[�V�����̔ԍ�
#define ANIM_RUN			1

// DxChara.x �ł̑ҋ@�A�j���[�V�����̔ԍ�
#define ANIM_NEUTRAL			4

// �J�����̉�]���x
#define CAMERA_ANGLE_SPEED		3.0f

// �J�����̒����_�̍���
#define CAMERA_LOOK_AT_HEIGHT		400.0f

// �J�����ƒ����_�̋���
#define CAMERA_LOOK_AT_DISTANCE		2150.0f

// ���C����`���͈�
#define LINE_AREA_SIZE			100000.0f

// ���C���̐�
#define LINE_NUM			500

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int    ModelHandle;
	float  AnimTotalTime;
	float  AnimNowTime;
	int    AnimAttachIndex;
	int    RunFlag;
	int    MoveAnimFrameIndex;
	VECTOR Position;
	int    MoveFlag;
	float  Angle;
	float  CameraHAngle;
	float  CameraVAngle;
	VECTOR MoveVector;
	float  SinParam;
	float  CosParam;

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);

	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0) return -1;

	// �R�c���f���̓ǂݍ���
	ModelHandle = MV1LoadModel("samples/DxChara.x");

	// �J�����̌�����������
	CameraHAngle = 0.0f;
	CameraVAngle = 40.0f;

	// ������������
	Angle = 0.0f;

	// �����Ă���t���O��|��
	RunFlag = FALSE;

	// �ҋ@�A�j���[�V�������A�^�b�`
	AnimAttachIndex = MV1AttachAnim(ModelHandle, ANIM_NEUTRAL);

	// �ҋ@�A�j���[�V�����̑����Ԃ��擾���Ă���
	AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

	// �A�j���[�V�����Đ����Ԃ�������
	AnimNowTime = 0.0f;
	MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

	// �A�j���[�V�����ňړ������Ă���t���[���̔ԍ�����������
	MoveAnimFrameIndex = MV1SearchFrame(ModelHandle, "BasePoint");

	// �A�j���[�V�����ňړ������Ă���t���[���𖳌��ɂ���
	MV1SetFrameUserLocalMatrix(ModelHandle, MoveAnimFrameIndex, MV1GetFrameLocalMatrix(ModelHandle, MoveAnimFrameIndex));

	// �R�c���f���̍��W��������
	Position = VGet(0.0f, 0.0f, 0.0f);

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// �J�����̃N���b�s���O������ݒ�
	SetCameraNearFar(100.0f, 50000.0f);

	// �w�i�̐F���D�F�ɂ���
	SetBackgroundColor(128, 128, 128);

	// ���C�����[�v(�����L�[�������ꂽ�烋�[�v�𔲂���)
	while (ProcessMessage() == 0)
	{
		// ��ʂ̃N���A
		ClearDrawScreen();

		// ZCSX�L�[�ŃJ�����̑���
		if (CheckHitKey(KEY_INPUT_C) == 1)
		{
			CameraHAngle += CAMERA_ANGLE_SPEED;
			if (CameraHAngle >= 180.0f)
			{
				CameraHAngle -= 360.0f;
			}
		}

		if (CheckHitKey(KEY_INPUT_Z) == 1)
		{
			CameraHAngle -= CAMERA_ANGLE_SPEED;
			if (CameraHAngle <= -180.0f)
			{
				CameraHAngle += 360.0f;
			}
		}

		if (CheckHitKey(KEY_INPUT_S) == 1)
		{
			CameraVAngle += CAMERA_ANGLE_SPEED;
			if (CameraVAngle >= 80.0f)
			{
				CameraVAngle = 80.0f;
			}
		}

		if (CheckHitKey(KEY_INPUT_X) == 1)
		{
			CameraVAngle -= CAMERA_ANGLE_SPEED;
			if (CameraVAngle <= 0.0f)
			{
				CameraVAngle = 0.0f;
			}
		}

		// �ړ��x�N�g����������
		MoveVector = VGet(0.0f, 0.0f, 0.0f);

		// �ړ����Ă��邩�ǂ����̃t���O��|��
		MoveFlag = FALSE;

		// �������͂ɏ]���ăL�����N�^�[�̈ړ��x�N�g���ƌ�����ݒ�
		if (CheckHitKey(KEY_INPUT_LEFT) == 1)
		{
			Angle = 90.0f - CameraHAngle;
			MoveFlag = TRUE;
			MoveVector.x = -MOVESPEED;
		}

		if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
		{
			Angle = -90.0f - CameraHAngle;
			MoveFlag = TRUE;
			MoveVector.x = MOVESPEED;
		}

		if (CheckHitKey(KEY_INPUT_DOWN) == 1)
		{
			Angle = 0.0f - CameraHAngle;
			MoveFlag = TRUE;
			MoveVector.z = -MOVESPEED;
		}

		if (CheckHitKey(KEY_INPUT_UP) == 1)
		{
			Angle = 180.0f - CameraHAngle;
			MoveFlag = TRUE;
			MoveVector.z = MOVESPEED;
		}

		// �ړ������ꍇ�́A�J�����̐����p�x���l�����������ɍ��W���ړ�����
		if (MoveFlag == TRUE)
		{
			VECTOR TempMoveVector;

			// �J�����̊p�x�ɍ��킹�Ĉړ��x�N�g������]���Ă�����Z
			SinParam = sin(CameraHAngle / 180.0f * DX_PI_F);
			CosParam = cos(CameraHAngle / 180.0f * DX_PI_F);
			TempMoveVector.x = MoveVector.x * CosParam - MoveVector.z * SinParam;
			TempMoveVector.y = 0.0f;
			TempMoveVector.z = MoveVector.x * SinParam + MoveVector.z * CosParam;

			Position = VAdd(Position, TempMoveVector);
		}

		// ���܂łƏ�Ԃ��ω������ꍇ�̓A�j���[�V������ύX����
		if (RunFlag != MoveFlag)
		{
			// �����Ă��邩�ǂ����̃t���O��ۑ�
			RunFlag = MoveFlag;

			// ���܂ŃA�^�b�`���Ă����A�j���[�V�������f�^�b�`
			MV1DetachAnim(ModelHandle, AnimAttachIndex);

			// �V�����A�j���[�V�������A�^�b�`
			if (RunFlag)
			{
				AnimAttachIndex = MV1AttachAnim(ModelHandle, ANIM_RUN);
			}
			else
			{
				AnimAttachIndex = MV1AttachAnim(ModelHandle, ANIM_NEUTRAL);
			}

			// �A�j���[�V�����̑����Ԃ��擾���Ă���
			AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

			// �A�j���[�V�����Đ����Ԃ�������
			AnimNowTime = 0.0f;
		}
		else
		{
			// �A�j���[�V�����Đ����Ԃ�i�߂�
			AnimNowTime += 200.0f;

			// �A�j���[�V�����Đ����Ԃ��A�j���[�V�����̑����Ԃ��z���Ă����烋�[�v������
			if (AnimNowTime >= AnimTotalTime)
			{
				// �V�����A�j���[�V�����Đ����Ԃ́A�A�j���[�V�����Đ����Ԃ���A�j���[�V���������Ԃ�����������
				AnimNowTime -= AnimTotalTime;
			}
		}

		// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
		MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

		// �V�����������Z�b�g
		MV1SetRotationXYZ(ModelHandle, VGet(0.0f, Angle / 180.0f * DX_PI_F, 0.0f));

		// �R�c���f���ɐV�������W���Z�b�g
		MV1SetPosition(ModelHandle, Position);

		// �J�����̈ʒu�ƌ�����ݒ�
		{
			VECTOR TempPosition1;
			VECTOR TempPosition2;
			VECTOR CameraPosition;
			VECTOR CameraLookAtPosition;

			// �����_�̓L�����N�^�[���f���̍��W���� CAMERA_LOOK_AT_HEIGHT �����������ʒu
			CameraLookAtPosition = Position;
			CameraLookAtPosition.y += CAMERA_LOOK_AT_HEIGHT;

			// �J�����̈ʒu�̓J�����̐����p�x�Ɛ����p�x����Z�o

			// �ŏ��ɐ����p�x�𔽉f�����ʒu���Z�o
			SinParam = sin(CameraVAngle / 180.0f * DX_PI_F);
			CosParam = cos(CameraVAngle / 180.0f * DX_PI_F);
			TempPosition1.x = 0.0f;
			TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE;
			TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE;

			// ���ɐ����p�x�𔽉f�����ʒu���Z�o
			SinParam = sin(CameraHAngle / 180.0f * DX_PI_F);
			CosParam = cos(CameraHAngle / 180.0f * DX_PI_F);
			TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z;
			TempPosition2.y = TempPosition1.y;
			TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z;

			// �Z�o�������W�ɒ����_�̈ʒu�����Z�������̂��J�����̈ʒu
			CameraPosition = VAdd(TempPosition2, CameraLookAtPosition);

			// �J�����̐ݒ�ɔ��f����
			SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition);
		}

		// �R�c���f���̕`��
		MV1DrawModel(ModelHandle);

		// �ʒu�֌W��������悤�ɒn�ʂɃ��C����`�悷��
		{
			int i;
			VECTOR Pos1;
			VECTOR Pos2;

			SetUseZBufferFlag(TRUE);

			Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
			Pos2 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, LINE_AREA_SIZE / 2.0f);
			for (i = 0; i <= LINE_NUM; i++)
			{
				DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
				Pos1.x += LINE_AREA_SIZE / LINE_NUM;
				Pos2.x += LINE_AREA_SIZE / LINE_NUM;
			}

			Pos1 = VGet(-LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
			Pos2 = VGet(LINE_AREA_SIZE / 2.0f, 0.0f, -LINE_AREA_SIZE / 2.0f);
			for (i = 0; i < LINE_NUM; i++)
			{
				DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
				Pos1.z += LINE_AREA_SIZE / LINE_NUM;
				Pos2.z += LINE_AREA_SIZE / LINE_NUM;
			}

			SetUseZBufferFlag(FALSE);
		}

		// ����ʂ̓��e��\��ʂɔ��f
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}