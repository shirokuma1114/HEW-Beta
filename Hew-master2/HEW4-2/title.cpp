#include "input.h"
#include "main.h"
#include "scene.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>
#include "sprite.h"

static int color = 0;
static float size1 = 0.0f;
static bool size1_flag = true;
static bool color_flag = true;


void Title_Initialize(void)
{
}

void Title_Finalize(void)
{
}

void Title_Update(void)
{
	color+=2;
	if(size1_flag) size1 += 0.03f;
	else size1 -= 0.03f;
	if (size1 < 0.1f)size1_flag = true;
	if (size1 > 0.3f)size1_flag = false;

	if (keyboard.IsTrigger(DIK_SPACE)) {
		Scene_Change(SCENE_INDEX_GAME);
	}
}

void Title_Draw(void)
{
	// タイトルBG
	Sprite_Draw(TEXTURE_INDEX_BG, 0.0f, 0.0f);
	// タイトル文字
	Sprite_Draw(TEXTURE_INDEX_TITLE, 700.0f, 250.0f, 0.0f, 0.0f, 256.0f, 64.0f, 128.0f, 32.0f, 3.5f+size1, 3.5f + size1, 0.0f);
	// 透過
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255-color));
	// スタートを押して
	Sprite_Draw(TEXTURE_INDEX_PRESS, 500.0f, 420.0f, 0.0f, 0.0f, 256.0f, 64.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	// α値を戻す
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
}
