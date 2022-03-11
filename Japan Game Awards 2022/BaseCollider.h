#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "CollisionInfo.h"

/// <summary>
/// コライダー基底クラス Collider base class
/// </summary>
class BaseCollider
{
public:
	BaseCollider() = default;
	// 仮想デストラクタ Virtual destructor
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d* object) {
		this->object3d = object; }

	inline Object3d* GetObject3d() {
		return object3d;
	}

	/// <summary>
	/// 衝突時コールバック関数 Collision callback function
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

	/// <summary>
	/// 更新 update
	/// </summary>
	virtual void Update() = 0;
	// 形状タイプ取得 Shape type acquisition
	inline CollisionShapeType GetShapeType() { return shapeType; }

protected:
	Object3d* object3d = nullptr;
	// 形状タイプ Shape type
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};