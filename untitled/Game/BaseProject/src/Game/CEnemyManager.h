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

	const std::list<CEnemyBase*>& GetEnemies();


private:
	// コンストラクタ
	CEnemyManager();
	// デストラクタ
	~CEnemyManager();

	// インスタンスのポインタ
	static CEnemyManager* spInstance;
	// 敵のリスト
	std::list<CEnemyBase*> mEnemies;
	// 現在出現している敵のリスト
	const std::list<CEnemyBase*> mCurreEnemies;

};
