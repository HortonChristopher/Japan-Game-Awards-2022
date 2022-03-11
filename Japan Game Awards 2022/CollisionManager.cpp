#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// 全ての組み合わせについて総当たりチェック Round-robin check for all combinations
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			// ともに球 Both spheres
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
		}
	}
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	// 操作用のイテレータ Iterator for operation
	std::forward_list<BaseCollider*>::iterator it;
	// 今までで最近いコライダーを記録するためのイテレータ Iterator to record the latest colliders so far
	std::forward_list<BaseCollider*>::iterator it_hit;
	// 今までで最近いコライダーの距離を記録する変数 A variable that records the distance of the latest colliders so far
	float distance = maxDistance;
	// 今までで最近いコライダーとの交点を記録する変数 A variable that records the intersection with the latest collider
	XMVECTOR inter;

	// 全てのコライダーと総当りチェック A variable that records the intersection with the latest collider
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* colA = *it;
		// 球の場合 In the case of a sphere
		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);
			float tempDistance;
			XMVECTOR tempInter;
			// 当たらなければ除外 Exclude if not hit
			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			// 距離が最小でなければ除外 Exclude if the distance is not the minimum
			if (tempDistance >= distance) continue;
			// 今までで最も近いので記録を取る Keep a record as it is the closest ever
			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	// 最後的になにかに当たっていたら結果を書き込む Write the result if something hits in the end
	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
		hitInfo->object = hitInfo->collider->GetObject3d();
	}

	return result;
}