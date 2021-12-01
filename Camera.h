//=============================================================================
//
// カメラ クラス [Camera.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "main.h"

class CCamera
{
private:
	XMFLOAT3 m_vPos;				// 視点
	XMFLOAT3 m_vTarget;				// 注視点
	XMFLOAT3 m_vUp;					// 上方ベクトル

	XMFLOAT3 m_vSrcPos;				// 元の視点

	XMFLOAT3 m_vDestPos;			// 視点の目的位置
	XMFLOAT3 m_vDestTarget;			// 注視点の目的位置
	XMFLOAT3 m_vVelocity;			// 移動量

	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	XMFLOAT4X4 m_mtxWorld;			// ワールド マトリックス
	XMFLOAT4X4 m_mtxView;			// ビュー マトリックス
	XMFLOAT4X4 m_mtxProj;			// プロジェクション マトリックス

	XMFLOAT3 m_vAngle;				// カメラの角度
	XMFLOAT3 m_vDestAngle;			// カメラの目的の向き
	XMFLOAT3 m_vDestTargetAngle;	// 注視点の目的の向き
	float m_fLengthInterval;		// カメラの視点と注視点の距離

	static CCamera* m_pCamera;		// 現在のカメラ

public:
	CCamera();

	void Init();
	void Update();

	void SetPos(XMFLOAT3& vPos) { m_vPos = vPos; }
	XMFLOAT3& GetPos() { return m_vPos; }
	void SetPos(float x, float y, float z) { m_vPos.x = x, m_vPos.y = y, m_vPos.z = z; }
	void SetTarget(XMFLOAT3& vTarget) { m_vTarget = vTarget; }
	void SetTarget(float x, float y, float z) { m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z; }
	void SetUpVector(XMFLOAT3& vUp) { m_vUp = vUp; }
	void SetUpVector(float x, float y, float z) { m_vUp.x = x, m_vUp.y = y, m_vUp.z = z; }
	void SetWorldMatrix(XMFLOAT4X4& mtxWorld);
	XMFLOAT4X4& GetWorldMatrix() { return m_mtxWorld; }
	XMFLOAT4X4& CalcWorldMatrix();
	void SetFovY(float fFovY) { m_fFovY = fFovY; }
	void SetAspectRatio(float fAspect) { m_fAspectRatio = fAspect; }
	void SetRangeZ(float fNearZ, float fFarZ) { m_fNearZ = fNearZ, m_fFarZ = fFarZ; }
	void UpdateMatrix();
	XMFLOAT4X4& GetViewMatrix() { return m_mtxView; }
	XMFLOAT4X4& GetProjMatrix() { return m_mtxProj; }
	XMFLOAT3& GetAngle() { return m_vAngle; }

	static CCamera* Get() { return m_pCamera; }
	static void Set(CCamera* pCamera) { m_pCamera = pCamera; }
};
