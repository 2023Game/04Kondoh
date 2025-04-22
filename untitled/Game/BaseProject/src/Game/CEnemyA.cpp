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

#define ANIM_PATH "Character\\EnemyA\\anim\\"
#define ATTACK_ANIM_PATH "Character\\EnemyA\\anim\\Attack\\"
#define DAMAGEHIT_PATH "Character\\EnemyA\\anim\\DamageHit\\"
#define CHANCEHIT_PATH "Character\\EnemyA\\anim\\ChanceHit\\"

#define ENEMY_HEIGHT 16.0f      // 敵の高さ
#define ENEMY_SOLE   5.0f       // 敵の底
#define ENEMY_WIDTH  5.0f       // 敵の半径
#define FOV_ANGLE 45.0f         // 視野範囲の角度
#define FOV_LENGTH 100.0f       // 視野範囲の距離
#define EYE_HEIGHT 10.0f        // 視点の高さ
#define WALK_SPEED 10.0f        // 歩きの速度
#define RUN_SPEED 40.0f         // 走っている時の速度
#define ROTATE_SPEED 6.0f       // 回転速度

#define DISTANT_ATTACK_RANGE 50.0f	// タックル攻撃範囲
#define NEAR_ATTACK_RANGE 30.0f		// 薙ぎ払いと回し蹴り、三連攻撃の範囲
#define HEADBUTT_ATTACK_RANGE 15.0f	// 頭突き攻撃範囲

#define BLOWL_PROB 20		// 左薙ぎ払いの確率
#define BLOWR_PROB 40		// 右薙ぎ払いの確率
#define KICKL_PROB 50		// 左回し蹴りの確率
#define KICKR_PROB 60		// 右回し蹴りの確率
#define TRIPLE_PROB 95		// 三連攻撃の確率
#define TACKLE_PROB 40		// タックルの確率
#define FIFTY_FIFTY_PROB 50	// 半分の確率

#define TACKLE_MOVE_DIST	50.0f	// タックル時の移動距離
#define TACKLE_MOVE_START	0.5f	// タックル時の移動開始フレーム
#define TACKLE_MOVE_END		55.0f	// タックル時の移動終了フレーム
#define KICK_MOVE_DIST		10.0f	// 回し蹴り時の移動距離
#define KICK_MOVE_START		1.0f	// 回し蹴り時の移動開始フレーム
#define KICK_MOVE_END		50.0f	// 回し蹴り時の移動終了フレーム
#define ATTACK_WAIT_TIME	1.0f	// 攻撃終了時の待ち時間

#define IDLE_TIME_MIN 0.0f			// 待機時の最短待機時間
#define IDLE_TIME_MAX 10.0f			// 待機時の最長待機時間
#define BATTLE_IDLE_TIME_MIN 1.0f	// 戦闘待機時の最短待機時間
#define BATTLE_IDLE_TIME_MAX 5.0f	// 戦闘待機時の最長待機時間

#define ATTACK_ANGLE		35.0f	// 攻撃範囲の角度
#define ATTACK_LENGTH		50.0f	// 攻撃範囲の距離

#define PATROL_INTERVAL 3.0f    // 次の巡回に移動開始するまでの時間
#define PATROL_NEAR_DIST 10.0f  // 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME 5.0f          // 待機状態の時間
#define DEATH_WAIT_TIME 3.0f	// 死亡時の待機時間
#define LOOKAT_SPEED 5.0f		// 対象の方向に向く速度

#define GUARD_PROB 90		// 防御の確率（パーセント）

#define DETECT_COL_POS CVector(0.0f, 5.0f, 3.0f)	// 判定用コライダの座標
#define DETECT_COL_SCL CVector(0.7f, 1.5f, 0.7f)	// 判定用コライダの大きさ

#define EATTACKWAY CPlayer::EAttackDir		// プレイヤーの攻撃方向
#define EATTACKPWOER CPlayer::EAttackPower	// プレイヤーの攻撃威力

// プレイヤーのアニメーションデータのテーブル
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",									true,	0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle.x",					true,	126.0f,	1.0f},	// 待機
	{ ANIM_PATH"BattleIdle.x",				true,	110.0f,	1.0f},	// 戦闘待機		
	{ ANIM_PATH"Walk.x",					true,	42.0f,	1.0f},	// 歩行
	{ ANIM_PATH"Run.x",						true,	23.0f,	1.0f},	// 走る
	{ ANIM_PATH"Death.x",					false,	129.0f,	1.0f},	// 死亡
	{ ANIM_PATH"Stan.x",					false,	129.0f,	1.0f},	// 気絶アニメーション

	{ ATTACK_ANIM_PATH"LeftAttackS.x",		false,	34.0f,	0.5f},	// 左薙ぎ払い
	{ ATTACK_ANIM_PATH"RightAttackS.x",		false,	34.0f,	0.5f},	// 右薙ぎ払い
	{ ATTACK_ANIM_PATH"LeftAttackM.x",		false,	53.0f,	1.0f},	// 左回し蹴り
	{ ATTACK_ANIM_PATH"RightAttackM.x",		false,	53.0f,	1.0f},	// 右回し蹴り
	{ ATTACK_ANIM_PATH"Tackle.x",			false,	56.0f,	1.0f},	// 竜巻旋風脚タックル
	{ ATTACK_ANIM_PATH"HeadButt.x",			false,	68.0f,	1.0f},	// 頭突き攻撃

	{ DAMAGEHIT_PATH"Damage1.x",			true,	42.0f,	1.0f},	// 仰け反り1
	{ DAMAGEHIT_PATH"Damage2.x",			true,	42.0f,	1.0f},	// 仰け反り2
	{ DAMAGEHIT_PATH"Damage3.x",			true,	42.0f,	1.0f},	// 仰け反り3
	{ CHANCEHIT_PATH"chancehit1.x",			true,	23.0f,	1.0f},	// チャンス時1
	{ CHANCEHIT_PATH"chancehit2.x",			true,	23.0f,	1.0f},	// チャンス時2
	{ CHANCEHIT_PATH"chancehit3.x",			true,	23.0f,	1.0f},	// チャンス時3
	{ CHANCEHIT_PATH"chancehit4.x",			true,	23.0f,	1.0f},	// チャンス時4

};

const std::vector<CEnemyBase::AttackData> ATTACK_DATA =
{
	// 指定なし
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, 0.0f, 0.0f},
	// 左薙ぎ払い
	{ EAttackDir::eLeft,	EAttackPower::eAttackS,	true, 0.0f, 12.0f},
	// 右薙ぎ払い
	{ EAttackDir::eRight,	EAttackPower::eAttackS,	true, 0.0f, 12.0f},
	// 左回し蹴り
	{ EAttackDir::eLeft,	EAttackPower::eAttackM,	true, 0.0f, 10.0f},
	// 右回し蹴り
	{ EAttackDir::eRight,	EAttackPower::eAttackM,	true, 0.0f, 10.0f},
	// 竜巻旋風脚タックル
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, 0.0f, 0.0f},
	// 頭突き攻撃
	{ EAttackDir::eUp,		EAttackPower::eAttackL,	false, 0.0f, 0.0f},
	// 三連攻撃
	{ EAttackDir::eNone,	EAttackPower::eAttackS,	false, 0.0f, 0.0f},
};

// コンストラクタ
CEnemyA::CEnemyA(std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mPlayerAttackAngle(ATTACK_ANGLE)
	, mPlayerAttackLength(ATTACK_LENGTH)
	, mpDebugFov(nullptr)
	, mAttackStartPos(CVector::zero)
	, mAttackEndPos(CVector::zero)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
	, mpBattleTarget(nullptr)
	, mIsBattle(false)
	, mAttackCount(0)
	, mIsTripleAttack(false)
{
	//この敵キャラの攻撃データを設定
	mpAttackData = &ATTACK_DATA;
	mMaxHp = 100;
	mHp = mMaxHp;

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
		this, ELayer::eAttackCol,
		30.0
	);
	// 衝突するタグとレイヤーを設定
	mpLAttackCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpLAttackCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpLAttackCol->SetEnable(false);

	// 右手のカプセルコライダ
	mpRAttackCol = new CColliderSphere
	(
		this,ELayer::eAttackCol,
		30.0
	);
	// 衝突するタグとレイヤーを設定
	mpRAttackCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpRAttackCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpRAttackCol->SetEnable(false);

	// プレイヤーが存在しない場合は、範囲外とする
	CPlayer* player = CPlayer::Instance();

	// 視野範囲のデバッグ表示を作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
	// 攻撃範囲のデバッグ表示を作成
	mpDebugAttack = new CDebugFieldOfView(player, mPlayerAttackAngle, mPlayerAttackLength);

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

	// 待機最大時間をランダムで決める（１回だけだよ）
	mIdleTime = Math::Rand(0.0f, 5.0f);
}

CEnemyA::~CEnemyA()
{
	// コライダーを破棄
	SAFE_DELETE(mpLAttackCol);
	SAFE_DELETE(mpRAttackCol);

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
	case (int)EState::eIdle:		UpdateIdle();		break;
	case (int)EState::ePatrol:		UpdatePatrol();		break;
	case (int)EState::eBattleIdle:	UpdateBattleIdle();	break;
	case (int)EState::eChase:		UpdateChase();		break;
	case (int)EState::eLost:		UpdateLost();		break;
	case (int)EState::eAttack:		UpdateAttack();		break;
	case (int)EState::eGuard:		UpdateGuard();		break;
	case (int)EState::eAvoid:		UpdateAvoid();		break;
	case (int)EState::eStan:		UpdateStan();		break;
	case (int)EState::eDeath:		UpdateDeath();		break;
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

	CDebugPrint::Print("■敵の情報\n");
	CDebugPrint::Print("　HP：%d\n", mHp);
	CDebugPrint::Print("　怯み度：%.2f\n", mStanPoints);
	CDebugPrint::Print("　状態：%s\n", GetStateStr(mState).c_str());
	CDebugPrint::Print("　攻撃タイプ：%s\n", GetAttackTypeStr(mAttackType).c_str());
	CDebugPrint::Print("　攻撃の強さ：%s\n", GetAttackPowerStr().c_str());
	CDebugPrint::Print("　攻撃の方向：%s\n", GetAttackDirStr().c_str());
	CDebugPrint::Print("　フレーム：%.2f\n", GetAnimationFrame());
	CDebugPrint::Print("　最大待機時間：%.2f\n", mIdleTime);

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

bool CEnemyA::IsState(int state)
{
	if ((int)mState == state) return true;
	return false;
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
	if (mAttackType == (int)EAttackType::eBlowR)
	{
		mpRAttackCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eBlowL)
	{
		mpLAttackCol->SetEnable(true);
	}
}

void CEnemyA::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemyBase::AttackEnd();

	// 攻撃コライダーをオフ
	mpLAttackCol->SetEnable(false);
	mpRAttackCol->SetEnable(false);
}

void CEnemyA::TakeDamage(int damage, float stan, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CEnemyBase::TakeDamage(damage, stan, causer);
	// 死亡しなければ
	if (!IsDeath())
	{
		// 攻撃を加えた相手を戦闘相手に設定
		mpBattleTarget = causer;
		// 戦闘状態を切り替える
		mIsBattle = true;

		if (!IsAttacking())
		{
			// 移動を停止
			mMoveSpeed = CVector::zero;
			// 攻撃を加えた相手の方向へ向く
			LookAtBattleTarget(true);
		}
	}

}

void CEnemyA::Death()
{
	ChangeState((int)EState::eDeath);
}

void CEnemyA::Stan()
{
	ChangeState((int)EState::eStan);
}

// 衝突処理
void CEnemyA::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayer* player = CPlayer::Instance();

	// ベースの衝突処理を呼び出す
	CEnemyBase::Collision(self, other, hit);

	if (self == mpLAttackCol || self == mpRAttackCol)
	{
		if (other->Tag() == ETag::ePlayer && other->Layer() == ELayer::ePlayer)
		{
			// ヒットしたのがキャラクターかつ、
			// まだ攻撃がヒットしていないキャラクターであれば
			CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
			if (chara != nullptr && !IsAttackHitObj(chara))
			{
				// 攻撃ヒット済みリストに登録
				AddAttackHitObj(chara);

				// 与えるダメージの計算
				int damage = 0;
				float stan = 0.0f;
				// ダメージを与える
				chara->TakeDamage(3, 0.0f, this);

			}
		}
	}
}

void CEnemyA::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan) const
{
	if (mAttackType == (int)EAttackType::eBlowL)
	{

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

// 攻撃タイプ切り替え
void CEnemyA::ChangeAttackType(int attacktype)
{
	// 攻撃中に他の攻撃タイプへ移行する場合は
	// 攻撃終了処理を呼び出す
	if (mAttackType != attacktype && IsState((int)EState::eAttack))
	{
		AttackEnd();
	}
	// 攻撃タイプ切り替え
	CEnemyBase::ChangeAttackType(attacktype);
}



// TODO:エネミーベースクラスに移植する
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

// プレイヤーを攻撃出来るかどうか
bool CEnemyA::CanAttackPlayer(float range) const
{
	// プレイヤーがいない場合は、攻撃できない
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();
	// 攻撃範囲より外にいたら、攻撃しない
	if (dist > range) return false;

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

	if (dist > HEADBUTT_ATTACK_RANGE) return false;

	// 全ての条件をみたした
	return true;
}

// TODO:ここ間違っているかも
// プレイヤーの攻撃を検知したか？
bool CEnemyA::IsPlayerAttackDetected() const
{
	// プレイヤーが存在しない場合は、範囲外とする
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	if (!player->IsAttacking()) return false;

	// プレイヤー座標の取得
	CVector playerPos = player->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// プレイヤーから自身までのベクトルを求める
	CVector vec = pos - playerPos;
	vec.Y(0.0f);


	// 1: 視野角度内か求める
	// ベクトルを正規化して方向要素のみにするため
	// 長さを１にする
	CVector dir = vec.Normalized();
	// プレイヤーの正面方向のベクトルを取得
	CVector forward = player->VectorZ();
	// 自身までのベクトルと
	// プレイヤーの正面方向のベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野角度のラジアンを求める
	float angleR = Math::DegreeToRadian(mPlayerAttackAngle);
	// 求めた内積と視野角度で、視野範囲か判断する
	if (dot < cosf(angleR)) return false;


	// 2: 攻撃距離内か求める
	// プレイヤーまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mPlayerAttackLength) return false;

	// プレイヤーとの間に遮蔽物がないか判定する
	if (!IsLookPlayer()) return false;

	// 全ての条件をクリアしたので、視野範囲内である
	return true;

}

bool CEnemyA::DetectedPlayerAttack()
{
	CPlayer* player = CPlayer::Instance();

	// プレイヤーの攻撃が検知済みでなければ
	if (!mIsDetectedPlayerAttack)
	{
		// プレイヤーの攻撃を検知したかどうか
		if (IsPlayerAttackDetected())
		{
			mIsDetectedPlayerAttack = true;

			int rand = Math::Rand(0, 99);

			if (rand < GUARD_PROB)
			{
				ChangeState((int)EState::eStan);
					return true;
			}
		}
	}
	// プレイヤーの攻撃を検知済み
	else
	{
		// プレイヤーの攻撃が終わったら、検知フラグを初期化
		if (!player->IsAttacking())
		{
			mIsDetectedPlayerAttack = false;
		}
	}
	return false;
}

// どの攻撃をするか判定する
void CEnemyA::AttackPickDetect()
{
	EState nextstate = EState::eBattleIdle;
	EAttackType nextattack = EAttackType::eNone;

	// 超近距離の攻撃判定
	if (CanAttackPlayer(HEADBUTT_ATTACK_RANGE))
	{
		nextattack = EAttackType::eHeadButt;
		nextstate = EState::eAttack;
	}
	// 近距離の攻撃判定
	else if (CanAttackPlayer(NEAR_ATTACK_RANGE))
	{
		int attackRand = Math::Rand(0, 99);

		if (attackRand < BLOWL_PROB)
		{
			nextattack = EAttackType::eBlowL;
			nextstate = EState::eAttack;
		}
		else if (attackRand < BLOWR_PROB)
		{
			nextattack = EAttackType::eBlowR;
			nextstate = EState::eAttack;
		}
		else if (attackRand < KICKL_PROB)
		{
			nextattack = EAttackType::eRoundKickL;
			nextstate = EState::eAttack;
		}
		else if (attackRand < KICKR_PROB)
		{
			nextattack = EAttackType::eRoundKickR;
			nextstate = EState::eAttack;
		}
		else if (attackRand < TRIPLE_PROB)
		{
			mIsTripleAttack = true;
			nextattack = EAttackType::eTripleAttack;
			nextstate = EState::eAttack;
		}
		else
		{
			nextstate = EState::eBattleIdle;
		}
	}
	// 遠距離の攻撃判定
	else if (CanAttackPlayer(DISTANT_ATTACK_RANGE))
	{
		int attackRand = Math::Rand(0, 99);

		if (attackRand < TACKLE_PROB)
		{
			nextattack = EAttackType::eTackle;
			nextstate = EState::eAttack;
		}
		else
		{
			nextstate = EState::eChase;
		}
	}
	else
	{
		nextstate = EState::eChase;
	}

		ChangeAttackType((int)nextattack);
		ChangeState((int)nextstate);
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


bool CEnemyA::RandMove(float speed)
{

}

// 戦闘相手の方へ向く
void CEnemyA::LookAtBattleTarget(bool immediate)
{
	// 戦闘相手がいなければ、処理しない
	if (mpBattleTarget == nullptr) return;

	// 戦闘相手までの方向ベクトルを求める
	CVector targetPos = mpBattleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);
	vec.Normalize();
	// すぐに戦闘相手の方向へ向く
	if (immediate)
	{
		Rotation(CQuaternion::LookRotation(vec));
	}
	// 徐々に戦闘相手の方向へ向く
	else
	{
		CVector forward = CVector::Slerp
		(
			VectorZ(), vec,
			LOOKAT_SPEED * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
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
	// プレイヤーが視野範囲内に入るもしくは、
	// 既に戦闘状態であれば、追跡状態へ移行
	if (IsFoundPlayer() || mIsBattle)
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// 待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	if (mElapsedTime < mIdleTime)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 待機時間が経過したら、巡回状態へ移行
		ChangeState((int)EState::ePatrol);
	}
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
			// 待機最大時間をランダムで決める
			mIdleTime = Math::Rand(IDLE_TIME_MIN, IDLE_TIME_MAX);
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
		break;
	}
}

// 戦闘待機状態の更新処理
void CEnemyA::UpdateBattleIdle()
{
	if (DetectedPlayerAttack()) return;

	ChangeAnimation((int)EAnimType::eBattleIdle);

	LookAtBattleTarget();
	switch (mStateStep)
	{
	case 0:	// ステップ0：待機時間をランダムで決定
		// 待機時間が決まってなければ
		if (mBattleIdletime <= 0.0f)
		{
			// ランダムで待機時間を決定
			mBattleIdletime = Math::Rand
			(
				BATTLE_IDLE_TIME_MIN,
				BATTLE_IDLE_TIME_MAX
			);
		}
		mStateStep++;
		break;
	case 1:
		// 戦闘時の待機時間待ち
		if (mElapsedTime < mBattleIdletime)
		{
			// TODO:プレイヤーの攻撃範囲外に移動する
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// 攻撃の種類とステートを決める
			AttackPickDetect();
			// 戦闘待機時間を初期化
			mBattleIdletime = 0.0f;
		}
	}
}

// 追跡中の更新処理
void CEnemyA::UpdateChase()
{

	if (DetectedPlayerAttack()) return;

	// プレイヤーの座標へ向けて移動する
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();

	// 戦闘状態に入っていなければ
	if (!mIsBattle)
	{
		// 戦闘状態になる
		mIsBattle = true;
		mpBattleTarget = player;
	}

	// プレイヤーが見えなくなったら、見失った状態に戻す
	if (!IsLookPlayer())
	{
		mpLostPlayerNode->SetPos(targetPos);
		ChangeState((int)EState::eLost);
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

	if (CanAttackPlayer(DISTANT_ATTACK_RANGE))
	{
		// プレイヤーに攻撃できるならば、攻撃状態へ移行
		AttackPickDetect();
	}
}

// 見失った時の更新処理
void CEnemyA::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		// 戦闘状態終了
		mIsBattle = false;
		mpBattleTarget = nullptr;
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
				// 戦闘状態終了
				mIsBattle = false;
				mpBattleTarget = nullptr;
				// 移動が終われば、待機状態へ移行
				ChangeState((int)EState::eIdle);
			}
		}
		break;
	}
}

// TODO:攻撃コライダーがオンになるまで、プレイヤーの方向へ向く

// 攻撃時の更新処理
void CEnemyA::UpdateAttack()
{
	// 攻撃の種類ごとに、更新処理を切り替える
	switch (mAttackType)
	{
	case (int)EAttackType::eBlowL:			UpdateBlowL();			break;
	case (int)EAttackType::eBlowR:			UpdateBlowR();			break;
	case (int)EAttackType::eRoundKickL:		UpdateRoundKickL();		break;
	case (int)EAttackType::eRoundKickR:		UpdateRoundKickR();		break;
	case (int)EAttackType::eTackle:			UpdateTackle();			break;
	case (int)EAttackType::eHeadButt:		UpdateHeadButt();		break;
	case (int)EAttackType::eTripleAttack:	UpdataTripleAttack();	break;
	}

	if (mAttackType == (int)EAttackType::eNone)
	{
		ChangeState((int)EState::eBattleIdle);
	}
}

void CEnemyA::UpdateGuard()
{
}

void CEnemyA::UpdateAvoid()
{
}

void CEnemyA::UpdateDeath()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：死亡アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eDeath, true);
		mStateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 死亡アニメーションが終了したら、削除
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
		// ステップ2：死亡後の待ち
	case 2:
		if (mElapsedTime < DEATH_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待ち時間が終了したら、削除
		else
		{
			Kill();
		}
		break;
	}
}

void CEnemyA::UpdateStan()
{
	// TODO:アニメーションを再生、１秒後に復活、
	// 視界に入っていたら：攻撃状態に、入っていなかったら：待機状態に
	// １秒の間で攻撃をすると：攻撃チャンス！！状態へ
	switch (mStateStep)
	{
		// 怯みアニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eStan, true);
		mStateStep++;
		break;
		// アニメーション終了待ち
	case 1:
		// アニメーションが終われば待機状態へ戻す
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

//void CEnemyA::UpdateChance()
//{
//	// TODO：ノックバックするようになる
//	// 攻撃に倍率が乗る
//}

// 薙ぎ払い攻撃
void CEnemyA::UpdateBlowL()
{
	switch (mStateStep)
	{
	case 0: // アニメーション再生
		ChangeAnimation((int)EAnimType::eBlowL, true);
		mStateStep++;
		break;
	case 1: // 攻撃用コライダーオン
	{
		// パリィが出来る範囲が終了したら、攻撃コライダーをオンにする
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		if (frame >= data.parryEndFrame)
		{
			AttackStart();
			mStateStep++;
		}
		// 攻撃コライダーがオンになるまで、プレイヤーの方向へ向く
		else
		{
			LookAtBattleTarget();
		}
		break;
	}
	case 2:	// ステップ2 : 攻撃アニメーションの終了待ち
		if (IsAnimationFinished())
		{
			AttackEnd();
			if (mIsTripleAttack)
			{
				mStateStep++;
			}
			else
			{
				// 時間が経過したら、待機状態へ移行
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// ステップ3 : 攻撃終了時の待ち時間
		mAttackCount++;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		ChangeState((int)EState::eAttack);
		break;
	}
}

// 薙ぎ払い攻撃
void CEnemyA::UpdateBlowR()
{
	switch (mStateStep)
	{
	case 0: // アニメーション再生
		ChangeAnimation((int)EAnimType::eBlowR, true);
		mStateStep++;
		break;
	case 1: // 攻撃用コライダーオン
	{
		// パリィが出来る範囲が終了したら、攻撃コライダーをオンにする
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		if (frame >= data.parryEndFrame)
		{
			AttackStart();
			mStateStep++;
		}
		// 攻撃コライダーがオンになるまで、プレイヤーの方向へ向く
		else
		{
			LookAtBattleTarget();
		}
		break;
	}
	case 2:	// ステップ2 : 攻撃アニメーションの終了待ち
		if (IsAnimationFinished())
		{
			AttackEnd();
			if (mIsTripleAttack)
			{
				mStateStep++;
			}
			else
			{
				// 時間が経過したら、待機状態へ移行
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// ステップ3 : 攻撃終了時の待ち時間
		mAttackCount++;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		ChangeState((int)EState::eAttack);
		break;
	}
}

// 回し蹴り攻撃
void CEnemyA::UpdateRoundKickL()
{
	switch (mStateStep)
	{
		case 0:	// ステップ０：攻撃アニメーション、攻撃開始位置と攻撃終了位置の設定
			mAttackStartPos = Position();
			mAttackEndPos = mAttackStartPos + VectorZ() * KICK_MOVE_DIST;
			ChangeAnimation((int)EAnimType::eRoundKickL, true);
			mStateStep++;
			break;
		case 1:	// ステップ１：攻撃時の移動処理
		{
			// 攻撃アニメーションが移動開始フレームを超えた場合
			float frame = GetAnimationFrame();
			AttackStart();
			if (!AttackRangeMin()) {
				if (frame >= KICK_MOVE_START)
				{
					// 移動終了フレームまで到達してない場合
					if (frame < KICK_MOVE_END)
					{
						// 線形補間で移動開始位置から移動終了位置まで移動する
						float moveFrame = KICK_MOVE_END - KICK_MOVE_START;
						float percent = (frame - KICK_MOVE_START) / moveFrame;
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
		case 2:	// ステップ２：攻撃アニメーションの終了待ち
			if (IsAnimationFinished())
			{
				AttackEnd();
				ChangeState((int)EState::eBattleIdle);
			}
			break;
	}
}

// 回し蹴り攻撃
void CEnemyA::UpdateRoundKickR()
{
	switch (mStateStep)
	{
	case 0:	// ステップ０：攻撃アニメーション、攻撃開始位置と攻撃終了位置の設定
		mAttackStartPos = Position();
		mAttackEndPos = mAttackStartPos + VectorZ() * KICK_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eRoundKickR, true);
		mStateStep++;
		break;
	case 1:	// ステップ１：攻撃時の移動処理
	{
		// 攻撃アニメーションが移動開始フレームを超えた場合
		float frame = GetAnimationFrame();
		AttackStart();
		if (!AttackRangeMin()) {
			if (frame >= KICK_MOVE_START)
			{
				// 移動終了フレームまで到達してない場合
				if (frame < KICK_MOVE_END)
				{
					// 線形補間で移動開始位置から移動終了位置まで移動する
					float moveFrame = KICK_MOVE_END - KICK_MOVE_START;
					float percent = (frame - KICK_MOVE_START) / moveFrame;
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
	case 2:	// ステップ２：攻撃アニメーションの終了待ち
		if (IsAnimationFinished())
		{
			AttackEnd();
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// 竜巻旋風脚タックル
void CEnemyA::UpdateTackle()
{
	switch (mStateStep)
	{
	case 0:	// ステップ0 : 攻撃アニメーション
		// 攻撃開始位置と攻撃終了位置の設定
		mAttackStartPos = Position();
		mAttackEndPos = mAttackStartPos + VectorZ() * TACKLE_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eTackle, false);
		mStateStep++;
		break;
	case 1:	// ステップ１：攻撃時の移動処理
	{
		// 攻撃アニメーションが移動開始フレームを超えた場合
		float frame = GetAnimationFrame();
		AttackStart();
		if (!AttackRangeMin()) {
			if (frame >= TACKLE_MOVE_START)
			{
				// 移動終了フレームまで到達してない場合
				if (frame < TACKLE_MOVE_END)
				{
					// 線形補間で移動開始位置から移動終了位置まで移動する
					float moveFrame = TACKLE_MOVE_END - TACKLE_MOVE_START;
					float percent = (frame - TACKLE_MOVE_START) / moveFrame;
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
	case 2:	// ステップ2 : 攻撃アニメーションの終了待ち
		if (IsAnimationFinished())
		{
			AttackEnd();
			// 時間が経過したら、待機状態へ移行
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// 押し出し攻撃
void CEnemyA::UpdateHeadButt()
{
	switch (mStateStep)
	{
	case 0: // アニメーション再生
		ChangeAnimation((int)EAnimType::eHeadButt, true);
		mStateStep++;
		break;
	case 1: // 攻撃用コライダーオン
	{
		// パリィが出来る範囲が終了したら、攻撃コライダーをオンにする
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		if (frame >= data.parryEndFrame)
		{
			AttackStart();
			mStateStep++;
		}
		// 攻撃コライダーがオンになるまで、プレイヤーの方向へ向く
		else
		{
			LookAtBattleTarget();
		}
		break;
	}
	case 2:	// ステップ2 : 攻撃アニメーションの終了待ち
		if (IsAnimationFinished())
		{
			AttackEnd();
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// 三連攻撃
void CEnemyA::UpdataTripleAttack()
{
	switch (mAttackCount)
	{
	case 0:
	{
		int attackrand = Math::Rand(0, 99);
		// 三項演算子
		attackrand < FIFTY_FIFTY_PROB ?
			ChangeAttackType((int)EAttackType::eBlowL) : ChangeAttackType((int)EAttackType::eBlowR);
		ChangeState((int)EState::eAttack);
		break;
	}
	case 1:
	{
		if (mElapsedTime < ATTACK_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			attackrand < FIFTY_FIFTY_PROB ?
				ChangeAttackType((int)EAttackType::eBlowL) : ChangeAttackType((int)EAttackType::eBlowR);
			ChangeState((int)EState::eAttack);
			break;
		}
	}
	case 2:
	{
		if (mElapsedTime < ATTACK_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			attackrand < FIFTY_FIFTY_PROB ?
				ChangeAttackType((int)EAttackType::eBlowL) : ChangeAttackType((int)EAttackType::eBlowR);
			ChangeState((int)EState::eAttack);
			break;
		}
	}
	case 3:
		mAttackCount = 0;
		mIsTripleAttack = false;
		ChangeState((int)EState::eBattleIdle);
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
	case (int)EState::eDeath:	return "死亡";
	case (int)EState::eStan:	return "気絶";
	}
	return "";
}

std::string CEnemyA::GetAttackTypeStr(int state) const
{
	switch ((int)state)
	{
	case (int)EAttackType::eNone:			return "指定なし";
	case (int)EAttackType::eBlowL:			return "左薙ぎ払い";
	case (int)EAttackType::eBlowR:			return "右薙ぎ払い";
	case (int)EAttackType::eRoundKickL:		return "左回し蹴り";
	case (int)EAttackType::eRoundKickR:		return "右回し蹴り";
	case (int)EAttackType::eTackle:			return "竜巻旋風脚タックル";
	case (int)EAttackType::eHeadButt:		return "頭突き攻撃";
	case (int)EAttackType::eTripleAttack:	return "三連攻撃";
	}
	return std::string();
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
	case (int)EState::eDeath:	return CColor::black;
	case (int)EState::eStan:	return CColor::cyan;
	}
	return CColor::white;
}


