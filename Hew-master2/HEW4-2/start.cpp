#include "Start.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VALUE_MOVE		(1.0f)							// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// 回転量

#define	Start_WIDTH		(15.0f)						// 地面の幅(X方向)
#define	Start_DEPTH		(15.0f)						// 地面の奥行(Z方向)
#define	Start_HEIGHT	(15.0f)						// 地面の高さ(Y方向)

#define Start_ANGLE		(8)
#define Start_NUM		(5)

#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}//安全に解放する
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;	// 頂点バッファへのポインタ

D3DXMATRIX				g_mtxWorldStart;		// ワールドマトリックス
D3DXVECTOR3				g_posStart;				// 地面の位置
D3DXVECTOR3				g_rotStart;				// 地面の向き(回転)
D3DXVECTOR3				g_sclStart;				// 地面の大きさ(スケール)
bool					isUse=true;				// スタートの使用判定


static bool Hit = false;

static LPDIRECT3DDEVICE9		pDevice;
static DWORD					dwNumMaterials = 0;
static LPD3DXMESH				pMesh = NULL;				//メッシュ
static D3DMATERIAL9*			pMaterials = NULL;			//マテリアル
static LPDIRECT3DTEXTURE9*		pTextures = NULL;			//テクスチャ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void Collision_Start();

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Start_Initialize(void)
{
	pDevice = GetD3DDevice();

	// Xファイルからメッシュをロードする 
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("start.x", D3DXMESH_SYSTEMMEM,
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

	g_posStart = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	g_rotStart = D3DXVECTOR3(D3DX_PI / 6, 0.0f, 0.0f);
	g_sclStart = D3DXVECTOR3(15.0f, 15.0f, 15.0f);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Start_Finalize(void)
{
	if (g_pVtxBuffStart != NULL)
	{// 頂点バッファの開放
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pDevice);
}

//=============================================================================
// 更新処理
//=============================================================================
void Start_Update(void)
{
	Collision_Start();
}

//=============================================================================
// 描画処理
//=============================================================================
void Start_Draw(void)
{
	//デバイスが無いなら終了
	if (NULL == pDevice) { return; }

	D3DXMATRIXA16 mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldStart);

	// スケール反転
	D3DXMatrixScaling(&mtxScl, g_sclStart.x, g_sclStart.y, g_sclStart.z);
	D3DXMatrixMultiply(&g_mtxWorldStart, &g_mtxWorldStart, &mtxScl);

	// 回転を反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotStart.y, g_rotStart.x, g_rotStart.z);
	D3DXMatrixMultiply(&g_mtxWorldStart, &g_mtxWorldStart, &mtxRot);

	// 移動を反転
	D3DXMatrixTranslation(&mtxTranslate, g_posStart.x, g_posStart.y, g_posStart.z);
	D3DXMatrixMultiply(&g_mtxWorldStart, &g_mtxWorldStart, &mtxTranslate);

	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldStart);

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
}

// スタートの当たり判定
void Collision_Start()
{
	D3DXVECTOR3 Player = GetPlayer_Pos();
	D3DXVECTOR3 dst1(Player.x, Player.y, Player.z);

	Hit = false;
	D3DXVECTOR3 dst2(g_posStart.x, g_posStart.y, g_posStart.z);
	D3DXVECTOR3 distance;
	distance = dst2 - dst1;
	float length = D3DXVec3Length(&distance);
	float size = 15;
	if (length < size)
	{
		Hit = true;
		isUse = false;
		return;

	}
	return;
}

bool Get_StartHit()
{
	return Hit;
}