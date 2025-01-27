#pragma once

class CEnemyBase;

// 敵管理クラス
class CEnemyManager
{
public:
	// 敵のインスタンス
	static CEnemyManager* Instance();
	// インスタンス削除
	static void ClearInstance();

	// 敵をリストに追加
	void Add(CEnemyBase* enemy);
	// 敵をリストから削除
	void Remove(CEnemyBase* enemy);

	CEnemyBase* FindLockOnTarget(float angle, float length) const;

private:
	// コンストラクタ
	CEnemyManager();
	// デストラクタ
	~CEnemyManager();

	// インスタンスのポインタ
	static CEnemyManager* spInstance;
	// 敵のリスト
	std::list<CEnemyBase*> mEnemies;

};
