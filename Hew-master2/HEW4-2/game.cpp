#include <time.h>
#include "myDirect3D.h"
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "texture.h"
#include "sprite.h"
#include "score.h"
#include "meter.h"
#include "ground.h"
#include "scene.h"
#include "fade.h"
#include "Player.h"
#include "grid.h"
#include "drink.h"
#include "start.h"


typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_IN,
	PHASE_INDEX_PLAYER_MUTEKI,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};

static PHASE_INDEX g_Phase;
static int g_GameFrameCount;

static int g_Score = 0;// 点数
static int g_KillCount = 0;


static bool Game_EndCheck(void);



void Game_Initialize(void)
{
	// カメラの初期化処理
	Camera_Initialize();
	//Camera2_Initialize();

	// ライトの初期化処理
	Light_Initialize();

	//グリッドの初期化処理
	Grid_Initialize();

	// プレイヤー
	Player_Initialize();

	// グラウンド
	Ground_Initialize();

	// スタート
	//Start_Initialize();

	// ドリンク
	Drink_Initialize();

	// スコア
	InitScore();

	// メーター
	Meter_Initialize();
}

void Game_Finalize(void)
{
	// カメラの終了処理
	Camera_Finalize();
	//Camera2_Finalize();

	// ライトの終了処理
	Light_Finalize();

	// グリッドの終了
	Grid_Finalize();

	// プレイヤー
	Player_Finalize();

	// グラウンド
	Ground_Finalize();

	// スタート
	//Start_Finalize();

	// ドリンク
	Drink_Finalize();

	// スコア
	UninitScore();

	// メーター
	Meter_Finalize();
}

void Game_Update(void)
{
	// カメラの更新処理
	Camera_Update();
	//Camera2_Update();

	// ライトの更新処理
	Light_Update();

	//グリッド線
	Grid_Update();

	// プレイヤー
	Player_Update();

	// グラウンド
	Ground_Update();

	// スタート
	//Start_Update();

	// ドリンク
	Drink_Update();

	// スコア
	UpdateScore();	

	// メーター
	Meter_Update();

	if (keyboard.IsTrigger(DIK_SPACE))
	{
		Scene_Change(SCENE_INDEX_RESULT);
	}

}

void Game_Draw(void)
{
	// カメラの設定
	Camera_SetCamera();
	//Camera2_SetCamera();

	//グリッドの描画
	Grid_Draw();

	// プレイヤー
	Player_Draw();

	// グラウンド
	Ground_Draw();

	// スタート
	//Start_Draw();

	// ドリンク
	Drink_Draw();

	// スコア
	DrawScore();

	// メーター
	Meter_Draw();
}



bool Game_EndCheck(void)
{

}



void Game_AddScore(int score)
{
	//g_Score += score;
}

void Game_AddKillCount()
{
	//g_KillCount += 1;
}
