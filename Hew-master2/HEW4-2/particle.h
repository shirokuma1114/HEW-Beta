//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 
//
//=============================================================================
#pragma once


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class ParticleManager
{
public:
	HRESULT InitParticle(void);
	void UninitParticle(void);
	void UpdateParticle(void);
	void DrawParticle(void);

	int SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife);
};
