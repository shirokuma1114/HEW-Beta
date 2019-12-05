
#include "sprite.h"
#include "input.h"
#include "scene.h"
#include "texture.h"
#include "fade.h"


void Result_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void Result_Finalize(void)
{
}

void Result_Update(void)
{
	if (keyboard.IsTrigger(DIK_SPACE)) 
	{
		Scene_Change(SCENE_INDEX_TITLE);
	}

	if (keyboard.IsTrigger(DIK_G))
	{
		Scene_Change(SCENE_INDEX_GAME);
	}
}

void Result_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f);
}
