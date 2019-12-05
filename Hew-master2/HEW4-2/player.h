#pragma once

#include "main.h"

#define GRAVITY		(2.0f)			// 重力
#define MAXSPEED	(200.0f)		// 移動量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
D3DXVECTOR3 GetPlayer_Pos();
D3DXVECTOR3 GetPlayer_Rot();
D3DXVECTOR3 GetNormalPlayer_Pos();
float GetMeter_Pos();
