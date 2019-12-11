#pragma once

#include "main.h"


//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
} CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void Camera_Initialize(void);
void Camera_Finalize(void);
void Camera_Update(void);

void Camera_SetCamera(void);

CAMERA *GetCamera(void);