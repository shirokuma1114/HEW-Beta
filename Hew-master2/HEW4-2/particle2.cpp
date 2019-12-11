//=============================================================================
//
// �p�[�e�B�N������ [Particle2.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "myDirect3D.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "Particle2.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_Particle2	"asset/texture/effect000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	Particle2_SIZE_X		(50.0f)							// �r���{�[�h�̕�
#define	Particle2_SIZE_Y		(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_Particle2	(5.0f)							// �ړ����x

#define	MAX_Particle2			(1000)						// �r���{�[�h�ő吔


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos2;		// �ʒu
	D3DXVECTOR3 rot2;		// ��]
	D3DXVECTOR3 scale2;		// �X�P�[��
	D3DXVECTOR3 move2;		// �ړ���
	D3DXCOLOR col2;			// �F
	float fSizeX2;			// ��
	float fSizeY2;			// ����
	int nIdxShadow2;			// �eID
	int nLife2;				// ����
	bool bUse2;				// �g�p���Ă��邩�ǂ���
} PARTICLE2;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY);
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle2 = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParticle2 = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldParticle2;				// ���[���h�}�g���b�N�X

PARTICLE2				g_aParticle2[MAX_Particle2];		// �p�[�e�B�N�����[�N
D3DXVECTOR3				g_posBase2;						// �r���{�[�h�����ʒu
float					g_fWidthBase2 = 5.0f;			// ��̕�
float					g_fHeightBase2 = 10.0f;			// ��̍���
bool					g_bPause2 = false;				// �|�[�YON/OFF
float					g_roty2 = 0.0f;					// �ړ�����
float					g_spd2 = 0.0f;					// �ړ��X�s�[�h

bool					g_player_touch2 = true;			// �v���C���[�ɂ�����

														//=============================================================================
														// ����������
														//=============================================================================
HRESULT Particle2Manager::InitParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexParticle2(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_Particle2,			// �t�@�C���̖��O
		&g_pD3DTextureParticle2);	// �ǂݍ��ރ������[

	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		g_aParticle2[nCntParticle2].pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2[nCntParticle2].rot2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2[nCntParticle2].scale2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].move2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].col2 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].fSizeX2 = Particle2_SIZE_X;
		g_aParticle2[nCntParticle2].fSizeY2 = Particle2_SIZE_Y;
		g_aParticle2[nCntParticle2].nIdxShadow2 = -1;
		g_aParticle2[nCntParticle2].nLife2 = 0;
		g_aParticle2[nCntParticle2].bUse2 = false;
	}

	g_posBase2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_roty2 = 0.0f;
	g_spd2 = 0.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Particle2Manager::UninitParticle2(void)
{
	if (g_pD3DTextureParticle2 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureParticle2->Release();
		g_pD3DTextureParticle2 = NULL;
	}

	if (g_pD3DVtxBuffParticle2 != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffParticle2->Release();
		g_pD3DVtxBuffParticle2 = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Particle2Manager::UpdateParticle2(void)
{
	// �X�V����
	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		// �g�p��
		if (g_aParticle2[nCntParticle2].bUse2)
		{

			// ����������
			g_aParticle2[nCntParticle2].nLife2-=2;

			// �������Ȃ��Ȃ�����
			if (g_aParticle2[nCntParticle2].nLife2 <= 0)
			{
				// �\������Ȃ��Ȃ�
				g_aParticle2[nCntParticle2].bUse2 = false;
				// ReleaseShadow(g_aParticle2[nCntParticle2].nIdxShadow);
				// �e�̐������炷
				g_aParticle2[nCntParticle2].nIdxShadow2 = -1;
			}
			else
			{
				// ���l�ݒ�
				g_aParticle2[nCntParticle2].col2.a -= 0.05f;
				// �T�C�Y�ݒ�
				g_aParticle2[nCntParticle2].fSizeX2 -= 2.0f;
				g_aParticle2[nCntParticle2].fSizeY2 -= 2.0f;
				if (g_aParticle2[nCntParticle2].col2.a < 0.0f)
				{
					g_aParticle2[nCntParticle2].col2.a = 0.0f;
				}

				// �F�̐ݒ�
				SetColorParticle2(nCntParticle2, g_aParticle2[nCntParticle2].col2);
			}
		}
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void Particle2Manager::DrawParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	CAMERA *cam = GetCamera();

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);				// Z�o�b�t�@�[�̏������݂����Ȃ�
																	//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z��r�Ȃ�

																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		if (g_aParticle2[nCntParticle2].bUse2)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldParticle2);

			// �r���[�}�g���b�N�X���擾
			mtxView = cam->mtxView;

			g_mtxWorldParticle2._11 = mtxView._11;
			g_mtxWorldParticle2._12 = mtxView._21;
			g_mtxWorldParticle2._13 = mtxView._31;
			g_mtxWorldParticle2._21 = mtxView._12;
			g_mtxWorldParticle2._22 = mtxView._22;
			g_mtxWorldParticle2._23 = mtxView._32;
			g_mtxWorldParticle2._31 = mtxView._13;
			g_mtxWorldParticle2._32 = mtxView._23;
			g_mtxWorldParticle2._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aParticle2[nCntParticle2].scale2.x, g_aParticle2[nCntParticle2].scale2.y, g_aParticle2[nCntParticle2].scale2.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aParticle2[nCntParticle2].pos2.x, g_aParticle2[nCntParticle2].pos2.y, g_aParticle2[nCntParticle2].pos2.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle2);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle2, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureParticle2);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle2 * 4), NUM_POLYGON);
		}
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�o�b�t�@�[�̏������݂�����
																	//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);				// Z��r����

																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);	// �A���t�@�u�����f�B���O����
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// �ŏ��̃A���t�@����
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// �Q�Ԗڂ̃A���t�@����
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_Particle2,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffParticle2,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-Particle2_SIZE_X / 2, -Particle2_SIZE_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(Particle2_SIZE_X / 2, -Particle2_SIZE_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-Particle2_SIZE_X / 2, Particle2_SIZE_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(Particle2_SIZE_X / 2, Particle2_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle2->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// ���_���W�̐ݒ�
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int Particle2Manager::SetParticle2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle2 = -1;

	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		if (!g_aParticle2[nCntParticle2].bUse2)
		{
			g_aParticle2[nCntParticle2].pos2 = pos;
			g_aParticle2[nCntParticle2].rot2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle2[nCntParticle2].scale2 = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
			g_aParticle2[nCntParticle2].move2 = move;
			g_aParticle2[nCntParticle2].col2 = col;
			g_aParticle2[nCntParticle2].fSizeX2 = fSizeX;
			g_aParticle2[nCntParticle2].fSizeY2 = fSizeY;
			g_aParticle2[nCntParticle2].nLife2 = nLife;
			g_aParticle2[nCntParticle2].bUse2 = true;

			// ���_���W�̐ݒ�
			SetVertexParticle2(nCntParticle2, fSizeX, fSizeY);

			nIdxParticle2 = nCntParticle2;


			break;
		}
	}

	return nIdxParticle2;
}
