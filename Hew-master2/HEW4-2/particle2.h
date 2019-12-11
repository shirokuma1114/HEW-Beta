//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class Particle2Manager
{
public:
	HRESULT InitParticle2(void);
	void UninitParticle2(void);
	void UpdateParticle2(void);
	void DrawParticle2(void);

	int SetParticle2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);
};
