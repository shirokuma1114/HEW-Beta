
/*「Ground.cpp」=============================================
　・地面を作るファイルだよ

 　 制作概要：HEW2年次作品「ペダル」
   　　製作者：バイクのやつ　	(-"-)taiki okahara
	 =============================================================*/

#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include <iostream>
#include <time.h>
#include "ground.h"
#include <stdlib.h>
#include "player.h"

	 //*****************************************************************************
	 // マクロ定義
	 //*****************************************************************************
#define PLATE_SIZE		(30)						//プレートの大きさ

#define	PLATE_X_NUM		(15)						//プレートの数
#define PLATE_Z_NUM		(15)						//プレートの数
#define	PLATE_NUM		(PLATE_X_NUM)*(PLATE_Z_NUM)	//1ブロック内のプレートの総数

#define GROUND_NUM	(16)									//ブロックの総数
#define GROUND_X_SIZE		((PLATE_X_NUM) * (PLATE_SIZE))	//ブロックのXサイズ
#define GROUND_Z_SIZE		((PLATE_Z_NUM) * (PLATE_SIZE))	//ブロックのZサイズ
	 //*****************************************************************************
	 // グローバル変数
	 //*****************************************************************************
static LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffGround[GROUND_NUM];	// 頂点バッファへのポインタ
static LPDIRECT3DINDEXBUFFER9	g_pIdxBuffGround[GROUND_NUM];	// 頂点バッファへのポインタ

GROUND	ground[GROUND_NUM];	//グラウンドをGROUN_NUM分生成

							//GROUND	ground_zero;//草だけの当たりのない地面

int		PrimCount = (PLATE_X_NUM * PLATE_Z_NUM) * 2 + (PLATE_Z_NUM - 1) * 4;;	//ポリゴン数
int		NumIndex = (PLATE_X_NUM + 1) * 2 * PLATE_Z_NUM + (PLATE_Z_NUM - 1) * 2;	//頂点インデックス数
int		NumVertex = (PLATE_X_NUM + 1) * (PLATE_Z_NUM + 1);						//頂点数

//=============================================================================
// GROUNDクラス
//=============================================================================																			

//	[SetGround_Pos関数]	ブロックの初期設定をする関数
//【第一引数：ブロック番号】【第二引数：ブロック種類】【第三引数：ブロック座標】
void GROUND::SetGround_Pos(int ground_num, int g_type, D3DXVECTOR3 ground_pos) {


	LookVtx = new VERTEX_3D[NumVertex];	//　VRAMから持ってくるVtx情報の容量確保
	LookIdxNum = new int[NumIndex];		//	VRAMから持ってくるIdx情報の容量確保


										// 位置・回転・スケールの初期設定
	g_posGround = ground_pos;				//中心となる座標
	g_rotGround = D3DXVECTOR3(0, 0, 0.0f);	//回転角。基本的にはいじらない
	g_sclGround = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//大きさ

	ground_type = g_type;					//ブロックの種類を代入

	MakeVertexGround(ground_num);

}
//=============================================================================
// 初期化処理
//=============================================================================
void Ground_Initialize(void)
{
	//バッファポインタNULL初期化
	for (int i = 0; i < GROUND_NUM; i++)
	{
		g_pVtxBuffGround[i] = NULL;
		g_pIdxBuffGround[i] = NULL;
	}

	//ground生成
	ground[0].SetGround_Pos(0, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	ground[1].SetGround_Pos(1, 0, D3DXVECTOR3(0.0f, 0.0f, 450.0f));
	ground[2].SetGround_Pos(2, 0, D3DXVECTOR3(0.0f, 0.0f, 900.0f));
	ground[3].SetGround_Pos(3, 1, D3DXVECTOR3(0.0f, 0.0f, 1350.0f));//900


	ground[4].SetGround_Pos(4, 6, D3DXVECTOR3(450.0f, 0.0f, 1350.0f));
	ground[5].SetGround_Pos(5, 7, D3DXVECTOR3(900.0f, 90.0f, 1350.0f));

	ground[6].SetGround_Pos(6, 2, D3DXVECTOR3(900.0f, 90.0f, 1350.0f));

	ground[7].SetGround_Pos(7, 0, D3DXVECTOR3(1350.0f, 90.0f, 900.0f));
	ground[8].SetGround_Pos(8, 0, D3DXVECTOR3(1350.0f, 90.0f, 450.0f));
	ground[9].SetGround_Pos(9, 8, D3DXVECTOR3(1350.0f, 90.0f, 0.0f));


	ground[10].SetGround_Pos(10, 3, D3DXVECTOR3(900.0f, 0.0f, 0.0f));

	ground[11].SetGround_Pos(11, 5, D3DXVECTOR3(900.0f, 0.0f, -450.0f));
	ground[12].SetGround_Pos(12, 5, D3DXVECTOR3(450.0f, 0.0f, -450.0f));

	ground[13].SetGround_Pos(13, 4, D3DXVECTOR3(0.0f, 0.0f, -0.0f));

	ground[14].SetGround_Pos(14, 99, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
}

//=============================================================================
// 終了処理
//=============================================================================
void Ground_Finalize(void)
{

	for (int i = 0; i < GROUND_NUM; i++)
	{
		delete[] ground[i].LookVtx;
		delete[] ground[i].LookIdxNum;
		if (g_pIdxBuffGround[i] != NULL)
		{// 頂点バッファの開放
			g_pIdxBuffGround[i]->Release();
			g_pIdxBuffGround[i] = NULL;
		}

		if (g_pVtxBuffGround[i] != NULL)
		{// 頂点バッファの開放
			g_pVtxBuffGround[i]->Release();
			g_pVtxBuffGround[i] = NULL;
		}
	}
}

void Ground_Update(void) {
}

//=============================================================================
// 描画処理
//=============================================================================
void Ground_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	for (int i = 0; i < GROUND_NUM; i++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&ground[i].g_mtxWorldGround);

		// スケール反転
		D3DXMatrixScaling(&mtxScl, ground[i].g_sclGround.x, ground[i].g_sclGround.y, ground[i].g_sclGround.z);
		D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxScl);

		// 移動を反転
		D3DXMatrixTranslation(&mtxTranslate, ground[i].g_posGround.x, ground[i].g_posGround.y, ground[i].g_posGround.z);
		//D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxTranslate);

		// 回転を反転
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ground[i].g_rotGround.y, ground[i].g_rotGround.x, ground[i].g_rotGround.z);
		D3DXMatrixMultiply(&ground[i].g_mtxWorldGround, &ground[i].g_mtxWorldGround, &mtxRot);


		pDevice->SetTransform(D3DTS_WORLD, &ground[i].g_mtxWorldGround);
		pDevice->SetStreamSource(0, g_pVtxBuffGround[i], 0, sizeof(VERTEX_3D));

		//インデックスバッファのセット
		pDevice->SetIndices(g_pIdxBuffGround[i]);

		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_FIELD01));//テクスチャを変える場合はここの第二引数を変更する

		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertex, 0, PrimCount);
	}

}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexGround(int ground_num)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertex, //VRAMにメモリを作る
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGround[ground_num],
		NULL)))
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める

		VERTEX_3D *pVtx;


		//Lock,unLockをあまりしないのがパフォーマンスを上げる方法
		g_pVtxBuffGround[ground_num]->Lock(0, 0, (void**)&pVtx, 0); //VRAMにあったメモリをシステム上に持ってきてくれる


																	//ブロックの種類でswitch
		switch (ground[ground_num].ground_type)
		{
		case 0:
			Block0_Stage(pVtx, ground_num);//ブロック0を作成
			break;
		case 1:
			Block1_Stage(pVtx, ground_num);//ブロック1を作成
			break;
		case 2:
			Block2_Stage(pVtx, ground_num);//ブロック2を作成
			break;
		case 3:
			Block3_Stage(pVtx, ground_num);//ブロック3を作成
			break;
		case 4:
			Block4_Stage(pVtx, ground_num);//ブロック4を作成
			break;
		case 5:
			Block5_Stage(pVtx, ground_num);//ブロック5を作成
			break;
		case 6:
			Block6_Stage(pVtx, ground_num);//ブロック6を作成
			break;
		case 7:
			Block7_Stage(pVtx, ground_num);//ブロック7を作成
			break;
		case 8:
			Block8_Stage(pVtx, ground_num);//ブロック8を作成
			break;
		}
		g_pVtxBuffGround[ground_num]->Unlock(); //VRAMにメモリを返す(重い処理)
	}


	//ブロック座標の最大、最小値をPointに代入
	float min_Pos_x = ground[ground_num].g_posGround.x;
	float min_Pos_z = ground[ground_num].g_posGround.z;
	float max_Pos_x = ground[ground_num].g_posGround.x;
	float max_Pos_z = ground[ground_num].g_posGround.z;

	for (int i = 0; i < NumVertex; i++)
	{
		if (ground[ground_num].LookVtx[i].pos.x < min_Pos_x) {
			min_Pos_x = ground[ground_num].LookVtx[i].pos.x;
		}
		if (ground[ground_num].LookVtx[i].pos.z < min_Pos_z) {
			min_Pos_z = ground[ground_num].LookVtx[i].pos.z;
		}
		if (ground[ground_num].LookVtx[i].pos.x > max_Pos_x) {
			max_Pos_x = ground[ground_num].LookVtx[i].pos.x;
		}
		if (ground[ground_num].LookVtx[i].pos.z > max_Pos_z) {
			max_Pos_z = ground[ground_num].LookVtx[i].pos.z;
		}

	}
	/*
	1ブロックを上から見たとき（X-Z座標）の4頂点をPointとする

	Point1　　Point2
	　__________
	 |         |
	 |         |
	 |_________|
	 Point0　　Point3
	 */
	ground[ground_num].Point0 = D3DXVECTOR3(min_Pos_x, 0.0f, min_Pos_z);
	ground[ground_num].Point1 = D3DXVECTOR3(min_Pos_x, 0.0f, max_Pos_z);
	ground[ground_num].Point2 = D3DXVECTOR3(max_Pos_x, 0.0f, max_Pos_z);
	ground[ground_num].Point3 = D3DXVECTOR3(max_Pos_x, 0.0f, min_Pos_z);



	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * NumIndex, //VRAMにメモリを作る
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffGround[ground_num],
		NULL)))
	{
		return E_FAIL;
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		g_pIdxBuffGround[ground_num]->Lock(0, 0, (void**)&pIdx, 0);

		MakeIdx(pIdx, ground_num);

		g_pIdxBuffGround[ground_num]->Unlock();
	}

	return S_OK;
}

//=============================================================================
// インデックスの作成
//=============================================================================
void MakeIdx(WORD *Idx, int g_num) {

	int j = 0;
	int i = 0;
	for (int k = 0; k < PLATE_Z_NUM; k++)
	{

		for (j; j <= PLATE_X_NUM; j++)
		{
			Idx[(j * 2) + i] = (PLATE_X_NUM + 1) + j + (PLATE_X_NUM + 1) * k;
			ground[g_num].LookIdxNum[(j * 2) + i] = Idx[(j * 2) + i];
			Idx[(j * 2) + i + 1] = j + (PLATE_X_NUM + 1) * k;
			ground[g_num].LookIdxNum[(j * 2) + i + 1] = Idx[(j * 2) + i + 1];
		}
		if (k == PLATE_Z_NUM - 1)
		{
			break;
		}
		Idx[(j * 2) + i + 2] = Idx[(j * 2) + i + 2];
		ground[g_num].LookIdxNum[(j * 2) + i + 2] = j + (PLATE_X_NUM + 1) * k;
		Idx[(j * 2) + i + 3] = j + (PLATE_X_NUM + 2) + (PLATE_X_NUM + 1) * k;
		ground[g_num].LookIdxNum[(j * 2) + i + 3] = j + (PLATE_X_NUM + 2) + (PLATE_X_NUM + 1) * k;

		i += (2 * (PLATE_X_NUM + 2));
		j = 0;
	}
}

//=============================================================================
// プレイヤー接触ブロック判定
//=============================================================================
int Ground_Check() {

	D3DXVECTOR3 player_pos = GetPlayer_Pos();
	for (int i = 0; i < GROUND_NUM; i++)
	{
		//ブロックの最大、最小値の中にプレイヤーがいたら、当たり判定を計算する
		if (ground[i].Point0.x <= player_pos.x &&
			ground[i].Point2.x >= player_pos.x &&
			ground[i].Point0.z <= player_pos.z &&
			ground[i].Point2.z >= player_pos.z)
		{
			return i;//プレイヤーのいるブロックの番号を返す
		}
	}
	DebugProc_Print((char *)"接しているブロックはないよ\n");
	return 0;//エラー検知したほうがいいかも
}

//=============================================================================
// プレイヤーY軸補間（地面の上に載るようにする処理）
//=============================================================================
float SetGroundHit(int g_num) {


	D3DXVECTOR3 player_botom_position = GetPlayer_Pos();//現在のプレイヤーの位置;
	player_botom_position.y -= 15.0f;//プレイヤー底面

	D3DXVECTOR3 next_player_botom_position = D3DXVECTOR3(player_botom_position.x, 0.0f, player_botom_position.z);//二次元変換
	D3DXVECTOR3 vector1, vector2, vector3;//三角形の三辺をベクトル化


	D3DXVECTOR3 hosen1, hosen2, hosen3;
	for (int i = 0; i < NumIndex; i++)
	{
		if ((ground[g_num].LookIdxNum[i + 2]) > NumIndex || (ground[g_num].LookIdxNum[i + 2]) < 0)
		{
			//縮退ポリゴンのインデックスは飛ばす
			i += 3;
			continue;
		}


		vector1 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos);
		vector1.y = 0;

		vector2 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos);
		vector2.y = 0;

		vector3 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);
		vector3.y = 0;


		D3DXVec3Cross(&hosen1, &(next_player_botom_position -
			(D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x, 0.0f, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z))), &vector1);

		D3DXVec3Cross(&hosen2, &(next_player_botom_position -
			(D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.x, 0.0f, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos.z))), &vector2);



		//プレイヤーのいるポリゴン面を取得

		if (hosen1.y >= 0 && hosen2.y >= 0 || hosen1.y <= 0 && hosen2.y <= 0)
		{
			if (hosen1.y == 0 || hosen2 == 0)//頂点が「i」のときのは計算をしなくていいのでそのまま値を返す
			{
				return player_botom_position.y + 15.0f;
			}
			else
			{

				D3DXVec3Cross(
					&hosen3,
					&(next_player_botom_position - (D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.x, 0.0f, ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos.z))),
					&vector3);


				if (hosen2.y >= 0 && hosen3.y >= 0 || hosen2.y <= 0 && hosen3.y <= 0)
				{

					if (hosen3.y == 0)//頂点が「i」のときのは計算をしなくていいのでそのまま値を返す
					{
						return player_botom_position.y + 15.0f;
					}
					else {

						D3DXVECTOR3 naiseki1, naiseki2;
						naiseki1 = D3DXVECTOR3(
							player_botom_position.x - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x,
							player_botom_position.y - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.y,
							player_botom_position.z - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z);

						naiseki2 = D3DXVECTOR3(
							(player_botom_position.x + GetNormalPlayer_Pos().x) - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.x,
							(player_botom_position.y + GetNormalPlayer_Pos().y) - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.y,
							(player_botom_position.z + GetNormalPlayer_Pos().z) - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos.z);

						D3DXVECTOR3 n;//法線ベクトル
						D3DXVECTOR3 AB = GetNormalPlayer_Pos();

						//ベクトル再設定
						vector1 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 1]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);
						vector3 = D3DXVECTOR3(ground[g_num].LookVtx[ground[g_num].LookIdxNum[i + 2]].pos - ground[g_num].LookVtx[ground[g_num].LookIdxNum[i]].pos);



						D3DXVec3Cross(&n, &vector1, &vector3);//外積で面法線を計算

															  //地面より下にいたら地面上に再設置
						if ((D3DXVec3Dot(&naiseki2, &n)) > 0.00 && (D3DXVec3Dot(&naiseki1, &n)) <= 0.00 ||
							(D3DXVec3Dot(&naiseki2, &n)) <= 0.00 && (D3DXVec3Dot(&naiseki1, &n)) > 0.00) {


							player_botom_position.y += AB.y * (fabsf(D3DXVec3Dot(&n, &naiseki1) / (fabsf(D3DXVec3Dot(&n, &naiseki1) + fabsf(D3DXVec3Dot(&n, &naiseki2))))));

							return player_botom_position.y + 15.0f;
						}
						return player_botom_position.y + 15.0f;
					}
				}
			}
		}

	}
	return player_botom_position.y + 15.0f;
}


//=============================================================================
// ブロック生成関数
//=============================================================================
void Block0_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x, 0.0f *  (rand() % 10) + base_position.y, (PLATE_SIZE * z) + base_position.z);

			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * x) / 450, (30.0f * z) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}

}

void Block1_Stage(VERTEX_3D *pvtx, int g_num) {

	D3DXVECTOR3 base_position = ground[g_num].g_posGround;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{

			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-1 * sin(-(z* D3DX_PI / 30) + D3DX_PI / 2) * (PLATE_SIZE * x + 1) + base_position.x, 0.4f * x * z + base_position.y, cos(-(z* D3DX_PI / 30) + D3DX_PI / 2) * (PLATE_SIZE * x + 1) + base_position.z);



			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * x) / 450, (30.0f * z) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block2_Stage(VERTEX_3D *pvtx, int g_num) {

	D3DXVECTOR3 base_position = ground[g_num].g_posGround;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-1 * sin((z* D3DX_PI / 30) - D3DX_PI / 2) * ((PLATE_X_NUM - x) * PLATE_SIZE + 1) + base_position.x, 0.0f + base_position.y, cos((z* D3DX_PI / 30) - D3DX_PI / 2) * ((PLATE_X_NUM - x) * PLATE_SIZE + 1) + base_position.z);
			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * x) / 450, (30.0f * z) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block3_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-1 * sin((z* D3DX_PI / 30) - D3DX_PI) * ((PLATE_X_NUM - x) * PLATE_SIZE + 1) + base_position.x, 0.0f + base_position.y, cos((z* D3DX_PI / 30) - D3DX_PI) * ((PLATE_X_NUM - x) * PLATE_SIZE + 1) + base_position.z);


			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * (PLATE_X_NUM - x)) / 450, (30.0f * (PLATE_Z_NUM - z)) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block4_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-1 * sin(-(z* D3DX_PI / 30) + D3DX_PI) * (PLATE_SIZE * x + 1) + base_position.x, 0.0f + base_position.y, cos(-(z* D3DX_PI / 30) - D3DX_PI) * (PLATE_SIZE * x + 1) + base_position.z);


			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * x) / 450, (30.0f * z) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block5_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x, 0.0f + base_position.y, (PLATE_SIZE * z) + base_position.z);
			/*if (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1) < 5)
			{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos.y = (5 - (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1))) * 10;
			}*/

			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * (PLATE_Z_NUM - z)) / 450, (30.0f * x) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block6_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x, 6.0f * z + base_position.y, (PLATE_SIZE * z) + base_position.z);
			/*if (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1) < 5)
			{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos.y = (5 - (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1))) * 10;
			}*/

			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * z) / 450, (30.0f * x) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block7_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x, -0.4f * (PLATE_Z_NUM - z) * x + base_position.y, (PLATE_SIZE * z) + base_position.z);
			/*if (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1) < 5)
			{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos.y = (5 - (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1))) * 10;
			}*/

			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * (PLATE_Z_NUM - z)) / 450, (30.0f * (PLATE_X_NUM - x)) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}

void Block8_Stage(VERTEX_3D *pvtx, int g_num) {
	srand((unsigned int)time(NULL));
	D3DXVECTOR3 base_position = ground[g_num].g_posGround;
	int katamuki_hani = 5;

	for (int z = 0; z < PLATE_Z_NUM + 1; z++)
	{
		for (int x = 0; x < PLATE_X_NUM + 1; x++)
		{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos = D3DXVECTOR3(-(PLATE_SIZE * x) + base_position.x, (PLATE_Z_NUM - z) * 5.0f * -1.0f + base_position.y, (PLATE_SIZE * z) + base_position.z);
			/*if (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1) < 5)
			{
			pvtx[(PLATE_X_NUM + 1)* z + x].pos.y = (5 - (((PLATE_X_NUM + 1)* z + x) % (PLATE_X_NUM + 1))) * 10;
			}*/

			pvtx[(PLATE_X_NUM + 1)* z + x].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pvtx[(PLATE_X_NUM + 1)* z + x].tex = D3DXVECTOR2((30.0f * x) / 450, (30.0f * z) / 450);

			ground[g_num].LookVtx[(PLATE_X_NUM + 1)* z + x] = pvtx[(PLATE_X_NUM + 1)* z + x];
		}
	}
}
