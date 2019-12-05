#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

void ChangeScore(int nValue);
void ResetScore(int nScore = 0);
