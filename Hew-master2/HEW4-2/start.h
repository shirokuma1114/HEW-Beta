#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Start_Initialize(void);
void Start_Finalize(void);
void Start_Update(void);
void Start_Draw(void);

bool Get_StartHit();