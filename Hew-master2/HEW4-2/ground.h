
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void Ground_Initialize(void);
void Ground_Finalize(void);
void Ground_Update(void);
void Ground_Draw(void);

float SetGroundHit(int g_num);

HRESULT MakeVertexGround(int ground_num);
void MakeIdx(WORD *Idx, int g_num);
int Ground_Check();

class GROUND
{
public:
	D3DXMATRIX				g_mtxWorldGround;		// ワールドマトリックス
	D3DXVECTOR3				g_posGround;				// 地面の位置
	D3DXVECTOR3				g_rotGround;				// 地面の向き(回転)
	D3DXVECTOR3				g_sclGround;				// 地面の大きさ(スケール)

	void SetGround_Pos(int ground_num, int ground_type, D3DXVECTOR3 x_z_pos);//【第一引数：ブロック番号】【第二引数：ブロック種類】【第三引数：ブロック座標】


	int		ground_type;	//ブロックの種類

	//Max,Min座標
	D3DXVECTOR3	Point0;
	D3DXVECTOR3	Point1;
	D3DXVECTOR3	Point2;
	D3DXVECTOR3	Point3;

	GROUND() {};
	~GROUND() {};

	VERTEX_3D *LookVtx;//ポインタのポインタ
	int		  *LookIdxNum;//インデックスのポインタ
};


//ブロック生成関数
void Block0_Stage(VERTEX_3D *pvtx, int g_num);
void Block1_Stage(VERTEX_3D *pvtx, int g_num);
void Block2_Stage(VERTEX_3D *pvtx, int g_num);
void Block3_Stage(VERTEX_3D *pvtx, int g_num);
void Block4_Stage(VERTEX_3D *pvtx, int g_num);
void Block5_Stage(VERTEX_3D *pvtx, int g_num);
void Block6_Stage(VERTEX_3D *pvtx, int g_num);
void Block7_Stage(VERTEX_3D *pvtx, int g_num);
void Block8_Stage(VERTEX_3D *pvtx, int g_num);


//void Block_Stage(VERTEX_3D *pvtx, int g_num, D3DXVECTOR3 pos_algorithm, D3DXVECTOR3 texture);