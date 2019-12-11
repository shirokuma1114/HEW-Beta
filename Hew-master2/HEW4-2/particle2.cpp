//=============================================================================
//
// パーティクル処理 [Particle2.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "myDirect3D.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "Particle2.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_Particle2	"asset/texture/effect000.jpg"	// 読み込むテクスチャファイル名
#define	Particle2_SIZE_X		(50.0f)							// ビルボードの幅
#define	Particle2_SIZE_Y		(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_Particle2	(5.0f)							// 移動速度

#define	MAX_Particle2			(1000)						// ビルボード最大数


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos2;		// 位置
	D3DXVECTOR3 rot2;		// 回転
	D3DXVECTOR3 scale2;		// スケール
	D3DXVECTOR3 move2;		// 移動量
	D3DXCOLOR col2;			// 色
	float fSizeX2;			// 幅
	float fSizeY2;			// 高さ
	int nIdxShadow2;			// 影ID
	int nLife2;				// 寿命
	bool bUse2;				// 使用しているかどうか
} PARTICLE2;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY);
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle2 = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParticle2 = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldParticle2;				// ワールドマトリックス

PARTICLE2				g_aParticle2[MAX_Particle2];		// パーティクルワーク
D3DXVECTOR3				g_posBase2;						// ビルボード発生位置
float					g_fWidthBase2 = 5.0f;			// 基準の幅
float					g_fHeightBase2 = 10.0f;			// 基準の高さ
bool					g_bPause2 = false;				// ポーズON/OFF
float					g_roty2 = 0.0f;					// 移動方向
float					g_spd2 = 0.0f;					// 移動スピード

bool					g_player_touch2 = true;			// プレイヤーにくっつく

														//=============================================================================
														// 初期化処理
														//=============================================================================
HRESULT Particle2Manager::InitParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexParticle2(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_Particle2,			// ファイルの名前
		&g_pD3DTextureParticle2);	// 読み込むメモリー

	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		g_aParticle2[nCntParticle2].pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2[nCntParticle2].rot2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2[nCntParticle2].scale2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].move2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].col2 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].fSizeX2 = Particle2_SIZE_X;
		g_aParticle2[nCntParticle2].fSizeY2 = Particle2_SIZE_Y;
		g_aParticle2[nCntParticle2].nIdxShadow2 = -1;
		g_aParticle2[nCntParticle2].nLife2 = 0;
		g_aParticle2[nCntParticle2].bUse2 = false;
	}

	g_posBase2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_roty2 = 0.0f;
	g_spd2 = 0.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Particle2Manager::UninitParticle2(void)
{
	if (g_pD3DTextureParticle2 != NULL)
	{// テクスチャの開放
		g_pD3DTextureParticle2->Release();
		g_pD3DTextureParticle2 = NULL;
	}

	if (g_pD3DVtxBuffParticle2 != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffParticle2->Release();
		g_pD3DVtxBuffParticle2 = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Particle2Manager::UpdateParticle2(void)
{
	// 更新処理
	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		// 使用中
		if (g_aParticle2[nCntParticle2].bUse2)
		{

			// 寿命が減る
			g_aParticle2[nCntParticle2].nLife2-=2;

			// 寿命がなくなった時
			if (g_aParticle2[nCntParticle2].nLife2 <= 0)
			{
				// 表示されなくなる
				g_aParticle2[nCntParticle2].bUse2 = false;
				// ReleaseShadow(g_aParticle2[nCntParticle2].nIdxShadow);
				// 影の数を減らす
				g_aParticle2[nCntParticle2].nIdxShadow2 = -1;
			}
			else
			{
				// α値設定
				g_aParticle2[nCntParticle2].col2.a -= 0.05f;
				// サイズ設定
				g_aParticle2[nCntParticle2].fSizeX2 -= 2.0f;
				g_aParticle2[nCntParticle2].fSizeY2 -= 2.0f;
				if (g_aParticle2[nCntParticle2].col2.a < 0.0f)
				{
					g_aParticle2[nCntParticle2].col2.a = 0.0f;
				}

				// 色の設定
				SetColorParticle2(nCntParticle2, g_aParticle2[nCntParticle2].col2);
			}
		}
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void Particle2Manager::DrawParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;
	CAMERA *cam = GetCamera();

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);				// Zバッファーの書き込みをしない
																	//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		if (g_aParticle2[nCntParticle2].bUse2)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParticle2);

			// ビューマトリックスを取得
			mtxView = cam->mtxView;

			g_mtxWorldParticle2._11 = mtxView._11;
			g_mtxWorldParticle2._12 = mtxView._21;
			g_mtxWorldParticle2._13 = mtxView._31;
			g_mtxWorldParticle2._21 = mtxView._12;
			g_mtxWorldParticle2._22 = mtxView._22;
			g_mtxWorldParticle2._23 = mtxView._32;
			g_mtxWorldParticle2._31 = mtxView._13;
			g_mtxWorldParticle2._32 = mtxView._23;
			g_mtxWorldParticle2._33 = mtxView._33;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aParticle2[nCntParticle2].scale2.x, g_aParticle2[nCntParticle2].scale2.y, g_aParticle2[nCntParticle2].scale2.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aParticle2[nCntParticle2].pos2.x, g_aParticle2[nCntParticle2].pos2.y, g_aParticle2[nCntParticle2].pos2.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle2);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle2, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureParticle2);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle2 * 4), NUM_POLYGON);
		}
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zバッファーの書き込みをする
																	//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);				// Z比較あり

																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);	// アルファブレンディング処理
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// 最初のアルファ引数
																	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ２番目のアルファ引数
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_Particle2,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffParticle2,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-Particle2_SIZE_X / 2, -Particle2_SIZE_Y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(Particle2_SIZE_X / 2, -Particle2_SIZE_Y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-Particle2_SIZE_X / 2, Particle2_SIZE_Y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(Particle2_SIZE_X / 2, Particle2_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle2->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// 頂点座標の設定
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int Particle2Manager::SetParticle2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle2 = -1;

	for (int nCntParticle2 = 0; nCntParticle2 < MAX_Particle2; nCntParticle2++)
	{
		if (!g_aParticle2[nCntParticle2].bUse2)
		{
			g_aParticle2[nCntParticle2].pos2 = pos;
			g_aParticle2[nCntParticle2].rot2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle2[nCntParticle2].scale2 = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
			g_aParticle2[nCntParticle2].move2 = move;
			g_aParticle2[nCntParticle2].col2 = col;
			g_aParticle2[nCntParticle2].fSizeX2 = fSizeX;
			g_aParticle2[nCntParticle2].fSizeY2 = fSizeY;
			g_aParticle2[nCntParticle2].nLife2 = nLife;
			g_aParticle2[nCntParticle2].bUse2 = true;

			// 頂点座標の設定
			SetVertexParticle2(nCntParticle2, fSizeX, fSizeY);

			nIdxParticle2 = nCntParticle2;


			break;
		}
	}

	return nIdxParticle2;
}
