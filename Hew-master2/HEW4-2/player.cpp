
#include "player.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "camera.h"
#include "drink.h"
#include "ground.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_ROTATE		(D3DX_PI * 0.008f)			// 回転量

#define	Player_WIDTH		(15.0f)						// 地面の幅(X方向)
#define	Player_DEPTH		(15.0f)						// 地面の奥行(Z方向)
#define	Player_HEIGHT		(15.0f)						// 地面の高さ(Y方向)

#define Player_ANGLE		(8)

#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}//安全に解放する

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetPlayer_Pos(float set);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffPlayer = NULL;	// 頂点バッファへのポインタ

D3DXMATRIX						g_mtxWorldPlayer;			// ワールドマトリックス
D3DXVECTOR3						g_posPlayer;				// プレイヤーの位置
D3DXVECTOR3						g_rotPlayer;				// プレイヤーの向き(回転)
D3DXVECTOR3						g_sclPlayer;				// プレイヤーの大きさ(スケール)
D3DXVECTOR3						g_normalPlayer;				// プレイヤーの法線

float							accel = 0.0f;				// 加速の数値
float							item = 0.0f;				// アイテムによっての加速の数値
float							drift = 0.0f;				// ドリフトによっての加速の数値
float							drift_count = 0.0f;			// ドリフトカウント
float							drift_rot = 0.0f;			// ドリフト時の傾き
float							meter_speed = 0.0f;			// メーター用の変数

static LPDIRECT3DDEVICE9		pDevice;
static DWORD					dwNumMaterials = 0;
static LPD3DXMESH				pMesh = NULL;				//メッシュ
static D3DMATERIAL9*			pMaterials = NULL;			//マテリアル
static LPDIRECT3DTEXTURE9*		pTextures = NULL;			//テクスチャ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Player_Initialize(void)
{
	pDevice = GetD3DDevice();

	// Xファイルからメッシュをロードする 
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("player.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMaterials = new D3DMATERIAL9[dwNumMaterials];
	pTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pMaterials[i] = d3dxMaterials[i].MatD3D;
		pMaterials[i].Ambient = pMaterials[i].Diffuse;
		pTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();
	
	// 位置・回転・スケールの初期設定
	g_posPlayer = D3DXVECTOR3(-150.0f, 100.0f, 10.0f);
	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclPlayer = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
	g_normalPlayer= D3DXVECTOR3(0.0f, 100.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Player_Finalize(void)
{
	if (g_pVtxBuffPlayer != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pDevice);
}

//=============================================================================
// 更新処理
//=============================================================================
void Player_Update(void)
{
	// 重力
	g_posPlayer.y -= GRAVITY;

	// ↑key
	if (joycon[0].GetAccelerometer()>600|| joycon[0].GetAccelerometer()<-600)
	{
		// 加速
		if (accel < MAXSPEED) accel += 10.0f;
	}
	else
	{
		// 加速していないとき徐々にスピードが落ちる
		if (accel > 0) accel -= 5.0f;
		if (accel < 0) accel += 5.0f;
	}
	// 走っているとき
	if(accel+drift+item != 0)
	{
		// ←key
		if (joycon[1].IsPress(JOYCON_Y))
		{
			// Y軸左回転
			g_rotPlayer.y -= VALUE_ROTATE;
			// Z軸の回転
			g_rotPlayer.z += VALUE_ROTATE;
			// ドリフト時の傾き
			drift_rot -= VALUE_ROTATE;
			// ドリフトカウント
			drift_count += 1.0f;
		}
		// →key
		else if (joycon[1].IsPress(JOYCON_A))
		{
			// Y軸右回転
			g_rotPlayer.y += VALUE_ROTATE;
			// Z軸の回転
			g_rotPlayer.z -= VALUE_ROTATE;
			// ドリフト時の傾き
			drift_rot += VALUE_ROTATE;
			// ドリフトカウント
			drift_count += 1.0f;
		}
		else
		{
			// ドリフトのたまり具合で加速が決まる
			if (drift_count > 40.0f) drift = 100.0f;
			if (drift_count > 80.0f) drift = 200.0f;
			drift_count = 0.0f;
		}

		// 回転制御(Z軸)
		if (g_rotPlayer.z > D3DX_PI / Player_ANGLE)
		{
			g_rotPlayer.z = D3DX_PI / Player_ANGLE;
		}
		if (g_rotPlayer.z < -D3DX_PI / Player_ANGLE)
		{
			g_rotPlayer.z = -D3DX_PI / Player_ANGLE;
		}

		// 回転の制御(y軸)
		if (g_rotPlayer.y > D3DX_PI)
		{
			g_rotPlayer.y -= D3DX_PI * 2.0f;
		}
		if (g_rotPlayer.y < -D3DX_PI)
		{
			g_rotPlayer.y += D3DX_PI * 2.0f;
		}

		// 回転の制御(ドリフト)
		if (drift_rot > D3DX_PI / Player_ANGLE)
		{
			drift_rot = D3DX_PI / Player_ANGLE;
		}
		if (drift_rot < -D3DX_PI / Player_ANGLE)
		{
			drift_rot = -D3DX_PI / Player_ANGLE;
		}
	}

	// 移動
	g_posPlayer.x += sinf(g_rotPlayer.y) * (accel + item + drift) / 20;
	g_posPlayer.z += cosf(g_rotPlayer.y) * (accel + item + drift) / 20;

	// メーター変数
	meter_speed = accel;

	// アクセルのズレを修正する
	if (accel<5.0f && accel>-5.0f) accel = 0.0f;

	// ドリフト減速
	if (drift != 0.0f) drift -= 5.0f;

	// ドリフトズレを修正
	if (drift<1.0f&&drift>-1.0f) drift = 0.0f;

	// エナジースタートに当たった時
	if (Get_DrinkHit()) item += 100.0f;
	else if (item != 0.0f) item -= 5.0f;

	// アイテムズレを修正
	if (item<1.0f&&item>-1.0f) item = 0.0f;

	// z軸の傾きを0に収束
	if (g_rotPlayer.z != 0.0f)
	{
		if (g_rotPlayer.z > 0) g_rotPlayer.z -= VALUE_ROTATE * 0.7f;
		if (g_rotPlayer.z < 0) g_rotPlayer.z += VALUE_ROTATE * 0.7f;
	}

	// ドリフト時の傾きを0に収束
	if (drift_rot != 0.0f)
	{
		if (drift_rot > 0) drift_rot -= VALUE_ROTATE * 0.7f;
		if (drift_rot < 0) drift_rot += VALUE_ROTATE * 0.7f;
	}

	// グラウンド当たり判定
	SetPlayer_Pos(SetGroundHit(Ground_Check()));

	// リセット
	if (keyboard.IsPress(DIK_RETURN))
	{
		g_posPlayer.x = -150.0f;
		g_posPlayer.y = 100.0f;
		g_posPlayer.z = 10.0f;

		g_rotPlayer.x = 0.0f;
		g_rotPlayer.y = 0.0f;
		g_rotPlayer.z = 0.0f;
	}
	/*
	DebugProc_Print((char *)"*** ３Ｄポリゴン操作 ***\n");
	DebugProc_Print((char *)"位置 [%f : %f : %f]\n", g_posPlayer.x, g_posPlayer.y, g_posPlayer.z);
	DebugProc_Print((char *)"前移動 : ↑\n");
	DebugProc_Print((char *)"後移動 : ↓\n");
	DebugProc_Print((char *)"左移動 : ←\n");
	DebugProc_Print((char *)"右移動 : →\n");
	DebugProc_Print((char *)"%d",joycon[0].GetAccelerometer());

	DebugProc_Print((char *)"\n");

	DebugProc_Print((char *)"向き [%f : %f : %f]\n", g_rotPlayer.x, g_rotPlayer.y, g_rotPlayer.z);
	DebugProc_Print((char *)"\n");

	DebugProc_Print((char *)"ドリフトカウント[%f]", drift_count);
	DebugProc_Print((char *)"\n");


	DebugProc_Print((char *)"位置・向きリセット : ENTER\n");
	*/
}

//=============================================================================
// 描画処理
//=============================================================================
void Player_Draw(void)
{
	//デバイスが無いなら終了
	if (NULL == pDevice) { return; }

	D3DXMATRIXA16 mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPlayer);

	// スケール反転
	D3DXMatrixScaling(&mtxScl, g_sclPlayer.x, g_sclPlayer.y, g_sclPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxScl);

	// 回転を反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPlayer.y+drift_rot, g_rotPlayer.x, g_rotPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);
	
	// 移動を反転
	D3DXMatrixTranslation(&mtxTranslate, g_posPlayer.x, g_posPlayer.y, g_posPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTranslate);

	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

	//頂点シェーダをセット(使わない場合はNULL)
	GetD3DDevice()->SetVertexShader(NULL);

	//頂点フォーマットをセット
	GetD3DDevice()->SetFVF(pMesh->GetFVF());

	//ここに処理内容を書く
	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pDevice->SetMaterial(&pMaterials[i]);
		pDevice->SetTexture(0, pTextures[i]);
		pMesh->DrawSubset(i);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00aaaaaa);	// 環境光を設定
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);// 鏡面反射有効
	//pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);	// Zバッファーを有効
}

//-----------------------------------------------------------------------------
// ゲッター
//-----------------------------------------------------------------------------
D3DXVECTOR3 GetPlayer_Pos()
{
	return g_posPlayer;
}
D3DXVECTOR3 GetPlayer_Rot()
{
	return g_rotPlayer;
}

D3DXVECTOR3 GetNormalPlayer_Pos()
{
	return g_normalPlayer;
}

void SetPlayer_Pos(float set) 
{
	g_posPlayer.y = set;
}

float GetMeter_Pos()
{
	return meter_speed;
}