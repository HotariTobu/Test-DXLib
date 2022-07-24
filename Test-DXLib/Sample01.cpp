#include "DxLib.h"
#include <math.h>

#define BLOCK_SIZE		1000.0f		// �u���b�N�̃T�C�Y

#define BLOCK_NUM_X		16		// �w�����̃u���b�N��
#define BLOCK_NUM_Z		16		// �y�����̃u���b�N��

#define CAMERA_Y		500.0f		// �J�����̍���

#define MOVE_FRAME		30		// �ړ������Ɋ|����t���[����

// �}�b�v( 1:��  0:�� )
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
	int KabeModel;		// �ǃ��f��
	int x, z;		// �ʒu
	int movx, movz;	// �ړ���̍��W
	int Muki;		// ����( 0:x���v���X����  1:z���}�C�i�X����  2:x���}�C�i�X����  3:z���v���X���� )
	int NowInput;		// ���݂̃t���[���̓���
	int FrameNo;		// �t���[���ԍ�
	int State;		// ���( 0:���͑҂� 1:�O�i�� 2:��ޒ� 3:������ 4:�E���� )
	int Count;		// �ėp�J�E���^
	int i, j;		// �ėp�ϐ�
	float f;		// �ėp�ϐ�
	VECTOR CamPos;		// �J�����̍��W
	VECTOR CamDir;		// �J�����̌����Ă������
	VECTOR CamTarg;	// �J�����̒����_

					// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);

	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0) return -1;

	// �ǃ��f���̓ǂ݂���
	KabeModel = MV1LoadModel("samples/Kabe.mqo");

	// �ʒu�ƌ����̏�����
	x = 1;
	z = 1;
	Muki = 0;

	// �J�����̍��W�ƌ����ƒ����_���Z�b�g����
	CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
	CamDir = VGet(1.0f, 0.0f, 0.0f);
	CamTarg = VAdd(CamPos, CamDir);
	SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

	// ��Ԃ̏�����
	State = 0;

	// �`���𗠉�ʂɂ���
	SetDrawScreen(DX_SCREEN_BACK);

	// ���C�����[�v
	// �G�X�P�[�v�L�[���������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ��N���A����
		ClearDrawScreen();

		// ���݂̓��͂��擾����
		NowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// ��Ԃɂ���ď����𕪊�
		switch (State)
		{
		case 0:	// ���͑҂����

				// �オ�����ꂽ������Ă�������Ɉړ������ԂɈڍs����
			if ((NowInput & PAD_INPUT_UP) != 0)
			{
				// �����ɂ���Ĉړ��������ς��
				switch (Muki)
				{
				case 0: movx = 1; movz = 0; break;		// �w���v���X����
				case 1: movx = 0; movz = -1; break;		// �y���}�C�i�X����
				case 2: movx = -1; movz = 0; break;		// �w���}�C�i�X����
				case 3: movx = 0; movz = 1; break;		// �y���v���X����
				}

				// �ړ���̃}�X������������ړ�����
				if (Map[z + movz][x + movx] == 1)
				{
					// ��Ԃ�O�i���ɂ���
					State = 1;
					Count = 0;
				}
			}

			// ���������ꂽ������Ă�������Ƌt�����Ɉړ�����
			if ((NowInput & PAD_INPUT_DOWN) != 0)
			{
				// �����ɂ���Ĉړ��������ς��
				switch (Muki)
				{
				case 0: movx = -1; movz = 0; break;		// �w���v���X����
				case 1: movx = 0; movz = 1; break;		// �y���}�C�i�X����
				case 2: movx = 1; movz = 0; break;		// �w���}�C�i�X����
				case 3: movx = 0; movz = -1; break;		// �y���v���X����
				}

				// �ړ���̃}�X������������ړ�����
				if (Map[z + movz][x + movx] == 1)
				{
					// ��Ԃ���ޒ��ɂ���
					State = 2;
					Count = 0;
				}
			}

			// ����������Ă���������Ă�����������ɂX�O�x�ύX����
			if ((NowInput & PAD_INPUT_LEFT) != 0)
			{
				// ��Ԃ������񒆂ɂ���
				State = 3;
				Count = 0;
			}

			// �E��������Ă���������Ă���������E�ɂX�O�x�ύX����
			if ((NowInput & PAD_INPUT_RIGHT) != 0)
			{
				// ��Ԃ��E���񒆂ɂ���
				State = 4;
				Count = 0;
			}

			break;

		case 1:	// �O�i�����
				// �J�E���g��i�߂�
			Count++;

			// �J�����̍��W���ړ��r���̍��W�ɂ���
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VAdd(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 2:	// ��ޒ����
				// �J�E���g��i�߂�
			Count++;

			// �J�����̍��W���ړ��r���̍��W�ɂ���
			CamPos = VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE);
			CamPos = VSub(CamPos, VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME));
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
			if (Count == MOVE_FRAME)
			{
				x += movx;
				z += movz;

				State = 0;
				Count = 0;
			}
			break;

		case 3:	// �����񒆏��
				// �J�E���g��i�߂�
			Count++;

			// �����Ă�����������r���̕����ɂ���
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// �w�v���X����
			case 1: f = -DX_PI_F / 2.0f; break;		// �y�}�C�i�X����
			case 2: f = DX_PI_F; break;		// �w�}�C�i�X����
			case 3: f = DX_PI_F / 2.0f; break;		// �y�v���X����
			}
			f += DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g�����ڎ��ԂɒB�������������ύX���ē��͑҂���Ԃɖ߂�
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

		case 4:	// �E���񒆏��
				// �J�E���g��i�߂�
			Count++;

			// �����Ă�����������r���̕����ɂ���
			switch (Muki)
			{
			case 0: f = 0.0f; break;		// �w�v���X����
			case 1: f = -DX_PI_F / 2.0f; break;		// �y�}�C�i�X����
			case 2: f = DX_PI_F; break;		// �w�}�C�i�X����
			case 3: f = DX_PI_F / 2.0f; break;		// �y�v���X����
			}
			f -= DX_PI_F / 2.0f * Count / MOVE_FRAME;
			CamDir.x = cos(f);
			CamDir.z = sin(f);
			CamTarg = VAdd(CamPos, CamDir);

			// �J�E���g�����ڎ��ԂɒB�������������ύX���ē��͑҂���Ԃɖ߂�
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

		// �J�����̈ʒu�ƌ������Z�b�g����
		SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg);

		// �}�b�v��`�悷��
		for (i = 0; i < BLOCK_NUM_Z; i++)
		{
			for (j = 0; j < BLOCK_NUM_X; j++)
			{
				// ���ł͂Ȃ��Ƃ���͕`�悵�Ȃ�
				if (Map[i][j] == 0) continue;

				// �ǃ��f���̍��W��ύX����
				MV1SetPosition(KabeModel, VGet(j * BLOCK_SIZE, 0.0f, i * BLOCK_SIZE));

				// �S���̕ǂ̏�Ԃŕ`�悷��t���[���ԍ���ύX����
				FrameNo = 0;
				if (Map[i][j + 1] == 0) FrameNo += 1;
				if (Map[i][j - 1] == 0) FrameNo += 2;
				if (Map[i + 1][j] == 0) FrameNo += 4;
				if (Map[i - 1][j] == 0) FrameNo += 8;

				// ����o�����ԍ��̃t���[����`�悷��
				MV1DrawFrame(KabeModel, FrameNo);
			}
		}

		// ����ʂ̓��e��\��ʂɔ��f����
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}