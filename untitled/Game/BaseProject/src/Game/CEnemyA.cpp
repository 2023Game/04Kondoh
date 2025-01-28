#include "CEnemyA.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CDebugFieldOfView.h"
#include "CPlayer.h"
#include "Maths.h"
#include "Primitive.h"
#include "CField.h"
#include "CNavNode.h"
#include "CNavManager.h"

#define ANIM_PATH "Character\\EnemyA\\"

#define ENEMY_HEIGHT 16.0f      // 敵の高さ
#define ENEMY_SOLE   5.0f       // 敵の底
#define ENEMY_WIDTH  5.0f       // 敵の半径
#define FOV_ANGLE 45.0f         // 視野範囲の角度
#define FOV_LENGTH 100.0f       // 視野範囲の距離
#define EYE_HEIGHT 10.0f        // 視点の高さ
#define WALK_SPEED 10.0f        // 歩きの速度
#define RUN_SPEED 20.0f         // 走っている時の速度
#define ROTATE_SPEED 6.0f       // 回転速度
#define ATTACK_RANGE 20.0f      // 攻撃範囲
#define ATTACK_RANGE_MIN 12.0f  // 最小攻撃範囲
#define ATTACK_MOVE_DIST 8.0f   // 攻撃時の移動距離
#define ATTACK_MOVE_START 12.0f // 攻撃時の移動開始フレーム
#define ATTACK_MOVE_END 24.0f   // 攻撃時の移動終了フレーム
#define ATTACK_WAIT_TIME 1.0f   // 攻撃終了時の待ち時間
#define PATROL_INTERVAL 3.0f    // 次の巡回に移動開始するまでの時間
#define PATROL_NEAR_DIST 10.0f  // 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME 5.0f          // 待機状態の時間

// プレイヤーのアニメーションデータのテーブル
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",								true,	0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle.x",		        true,	126.0f,	1.0f},	// 待機
	{ ANIM_PATH"walk.x",		        true,	42.0f,	1.0f},	// 歩行
	{ ANIM_PATH"Run.x",					true,	23.0f,	1.0f},	// 走る
	{ ANIM_PATH"RightAttackS.x",		false,	58.0f,	1.0f},  // 右攻撃
};


// コンストラクタ
CEnemyA::CEnemyA(std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mAttackStartPos(CVector::zero)
	, mAttackEndPos(CVector::zero)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
{

	// 敵を初期化
	InitEnemy("EnemyA", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);


	// 本体のコライダー
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, ENEMY_HEIGHT, 0.0f),
		CVector(0.0f,   ENEMY_SOLE, 0.0f),
		ENEMY_WIDTH
	);
	// フィールドと、プレイヤーの攻撃コライダーとヒットするように設定
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eAttackCol });

	// 左手のカプセルコライダ
	mpLAttackCol = new CColliderSphere
	(
		this, ELayer::eEnemy,
		5.0
	);
	// 衝突するタグとレイヤーを設定
	mpLAttackCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpLAttackCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });

	// 右手のカプセルコライダ
	mpRAttackCol = new CColliderSphere
	(
		this,ELayer::eEnemy,
		10.0
	);
	// 衝突するタグとレイヤーを設定
	mpRAttackCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpRAttackCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });

	// 視野範囲のデバッグ表示を作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// プレイヤーを見失った位置のノードを作成
	mpLostPlayerNode = new CNavNode(CVector::zero, true);

	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}

	// 左手のボーンを取得
	CModelXFrame* LHand = mpModel->FinedFrame("Armature_mixamorig_LeftHand");
	const CMatrix& LhandMTX = LHand->CombinedMatrix();
	// 右手のボーンを取得
	CModelXFrame* RHand = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	const CMatrix& RhandMTX = RHand->CombinedMatrix();
	// 攻撃用のコライダーを行列に設定
	mpLAttackCol->SetAttachMtx(&LhandMTX);
	mpRAttackCol->SetAttachMtx(&RhandMTX);


}

CEnemyA::~CEnemyA()
{
	// コライダーを破棄


	// 視野範囲のデバッグ表示が存在したら、一緒に削除する
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpLostPlayerNode);

		//	巡回ポイントに配置したノードも全て削除
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			delete del;
			itr++;
		}
		mPatrolPoints.clear();
	}
}

// オブジェクト削除処理
void CEnemyA::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトが視野範囲のデバッグ表示なら
	// ポインタを空にする
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

//更新処理
void CEnemyA::Update()
{

	if (CInput::Key('U')) {
		ChangeState(2);
		//ChangeAnimation(EAnimType::eWalk);
	}
	else if (CInput::Key('L')) {
		ChangeState((int)EState::ePatrol);
		//ChangeAnimation(EAnimType::eRun);
	}

	// 現在の状態に合わせて更新処理を切り替える
	switch (mState)
	{
	case (int)EState::eIdle:    UpdateIdle();   break;
	case (int)EState::ePatrol:  UpdatePatrol(); break;
	case (int)EState::eChase:   UpdateChase();  break;
	case (int)EState::eLost:    UpdateLost();   break;
	case (int)EState::eAttack:  UpdateAttack(); break;
	}

	// キャラクターの更新
	CXCharacter::Update(); 

	// コライダを更新
	mpLAttackCol->Update();
	mpRAttackCol->Update();

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("状態 : %s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("ステップ : %d\n", mStateStep);
}

void CEnemyA::Render()
{
	CXCharacter::Render();

	// 巡回中であれば
	if (mState == (int)EState::ePatrol)
	{
		float rad = 1.0f;
		// 巡回ポイントを全て描画
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CMatrix m;
			m.Translate(mPatrolPoints[i]->GetPos() + CVector(0.0f, rad * 2.0f, 0.0f));
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireSphere(m, rad, c);
		}
	}

	// 見失ったとき
	if (mState == (int)EState::eLost)
	{
		//プレイヤーを見失った位置にデバッグ表示
		float rad = 2.0f;
		CMatrix m;
		m.Translate(mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f));
		Primitive::DrawWireSphere(m, rad, CColor::blue);
	}

	// レイコライダーのデバッグ表示
	CPlayer* player = CPlayer::Instance();
	CField* field = CField::Instance();
	if (player != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		CVector playerPos = player->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		// プレイヤーとの間に遮蔽物が存在する場合
		CHitInfo hit;
		if (field->CollisionRay(selfPos, playerPos, &hit))
		{
			// 衝突した位置までの線を描画
			Primitive::DrawLine
			(
				selfPos, hit.cross,
				CColor::red,
				2.0f
			);
		}
		// 遮蔽物が存在しなかった場合
		else
		{
			// プレイヤーの位置までの緑線を描画
			Primitive::DrawLine
			(
				selfPos, playerPos,
				CColor::green,
				2.0f
			);
		}
	}
}

bool CEnemyA::IsAttacking() const
{
	if (mState == (int)EState::eAttack) return true;
	return false;
}

void CEnemyA::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CEnemyBase::AttackStart();

	// パンチ攻撃中であれば、パンチ攻撃のコライダーをオンにする
	if (mState == (int)EState::eAttack)
	{
		//mpAttack1Col->SetEnable(true);
	}
}

void CEnemyA::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemyBase::AttackEnd();

	// 攻撃コライダーをオフ
	//mpAttack1Col->SetEnable(false);
}

// 衝突処理
void CEnemyA::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpBodyCol)
	{
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突した時（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();

			}
		}
		else if (other->Layer() == ELayer::eAttackCol && other->Tag() == ETag::ePlayer)
		{

		}
	}
}


//状態切り替え
void CEnemyA::ChangeState(int state)
{
	// 攻撃中に他の状態へ移行する場合は
	// 攻撃終了処理を呼び出す
	if (mState != state && IsAttacking())
	{
		AttackEnd();
	}

	// 状態切り替え
	CEnemyBase::ChangeState(state);
}

// プレイヤーが視野範囲内に入ったかどうか
bool CEnemyA::IsFoundPlayer() const
{
	// プレイヤーが存在しない場合は、範囲外とする
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	// プレイヤー座標の取得
	CVector playerPos = player->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自身からプレイヤーまでのベクトルを求める
	CVector vec = playerPos - pos;
	vec.Y(0.0f);


	// 1: 視野角度内か求める
	// ベクトルを正規化して方向要素のみにするため
	// 長さを１にする
	CVector dir = vec.Normalized();
	// 自身の正面方向のベクトルを取得
	CVector forward = VectorZ();
	// プレイヤーまでのベクトルと
	// 自身の正面方向のベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野角度のラジアンを求める
	float angleR = Math::DegreeToRadian(mFovAngle);
	// 求めた内積と視野角度で、視野範囲か判断する
	if (dot < cosf(angleR)) return false;


	// 2: 視野距離内か求める
	// プレイヤーまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength) return false;


	// プレイヤーとの間に遮蔽物がないか判定する
	if (!IsLookPlayer()) return false;


	// 全ての条件をクリアしたので、視野範囲内である
	return true;
}

// 現在位置からプレイヤーが見えているかどうか
bool CEnemyA::IsLookPlayer() const
{
	// プライヤーが存在しない場合は、見えていない
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;
	// フィールドが存在しない場合は、遮蔽物がないので見える
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// プレイヤーの座標を取得
	CVector playerPos = player->Position() + offsetPos;
	// 自分自身の座標を取得
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	// フィールドとレイ判定を行い、遮蔽物が存在した場合は、プレイヤーが見えない
	if (field->CollisionRay(selfPos, playerPos, &hit)) return false;

	// プレイヤーとの間に遮蔽物がないので、プレイヤーが見えている
	return true;
}

bool CEnemyA::CanAttackPlayer() const
{
	// プレイヤーがいない場合は、攻撃できない
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

	// 全ての条件をみたした
	return true;
}

 //攻撃時に移動する距離か
bool CEnemyA::AttackRangeMin()
{
	// プレイヤーがいない場合は、攻撃できない
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();
	if (dist > ATTACK_RANGE_MIN) return false;

	// 全ての条件をみたした
	return true;
	return false;
}

// 指定した位置まで移動する
bool CEnemyA::MoveTo(const CVector& targetPos, float speed)
{
	// 目的地までのベクトルを求める
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);

	// 移動方向ベクトルを求める
	CVector moveDir = vec.Normalized();

	// 徐々に移動方向へ向ける
	CVector forward = CVector::Slerp
	(
		VectorZ(), // 現在の正面
		moveDir,   // 移動方向
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// 今回の移動距離を求める
	float moveDist = speed * Times::DeltaTime();
	// 目的地までの残りの距離
	float remainDist = vec.Length();
	// 残りの距離が移動距離より短い場合
	if (remainDist <= moveDist)
	{
		// 目的地まで移動する
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;    // 目的地に到着したので、trueを返す
	}

	// 残りの距離が移動距離より長い場合は、
	// 移動距離分目的地へ移動
	pos += moveDir * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
}

// 次に巡回するポイントを変更
void CEnemyA::ChangePatrolPoint()
{
	// 巡回ポイントが設定されていない場合は、処理しない
	int size = mPatrolPoints.size();
	if (size == 0) return;

	// 巡回開始時であれば、一番近い巡回ポイントを選択
	if (mNextPatrolIndex == -1)
	{
		int nearIndex = -1;     // 一番近い巡回ポイントの番号
		float nearDist = 0.0f;  // 一番近い巡回ポイントまでの距離
		// 全ての巡回ポイントを調べ、一番近い巡回ポイントを探す
		for (int i = 0; i < size; i++)
		{
			CVector point = mPatrolPoints[i]->GetPos();
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// 巡回ポイントが近すぎる場合は、スルー
			if (dist < PATROL_NEAR_DIST) continue;
			// 一番近い巡回ポイントもしくは、
			// 現在一番近い巡回ポイントよりさらに近い場合は、
			// 巡回ポイントの番号を置き換える
			if (nearIndex < 0 || dist < nearDist)
			{
				nearIndex = i;
				nearDist = dist;
			}
		}
		mNextPatrolIndex = nearIndex;
	}
	// 巡回中だった場合、次の巡回ポイントを指定する
	else 
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}

	// 次に巡回するポイントが決まった場合
	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// 巡回ポイントの経路探索ノードの位置を設定し直すことで、
			// 各ノードへの接続情報を更新
			for (CNavNode* node : mPatrolPoints)
			{
				node->SetPos(node->GetPos());
			}

			// 巡回ポイントまでの最短経路を求める
			if (navMgr->Navigate(mpNavNode, mPatrolPoints[mNextPatrolIndex], mMoveRoute))
			{
				// 次の目的地のインデックスを設定
				mNextMoveIndex = 1;
			}
		}
	}
}

// 待機状態の更新処理
void CEnemyA::UpdateIdle()
{
	// プレイヤーが視野範囲内に入ったら、追跡状態へ移行
	if (IsFoundPlayer())
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// 待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 待機時間が経過したら、巡回状態へ移行
		ChangeState((int)EState::ePatrol);
	}

	CPlayer* player = CPlayer::Instance();
}

// 巡回中の更新処理
void CEnemyA::UpdatePatrol()
{
	if (IsFoundPlayer())
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：巡回ポイントを求める
	case 0:
		mNextPatrolIndex = -1;
		ChangePatrolPoint();
		mStateStep++;
		break;
		// ステップ1：巡回ポイントまで移動
	case 1:
	{
		ChangeAnimation((int)EAnimType::eWalk);
		// 最短経路の次のノードまで移動
		CNavNode* moveNode = mMoveRoute[mNextMoveIndex];
		if (MoveTo(moveNode->GetPos(), WALK_SPEED))
		{
			// 移動が終われば、次のノードへ切り替え
			mNextMoveIndex++;
			// 最後のノード（目的地のノード）だった場合は、次のステップへ進める
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
		// ステップ2：移動後の待機
	case 2:
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
		break;
	}
}

// 追跡中の更新処理
void CEnemyA::UpdateChase()
{
	// プレイヤーの座標へ向けて移動する
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();

	// プレイヤーが見えなくなったら、見失った状態に戻す
	if (!IsLookPlayer())
	{
		mpLostPlayerNode->SetPos(targetPos);
		ChangeState((int)EState::eLost);
		return;
	}
	// プレイヤーに攻撃できるならば、攻撃状態へ移行
	if (CanAttackPlayer())
	{
		ChangeState((int)EState::eAttack);
		return;
	}

	// 走るアニメーションを再生
	ChangeAnimation((int)EAnimType::eRun);

	// 経路探索管理クラスが存在すれば、
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 自身のノードからプレイヤーのノードまでの最短距離を求める
		CNavNode* playerNode = player->GetNavNode();
		if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
		{
			// 自身のノードからプレイヤーのノードまで繋がっていたら、
			// 移動する位置を次のノードの位置に設定
			targetPos = mMoveRoute[1]->GetPos();
		}
	}

	// 移動処理
	if (MoveTo(targetPos, RUN_SPEED))
	{
	}
}

// 見失った時の更新処理
void CEnemyA::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		ChangeState((int)EState::eIdle);
		return;
	}
	// プレイヤーが見えたら、追跡状態へ移行
	if (IsLookPlayer()) 
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// 走るアニメーションを再生
	ChangeAnimation((int)EAnimType::eRun);

	switch (mStateStep)
	{
		// ステップ0：見失った位置までの最短経路を求める
	case 0:
		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
		{
			// 見失った位置まで経路が繋がっていたら、次のステップへ
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			// 経路が繋がっていなければ、待機状態へ戻す
			ChangeState((int)EState::eIdle);
		}
		break;
	case 1:
		// プレイヤーを見失った位置まで移動
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// 移動が終われば、待機状態へ移行
				ChangeState((int)EState::eIdle);
			}
		}
		break;
	}
}

// 攻撃時の更新処理
void CEnemyA::UpdateAttack()
{
	switch (mStateStep)
	{
		// ステップ0 : 攻撃アニメーション
	    case 0:
			// 攻撃開始位置と攻撃終了位置の設定
			mAttackStartPos = Position();
			mAttackEndPos = mAttackStartPos + VectorZ() * ATTACK_MOVE_DIST;
			ChangeAnimation((int)EAnimType::eRightAttack,true);
			mStateStep++;
			break;
		// ステップ1 : 攻撃時の移動処理
		case 1:
		{
			// 攻撃アニメーションが移動開始フレームを超えた場合
			float frame = GetAnimationFrame();
			if (!AttackRangeMin()) {
				if (frame >= ATTACK_MOVE_START)
				{
					// 移動終了フレームまで到達してない場合
					if (frame < ATTACK_MOVE_END)
					{
						// 線形補間で移動開始位置から移動終了位置まで移動する
						float moveFrame = ATTACK_MOVE_END - ATTACK_MOVE_START;
						float percent = (frame - ATTACK_MOVE_START) / moveFrame;
						CVector pos = CVector::Lerp(mAttackStartPos, mAttackEndPos, percent);
						Position(pos);
					}
					// 移動終了フレームまで到達した場合
					else
					{
						Position(mAttackEndPos);
						mStateStep++;
					}
				}
			}
			else
			{
				mStateStep++;
			}
			break;
		}
		// ステップ2 : 攻撃アニメーションの終了待ち
		case 2:
			if (IsAnimationFinished())
			{
				mStateStep++;
			}
			break;
		// ステップ3 : 攻撃終了時の待ち時間
		case 3:
			if (mElapsedTime < ATTACK_WAIT_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				// 時間が経過したら、待機状態へ移行
				ChangeState((int)EState::eIdle);
			}
			break;
	}
	
}



// 状態の文字列を取得
std::string CEnemyA::GetStateStr(int state) const
{
	switch ((int)state)
	{
	case (int)EState::eIdle:    return "待機";
	case (int)EState::ePatrol:  return "巡回";
	case (int)EState::eChase:   return "追跡";
	case (int)EState::eLost:    return "見失う";
	case (int)EState::eAttack:  return "攻撃";
	}
	return "";
}

CColor CEnemyA::GetStateColor(int state) const
{
	switch ((int)state)
	{
	case (int)EState::eIdle:    return CColor::white;
	case (int)EState::ePatrol:  return CColor::green;
	case (int)EState::eChase:   return CColor::red;
	case (int)EState::eLost:    return CColor::yellow;
	case (int)EState::eAttack:  return CColor::magenta;
	}
	return CColor::white;
}
