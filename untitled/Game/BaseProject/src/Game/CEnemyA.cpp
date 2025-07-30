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

#define ENEMY_HP 100
#define ENEMY_HEIGHT 16.0f      // 敵の高さ
#define ENEMY_SOLE   5.0f       // 敵の底
#define ENEMY_WIDTH  5.0f       // 敵の半径
#define FOV_ANGLE 45.0f         // 視野範囲の角度
#define FOV_LENGTH 200.0f       // 視野範囲の距離
#define EYE_HEIGHT 10.0f        // 視点の高さ
#define WALK_SPEED 10.0f        // 歩きの速度
#define BATTLE_WALK_SPEED 0.01f	// 戦闘時の移動
#define RUN_SPEED 40.0f         // 走っている時の速度
#define ROTATE_SPEED 6.0f       // 回転速度
#define LOOKAT_SPEED 4.0f		// 対象の方向に向く速度

#define IDLE_TIME 5.0f          // 待機状態の時間
#define DEATH_WAIT_TIME 3.0f	// 死亡時の待機時間
#define STUN_WAIT_TIME 3.0f		// 気絶時の待機時間

#define DISTANT_ATTACK_RANGE	150.0f	// タックル攻撃範囲
#define NEAR_ATTACK_RANGE		 30.0f	// 薙ぎ払いと回し蹴り、三連攻撃の範囲
#define HEADBUTT_ATTACK_RANGE	  10.0f	// 頭突き攻撃範囲

#define BLOW_DMG		10.0f	// 薙ぎ払いのダメージ
#define KICK_DMG		20.0f	// 回し蹴りのダメージ
#define TACKLE_DMG		50.0f	// タックルのダメージ
#define HEADBUTT_DMG	 5.0f	// 頭突きのダメージ

#define STUN_THRESHOLD	100.0f	// 怯みのしきい値
#define BLOW_STUN		10.0f	// 薙ぎ払いのスタン値
#define KICK_STUN		30.0f	// 回し蹴りのスタン値
#define TACKLE_STUN		20.0f	// タックルのスタン値
#define HEADBUTT_STUN	50.0f	// 頭突きのスタン値

#define BLOW_KNOCKBACK		50.0f	// 薙ぎ払いのノックバック距離
#define KICK_KNOCKBACK		50.0f	// 
#define TACKLE_KNOCKBACK	50.0f	// 
#define HEADBUTT_KNOCKBACK	50.0f	//

#define BLOWL_PROB	20		// 左薙ぎ払いの確率
#define BLOWR_PROB	40		// 右薙ぎ払いの確率
#define KICKL_PROB	50		// 左回し蹴りの確率
#define KICKR_PROB	60		// 右回し蹴りの確率
#define TRIPLE_PROB	65		// 三連攻撃の確率
#define TACKLE_PROB	90		// タックルの確率
#define GUARD_PROB	10		// 防御の確率
#define FIFTY_FIFTY_PROB 50	// 半分の確率

#define TACKLE_MOVE_DIST	150.0f	// タックル時の移動距離
#define TACKLE_MOVE_START	0.5f	// タックル時の移動開始フレーム
#define TACKLE_MOVE_END		55.0f	// タックル時の移動終了フレーム
#define TACKLE_WAIT_DIST	10.0f	// タックル終了時の予備動作の移動距離
#define TACKLE_WAIT_START	0.0f	// タックル終了時の予備動作の開始
#define TACKLE_WAIT_END		60.0f	// タックル終了時の予備動作の終了
#define TACKLE_COOL_TIME	4.0f	// タックル攻撃のクールタイム

#define BLOW_MOVE_DIST		15.0f	// 薙ぎ払い時の移動距離
#define BLOW_MOVE_START		 1.0f	// 薙ぎ払い時の移動開始フレーム
#define BLOW_MOVE_END		30.0f	// 薙ぎ払い時の移動終了フレーム
#define KICK_MOVE_DIST		15.0f	// 回し蹴り時の移動距離
#define KICK_MOVE_START		 1.0f	// 回し蹴り時の移動開始フレーム
#define KICK_MOVE_END		50.0f	// 回し蹴り時の移動終了フレーム
#define TRIPLE_WAIT_TIME	0.005f	// 三連攻撃の間の攻撃終了時の待ち時間

#define PLAYER_ATTACK_ANGLE		35.0f	// 攻撃範囲の角度
#define PLAYER_ATTACK_LENGTH	50.0f	// 攻撃範囲の距離

#define IDLE_TIME_MIN 0.0f			// 待機時の最短待機時間
#define IDLE_TIME_MAX 6.0f			// 待機時の最長待機時間
#define BATTLE_IDLE_TIME_MIN 0.0f	// 戦闘待機時の最短待機時間
#define BATTLE_IDLE_TIME_MAX 2.0f	// 戦闘待機時の最長待機時間
#define LOST_TIME 5.0f		// 壁に引っかかった時の追跡処理有効時間

#define PATROL_INTERVAL 3.0f    // 次の巡回に移動開始するまでの時間
#define PATROL_NEAR_DIST 10.0f  // 巡回開始時に選択される巡回ポイントの最短距離

#define EATTACKWAY CPlayer::EAttackDir		// プレイヤーの攻撃方向
#define EATTACKPWOER CPlayer::EAttackPower	// プレイヤーの攻撃威力

#define GAUGE_OFFSET_Y 23.0f

// プレイヤーのアニメーションデータのテーブル
const std::vector<CEnemyBase::AnimData> ANIM_DATA =
{
	{ "",									 true,	  0.0f,	1.0f},	// Tポーズ
	{ ANIM_PATH"Idle.x",					 true,	126.0f,	1.0f},	// 待機
	{ ANIM_PATH"BattleIdle.x",				 true,	110.0f,	0.5f},	// 戦闘待機		
	{ ANIM_PATH"Walk.x",					 true,	 42.0f,	1.0f},	// 歩行
	{ ANIM_PATH"Run.x",						 true,	 23.0f,	1.0f},	// 走る
	{ ANIM_PATH"Stun.x",					false,	 74.0f,	1.0f},	// 混乱
	{ ANIM_PATH"StunWait.x",				 true,	129.0f, 1.0f},	// 混乱待ち時間
	{ ANIM_PATH"Death.x",					false,	129.0f,	1.0f},	// 死亡

	{ ATTACK_ANIM_PATH"LeftAttackS.x",		false,	 34.0f,	0.5f},	// 左薙ぎ払い
	{ ATTACK_ANIM_PATH"RightAttackS.x",		false,	 34.0f,	0.5f},	// 右薙ぎ払い
	{ ATTACK_ANIM_PATH"LeftAttackM.x",		false,	 53.0f,	0.5f},	// 左回し蹴り
	{ ATTACK_ANIM_PATH"RightAttackM.x",		false,	 53.0f,	0.5f},	// 右回し蹴り
	{ ATTACK_ANIM_PATH"Tackle.x",			false,	 56.0f,	1.0f},	// タックル
	{ ATTACK_ANIM_PATH"TackleWait.x",		false,	 71.0f,	1.5f},	// タックル終了の予備動作	
	{ ATTACK_ANIM_PATH"HeadButt.x",			false,	 68.0f,	1.0f},	// 頭突き攻撃

	{ ANIM_PATH"BattleWalkL.x",				 true,	 32.0f,	1.0f},	// 戦闘時の左歩行
	{ ANIM_PATH"BattleWalkR.x",				 true,	 32.0f,	1.0f},	// 戦闘時の右歩行

	{ ANIM_PATH"GuardIdle.x",				 true,	151.0f,	0.8f},	// 防御時の待機
	{ ANIM_PATH"GuardHit.x",				false,	 54.0f,	1.0f},	// 防御時のヒット

	{ DAMAGEHIT_PATH"Damage1.x",			false,	 42.0f,	1.0f},	// 仰け反り1
	{ DAMAGEHIT_PATH"Damage2.x",			false,	 42.0f,	1.0f},	// 仰け反り2
	{ DAMAGEHIT_PATH"Damage3.x",			false,	 42.0f,	1.0f},	// 仰け反り3
	{ CHANCEHIT_PATH"chancehit1.x",			false,	 23.0f,	1.0f},	// チャンス時1
	{ CHANCEHIT_PATH"chancehit2.x",			false,	 23.0f,	1.0f},	// チャンス時2
	{ CHANCEHIT_PATH"chancehit3.x",			false,	 23.0f,	1.0f},	// チャンス時3
	{ CHANCEHIT_PATH"chancehit4.x",			false,	 23.0f,	1.0f},	// チャンス時4

};

// プレイヤーの攻撃データのテーブル
const std::vector<CEnemyBase::AttackData> ATTACK_DATA =
{
	// 指定なし
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
	// 左薙ぎ払い
	{ EAttackDir::eLeft,	EAttackPower::eAttackS,	true, true, 0.0f, 10.0f, 16.0f, 22.0f},
	// 右薙ぎ払い
	{ EAttackDir::eRight,	EAttackPower::eAttackS,	true, true, 0.0f, 10.0f, 16.0f, 24.0f},
	// 左回し蹴り
	{ EAttackDir::eLeft,	EAttackPower::eAttackM,	true, true, 0.0f, 10.0f, 23.0f, 43.0f},
	// 右回し蹴り
	{ EAttackDir::eRight,	EAttackPower::eAttackM,	true, true, 0.0f, 10.0f, 23.0f, 43.0f},
	// タックル
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
	// タックル終了時の予備動作
	{ EAttackDir::eNone,	EAttackPower::eAttackL,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
	// 頭突き攻撃
	{ EAttackDir::eUp,		EAttackPower::eAttackL,	false, true, 0.0f, 0.0f, 20.0f, 30.0f},
	// 三連攻撃
	{ EAttackDir::eNone,	EAttackPower::eAttackS,	false, false, 0.0f, 0.0f, 0.0f, 0.0f},
};

// コンストラクタ
CEnemyA::CEnemyA(const CVector& pos, std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mPlayerAttackAngle(PLAYER_ATTACK_ANGLE)
	, mPlayerAttackLength(PLAYER_ATTACK_LENGTH)
	, mpDebugFov(nullptr)
	, mpBattleTarget(nullptr)
	, mMoveStartPos(CVector::zero)
	, mMoveEndPos(CVector::zero)
	, mIsBattle(false)
	, mIsTripleAttack(false)
	, mRandMoveAngle(0.0f)
	, mRandHitAnim(0)
	, mAttackCount(0)
	, mTackleCount(0)
	, mNextPatrolIndex(-1)
	, mNextMoveIndex(0)
{
	Position(pos);

	//この敵キャラの攻撃データを設定
	mpAttackData = &ATTACK_DATA;
	mMaxHp = ENEMY_HP;
	mHp = mMaxHp;

	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

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
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eWall});
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::ePlayer, ELayer::eAttackCol });

	// 左手のカプセルコライダ
	mpLHandCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		40.0
	);
	// 衝突するタグとレイヤーを設定
	mpLHandCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpLHandCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpLHandCol->SetEnable(false);

	// 右手のカプセルコライダ
	mpRHandCol = new CColliderSphere
	(
		this,ELayer::eAttackCol,
		40.0
	);
	// 衝突するタグとレイヤーを設定
	mpRHandCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpRHandCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpRHandCol->SetEnable(false);

	// 左足の球コライダ
	mpLFootCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		50.0f
	);
	// 衝突するタグとレイヤーを設定
	mpLFootCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpLFootCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpLFootCol->SetEnable(false);

	// 右足の球コライダ
	mpRFootCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		50.0f
	);
	// 衝突するタグとレイヤーを設定
	mpRFootCol->SetCollisionTags({ ETag::eField,ETag::ePlayer });
	mpRFootCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpRFootCol->SetEnable(false);

	// 頭の球コライダ
	mpHeadCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		25.0, true
	);
	mpHeadCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpHeadCol->SetCollisionLayers({ ELayer::eField,ELayer::ePlayer,ELayer::eAttackCol });
	mpHeadCol-> SetEnable(false);

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
	CModelXFrame* handL = mpModel->FinedFrame("Armature_mixamorig_LeftHand");
	const CMatrix& lHandMTX = handL->CombinedMatrix();
	// 右手のボーンを取得
	CModelXFrame* handR = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	const CMatrix& rHandMTX = handR->CombinedMatrix();
	// 左足のボーンを取得
	CModelXFrame* footL = mpModel->FinedFrame("Armature_mixamorig_LeftFoot");
	const CMatrix& lFootMTX = footL->CombinedMatrix();
	// 右足のボーンを取得
	CModelXFrame* footR = mpModel->FinedFrame("Armature_mixamorig_RightFoot");
	const CMatrix& rFootMTX = footR->CombinedMatrix();
	// 
	CModelXFrame* head = mpModel->FinedFrame("Armature_mixamorig_Head");
	const CMatrix& headMTX = head->CombinedMatrix();

	// 攻撃用のコライダーを行列に設定
	mpLHandCol->SetAttachMtx(&lHandMTX);
	mpRHandCol->SetAttachMtx(&rHandMTX);
	mpLFootCol->SetAttachMtx(&lFootMTX);
	mpRFootCol->SetAttachMtx(&rFootMTX);
	mpHeadCol->SetAttachMtx(&headMTX);

	// 待機最大時間をランダムで決める（１回だけだよ）
	mIdleTime = Math::Rand(0.0f, 5.0f);
	// 円運動の移動角度をランダムで決める（１回だけだよ）
	mRandMoveAngle = Math::Rand(-30.0f, 30.0f);

	// 初回に更新処理を呼んでモデルの位置を調整
	CXCharacter::Update();
}

CEnemyA::~CEnemyA()
{
	// コライダーを破棄
	SAFE_DELETE(mpLHandCol);
	SAFE_DELETE(mpRHandCol);
	SAFE_DELETE(mpLFootCol);
	SAFE_DELETE(mpRFootCol);
	SAFE_DELETE(mpHeadCol);

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
		mpNavNode->Kill();
		mpLostPlayerNode->Kill();

		//	巡回ポイントに配置したノードも全て削除
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			del->Kill();
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
	// 現在の状態に合わせて更新処理を切り替える
	switch (mState)
	{
	case (int)EState::eIdle:		UpdateIdle();		break;	// 待機状態
	case (int)EState::ePatrol:		UpdatePatrol();		break;	// 巡回状態
	case (int)EState::eBattleIdle:	UpdateBattleIdle();	break;	// 戦闘待機状態
	case (int)EState::eChase:		UpdateChase();		break;	// 追跡状態
	case (int)EState::eLost:		UpdateLost();		break;	// 見失い状態
	case (int)EState::eAttack:		UpdateAttack();		break;	// 攻撃状態
	case (int)EState::eGuard:		UpdateGuard();		break;	// 防御状態
	case (int)EState::eAvoid:		UpdateAvoid();		break;	// 回避状態
	case (int)EState::eHit:			UpdateHit();		break;	// 仰け反り状態
	case (int)EState::eStun:		UpdateStun();		break;	// 混乱状態
	case (int)EState::eStunWait:	UpdateStunWait();	break;	// 混乱待ち状態
	case (int)EState::eParried:		UpdateParried();	break;	// パリィされた状態
	case (int)EState::eDeath:		UpdateDeath();		break;	// 死亡状態
	}

	// キャラクターの更新
	CEnemyBase::Update(); 

	// 経路探索用のノード座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// コライダを更新
	mpLHandCol->Update();
	mpRHandCol->Update();
	mpLFootCol->Update();
	mpRFootCol->Update();
	mpHeadCol->Update();

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	//CDebugPrint::Print("■敵の情報\n");
	//CDebugPrint::Print("　HP：%d\n", mHp);
	//CDebugPrint::Print("　怯み度：%.2f\n", mStunPoints);
	CDebugPrint::Print("　状態：%s\n", GetStateStr(mState).c_str());
	//CDebugPrint::Print("　攻撃タイプ：%s\n", GetAttackTypeStr(mAttackType).c_str());
	//CDebugPrint::Print("　攻撃の強さ：%s\n", GetAttackPowerStr().c_str());
	//CDebugPrint::Print("　攻撃の方向：%s\n", GetAttackDirStr().c_str());
	//CDebugPrint::Print("　経過時間：%.2f\n", GetAnimationFrame());
	//CDebugPrint::Print("　戦闘時の待機時間：%.2f\n", mBattleIdletime);
	//CDebugPrint::Print("　円運動の移動角度：%.2f\n", mRandMoveAngle);
	//CDebugPrint::Print("　パリィ出来たか：%s\n", mIsAttackParry ? "パリィOK" : "パリィNO");

	mIsHitWall = false;
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
		mpRHandCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eBlowL)
	{
		mpLHandCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eRoundKickL)
	{
		mpLFootCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eRoundKickR)
	{
		mpRFootCol->SetEnable(true);
	}
	else if (mAttackType == (int)EAttackType::eHeadButt)
	{
		mpHeadCol->SetEnable(true);
	}
}

void CEnemyA::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemyBase::AttackEnd();

	// 攻撃コライダーをオフ
	mpLHandCol->SetEnable(false);
	mpRHandCol->SetEnable(false);
	mpLFootCol->SetEnable(false);
	mpRFootCol->SetEnable(false);
	mpHeadCol->SetEnable(false);
}

bool CEnemyA::IsGuarding() const
{
	if (mState == (int)EState::eGuard) return true;
	return false;
}

void CEnemyA::TakeDamage(int damage, float stan, float knockback, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CEnemyBase::TakeDamage(damage, stan, knockback, causer);

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
			LookAtBattleTarget(false);
		}
	}

}

void CEnemyA::Parry()
{
	ChangeState((int)EState::eParried);
}

// 仰け反り処理
void CEnemyA::Hit()
{
	// 仰け反り状態のアニメーションをランダムで設定
	mRandHitAnim = Math::Rand(0, 2);
	ChangeState((int)EState::eHit);
}

// 怯み処理
void CEnemyA::Stun()
{
	ChangeState((int)EState::eStun);
}

// 死亡処理
void CEnemyA::Death()
{
	ChangeState((int)EState::eDeath);
}

// 衝突処理
void CEnemyA::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayer* player = CPlayer::Instance();

	// ベースの衝突処理を呼び出す
	CEnemyBase::Collision(self, other, hit);

	if (self == mpLHandCol || self == mpRHandCol 
		|| self == mpLFootCol || self == mpRFootCol
		|| self == mpHeadCol
		)
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

				if (chara->IsGuardParry() && CheckGuardParry())
				{
					TakeDamage(0, 1000, 0.0f, chara);
				}
				else
				{
					// 与えるダメージの計算
					int damage = 0;
					float stan = 0.0f;
					float knockback = 0.0f;
					CalcDamage(chara, &damage, &stan, &knockback);

					// ダメージを与える
					chara->TakeDamage(damage, stan, knockback, this);
				}
			}
		}
	}
}

void CEnemyA::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan, float* outKnockback) const
{
	switch (mAttackType)
	{
	case (int)EAttackType::eBlowL:
		*outDamage = BLOW_DMG;
		*outStan = BLOW_STUN;
		*outKnockback = BLOW_KNOCKBACK;
		break;
	case (int)EAttackType::eBlowR:
		*outDamage = BLOW_DMG;
		*outStan = BLOW_STUN;
		*outKnockback = BLOW_KNOCKBACK;
		break;
	case (int)EAttackType::eRoundKickL:
		*outDamage = KICK_DMG;
		*outStan = KICK_STUN;
		*outKnockback = KICK_KNOCKBACK;
		break;
	case (int)EAttackType::eRoundKickR:
		*outDamage = KICK_DMG;
		*outStan = KICK_STUN;
		*outKnockback = KICK_KNOCKBACK;
		break;
	case (int)EAttackType::eTackle:
		*outDamage = TACKLE_DMG;
		*outStan = TACKLE_STUN;
		*outKnockback = TACKLE_KNOCKBACK;
		break;
	case (int)EAttackType::eHeadButt:
		*outDamage = HEADBUTT_DMG;
		*outStan = HEADBUTT_STUN;
		*outKnockback = HEADBUTT_KNOCKBACK;
		break;
	}

	if (taker->IsGuarding())
	{
		*outDamage *= 0.7f;
		*outStan *= 0.7f;
		*outKnockback *= 0.3f;
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

	if (!IsFoundPlayer()) return false;

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
bool CEnemyA::IsMoveAttackRange()
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
	// プレイヤーの攻撃範囲か？
	if (IsPlayerAttackRange()) return true;
}

// プレイヤーの攻撃を検知時の処理
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
				ChangeState((int)EState::eGuard);

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

// プレイヤーの攻撃範囲内か？
bool CEnemyA::IsPlayerAttackRange() const
{
	CPlayer* player = CPlayer::Instance();
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
		if (mElapsedTime < TACKLE_COOL_TIME)
		{
			mElapsedTime += Times::DeltaTime();
			nextstate = EState::eChase;
		}
		else
		{
			int attackRand = Math::Rand(0, 99);

			if (attackRand < TACKLE_PROB)
			{
				mElapsedTime = 0.0f;
				nextattack = EAttackType::eTackle;
				nextstate = EState::eAttack;
			}
			else
			{
				mElapsedTime = 0.0f;
				nextstate = EState::eChase;
			}
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
bool CEnemyA::ChangePatrolPoint()
{
	// 自身の経路探索用ノードが更新中の場合は、処理しない
	if (mpNavNode->IsUpdaing()) return false;
	// 巡回ポイントが設定されていない場合は、処理しない
	int size = mPatrolPoints.size();
	if (size == 0) return false;

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

	return mNextPatrolIndex >= 0;
}

bool CEnemyA::UpdatePatrolRoute()
{
	// 巡回ポイントの経路探索ノードの位置を設定し直すことで、
	// 各ノードへの接続情報を更新
	for (CNavNode* node : mPatrolPoints)
	{
		node->SetPos(node->GetPos());
	}

	if (!(0 <= mNextPatrolIndex && mNextPatrolIndex < mPatrolPoints.size())) return false;

	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr) return false;

	// 自身のノードが更新中ならば、経路探索を行わない
	if (mpNavNode->IsUpdaing()) return false;
	// 巡回ポイントが更新中ならば、経路探索を行わない
	CNavNode* patrolPoint = mPatrolPoints[mNextPatrolIndex];
	if (patrolPoint->IsUpdaing()) return false;
	// 巡回ポイントまでの最短経路を求める
	if (navMgr->Navigate(mpNavNode, patrolPoint, mMoveRoute))
	{
		// 次の目的地のインデックスを設定
		mNextMoveIndex = 1;
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
		if (ChangePatrolPoint())
		{
			mStateStep++;
		}
		break;
	// ステップ1：巡回ポイントまでの経路探索
	case 1:
		if (UpdatePatrolRoute())
		{
			mStateStep++;
		}
		break;
	// ステップ2：巡回ポイントまで移動
	case 2:
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
	// ステップ3：移動後の待機
	case 3:
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// 待機最大時間をランダムで決める
			mIdleTime = Math::Rand(IDLE_TIME_MIN, IDLE_TIME_MAX);
			if (ChangePatrolPoint())
			{
				mStateStep = 1;
				mElapsedTime = 0.0f;
			}
		}
		break;
	}
}

// 戦闘待機状態の更新処理
void CEnemyA::UpdateBattleIdle()
{
	if (DetectedPlayerAttack()) return;

	if (!IsFoundPlayer())
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// プレイヤーの座標へ向けて移動する
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();

	if (mRandMoveAngle < -10.0f)
	{
		ChangeAnimation((int)EAnimType::eBattleWalkR);
	}
	else if (mRandMoveAngle > 10.0f)
	{
		ChangeAnimation((int)EAnimType::eBattleWalkL);
	}
	else
	{
		ChangeAnimation((int)EAnimType::eBattleIdle);
	}

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
		if (mBattleElapsedTime < mBattleIdletime)
		{
			
			UpdateHorizonMove();
			//UpdateForwardMove();

			// プレイヤーが見えなくなったら、見失った状態に戻す
			if (!IsLookPlayer())
			{
				mpLostPlayerNode->SetPos(targetPos);
				ChangeState((int)EState::eLost);
				return;
			}
			mBattleElapsedTime += Times::DeltaTime();
			
		}
		else
		{
			mRandMoveAngle = Math::Rand(-30.0f, 30.0f);
			// 攻撃の種類とステートを決める
			AttackPickDetect();
			// 戦闘待機時間を初期化
			mBattleIdletime = 0.0f;
			mBattleElapsedTime = 0.0f;
		}
		break;
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
		// 自身のノードとプレイヤーのノードが更新中でなければ、
		// 自身のノードからプレイヤーのノードまでの最短距離を求める
		CNavNode* playerNode = player->GetNavNode();
		if (!mpNavNode->IsUpdaing() && 
			playerNode != nullptr && !playerNode->IsUpdaing())
		{
			if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
			{
				// 自身のノードからプレイヤーのノードまで繋がっていたら、
				// 移動する位置を次のノードの位置に設定
				targetPos = mMoveRoute[1]->GetPos();
			}
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
	CPlayer* target = CPlayer::Instance();

	if (navMgr == nullptr || target == nullptr)
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
	case 0:
		mElapsedTime = 0.0f;
		mStateStep++;
		break;
		// ステップ1：見失った位置までの最短経路を求める
	case 1:
		// 
		mpLostPlayerNode->SetPos(target->Position());

		// 自身のノードと見失った位置のノードが更新中でなければ
		if (!mpNavNode->IsUpdaing() && !mpLostPlayerNode->IsUpdaing())
		{
			if (!navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
			{
				// 経路が繋がっていなければ、待機状態へ戻す
				ChangeState((int)EState::eIdle);
				mpLostPlayerNode->SetEnable(false);

			}
			else
			{
				// 見失った位置まで経路が繋がっていたら、次のステップへ
				mNextMoveIndex = 1;
				mStateStep++;
			}
		}
		break;
	case 2:
		// 次の移動先のインデックス値が不正値でなければ
		if (0 <= mNextMoveIndex && mNextMoveIndex < mMoveRoute.size())
		{
			// プレイヤーを見失った位置まで移動
			if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
			{
				if (mNextMoveIndex >= mMoveRoute.size())
				{
					// 戦闘状態終了
					mIsBattle = false;
					mpBattleTarget = nullptr;
					// 移動が終われば、待機状態へ移行
					ChangeState((int)EState::eIdle);
				}
				else
				{
					if (mElapsedTime <= LOST_TIME)
					{
						mElapsedTime += Times::DeltaTime();
					}
					else
					{
						// 戦闘状態終了
						mIsBattle = false;
						mpBattleTarget = nullptr;
						// 移動が終われば、待機状態へ移行
						ChangeState((int)EState::eIdle);
					}
				}
			}
		}
		break;
	}
}

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
	case (int)EAttackType::eTackleWait:		UpdateTackleWait();		break;
	case (int)EAttackType::eHeadButt:		UpdateHeadButt();		break;
	case (int)EAttackType::eTripleAttack:	UpdateTripleAttack();	break;
	}



	if (mAttackType == (int)EAttackType::eNone)
	{
		ChangeState((int)EState::eBattleIdle);
	}
}

void CEnemyA::UpdateGuard()
{
	CPlayer* player = CPlayer::Instance();
	mMoveSpeed = CVector::zero;
	ChangeAnimation((int)EAnimType::eGuardIdle);
	if (player->IsAttacking())
	{
		mIsGuard = true;
	}
	else
	{
		mIsGuard = false;
	}
	if (!mIsGuard) return ChangeState((int)EState::eBattleIdle);
}

// 回避時の更新処理
void CEnemyA::UpdateAvoid()
{

}

// 仰け反り時の更新処理
void CEnemyA::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
	{
		EAnimType hitAnim = EAnimType::eHit1;
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos - VectorZ() * mKnockBack;

		if (mRandHitAnim == 0)
		{
			hitAnim = EAnimType::eHit1;
		}
		else if (mRandHitAnim == 1)
		{
			hitAnim = EAnimType::eHit2;
		}
		else if (mRandHitAnim == 2)
		{
			hitAnim = EAnimType::eHit3;
		}

		ChangeAnimation((int)hitAnim, true);
		mStateStep++;
		break;
	}
	case 1:
	{
		float frame = GetAnimationFrame();
		float moveStartFrame = mAnimationFrameSize * 0.1f;
		float moveEndFrame = mAnimationFrameSize * 0.9f;
		if (GetAnimationFrameRatio() >= 0.1)
		{
			if (GetAnimationFrameRatio() <= 0.9)
			{
				// 線形補間で移動開始位置から移動終了位置まで移動する
				float moveFrame = moveEndFrame - moveStartFrame;
				float percent = (frame - moveStartFrame) / moveFrame;
				CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent * 1.3);
				Position(pos);
			}
			else
			{
				Position(mMoveEndPos);
				mStateStep++;
			}
		}

		break;
	}
	case 2:
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// 怯み時の更新処理
void CEnemyA::UpdateStun()
{
	// TODO:アニメーションを再生、１秒後に復活、
	// 視界に入っていたら：攻撃状態に、入っていなかったら：待機状態に
	// １秒の間で攻撃をすると：攻撃チャンス！！状態へ
	switch (mStateStep)
	{
		// 怯みアニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eStun, true);
		mStateStep++;
		break;
		// アニメーション終了待ち
	case 1:
		// アニメーションが終われば待機状態へ戻す
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eStunWait);
			mStateStep++;
		}
		break;
	}
}

// 混乱待ちの更新処理
void CEnemyA::UpdateStunWait()
{
	switch (mStateStep)
	{
	case 0:
		ChangeAnimation((int)EAnimType::eStunWait);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
	case 2:
		if (mElapsedTime < STUN_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// パリィされた時の更新処理
void CEnemyA::UpdateParried()
{
	switch (mStateStep)
	{
		// 怯みアニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eStun, true);
		mStateStep++;
		break;
		// アニメーション終了待ち
	case 1:
		// アニメーションが終われば待機状態へ戻す
		if (IsAnimationFinished())
		{
			mIsAttackParry = false;
			mIsGuardParry = false;
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// 死亡時の更新処理
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


// 薙ぎ払い攻撃の更新処理
void CEnemyA::UpdateBlowL()
{
	switch (mStateStep)
	{
	case 0: // アニメーション再生
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * BLOW_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eBlowL, true);
		mStateStep++;
		break;
	case 1: // 攻撃用コライダーオン
	{
		// 攻撃アニメーションが移動開始フレームを超えた場合
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		float blowMoveStart = data.attackParryEndFrame;
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= blowMoveStart)
			{
				// 移動終了フレームまで到達してない場合
				if (frame < BLOW_MOVE_END)
				{
					// 線形補間で移動開始位置から移動終了位置まで移動する
					float moveFrame = BLOW_MOVE_END - blowMoveStart;
					float percent = (frame - blowMoveStart) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// 移動終了フレームまで到達した場合
				else
				{
					Position(mMoveEndPos);
					mStateStep++;
				}
			}
			// 攻撃コライダーがオンになるまで、プレイヤーの方向へ向く
			else
			{
				LookAtBattleTarget();
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
			if (mIsTripleAttack)
			{
				mAttackCount++;
				mStateStep++;
			}
			else
			{
				// 時間が経過したら、待機状態へ移行
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// ステップ3 : 
		mStateStep = 0;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		break;
	}
}

// 薙ぎ払い攻撃の更新処理
void CEnemyA::UpdateBlowR()
{
	switch (mStateStep)
	{
	case 0: // アニメーション再生
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * BLOW_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eBlowR, true);
		mStateStep++;
		break;
	case 1: // 攻撃用コライダーオン
	{
		// 攻撃アニメーションが移動開始フレームを超えた場合
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		float blowMoveStart = data.attackParryEndFrame;
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= blowMoveStart)
			{
				// 移動終了フレームまで到達してない場合
				if (frame < BLOW_MOVE_END)
				{
					// 線形補間で移動開始位置から移動終了位置まで移動する
					float moveFrame = BLOW_MOVE_END - blowMoveStart;
					float percent = (frame - blowMoveStart) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// 移動終了フレームまで到達した場合
				else
				{
					Position(mMoveEndPos);
					mStateStep++;
				}
			}
			// 攻撃コライダーがオンになるまで、プレイヤーの方向へ向く
			else
			{
				LookAtBattleTarget();
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
			if (mIsTripleAttack)
			{
				mAttackCount++;
				mStateStep++;
			}
			else
			{
				// 時間が経過したら、待機状態へ移行
				ChangeState((int)EState::eBattleIdle);
			}
		}
		break;
	case 3:	// ステップ3 : 
		mStateStep = 0;
		ChangeAttackType((int)EAttackType::eTripleAttack);
		break;
	}
}

// 回し蹴り攻撃の更新処理
void CEnemyA::UpdateRoundKickL()
{
	switch (mStateStep)
	{
		case 0:	// ステップ０：攻撃アニメーション、攻撃開始位置と攻撃終了位置の設定
			mMoveStartPos = Position();
			mMoveEndPos = mMoveStartPos + VectorZ() * KICK_MOVE_DIST;
			ChangeAnimation((int)EAnimType::eRoundKickL, true);
			mStateStep++;
			break;
		case 1:	// ステップ１：攻撃時の移動処理
		{
			// 攻撃アニメーションが移動開始フレームを超えた場合
			float frame = GetAnimationFrame();
			const AttackData& data = (*mpAttackData)[mAttackType];
			float kickMoveStart = data.attackParryEndFrame;
			AttackStart();
			if (!IsMoveAttackRange()) {
				if (frame >= kickMoveStart)
				{
					// 移動終了フレームまで到達してない場合
					if (frame < KICK_MOVE_END)
					{
						// 線形補間で移動開始位置から移動終了位置まで移動する
						float moveFrame = KICK_MOVE_END - kickMoveStart;
						float percent = (frame - kickMoveStart) / moveFrame;
						CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
						Position(pos);
					}
					// 移動終了フレームまで到達した場合
					else
					{
						Position(mMoveEndPos);
						mStateStep++;
					}
				}
				else
				{
					LookAtBattleTarget();
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

// 回し蹴り攻撃の更新処理
void CEnemyA::UpdateRoundKickR()
{
	switch (mStateStep)
	{
	case 0:	// ステップ０：攻撃アニメーション、攻撃開始位置と攻撃終了位置の設定
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * KICK_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eRoundKickR, true);
		mStateStep++;
		break;
	case 1:	// ステップ１：攻撃時の移動処理
	{
		// 攻撃アニメーションが移動開始フレームを超えた場合
		float frame = GetAnimationFrame();
		const AttackData& data = (*mpAttackData)[mAttackType];
		float kickMoveStart = data.attackParryEndFrame;
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= kickMoveStart)
			{
				// 移動終了フレームまで到達してない場合
				if (frame < KICK_MOVE_END)
				{
					// 線形補間で移動開始位置から移動終了位置まで移動する
					float moveFrame = KICK_MOVE_END - kickMoveStart;
					float percent = (frame - kickMoveStart) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// 移動終了フレームまで到達した場合
				else
				{
					Position(mMoveEndPos);
					mStateStep++;
				}
			}
			else
			{
				LookAtBattleTarget();
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

// タックルの更新処理
void CEnemyA::UpdateTackle()
{

	switch (mStateStep)
	{
	case 0:	// ステップ0 : 攻撃アニメーション
		// 攻撃開始位置と攻撃終了位置の設定
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * TACKLE_MOVE_DIST;
		ChangeAnimation((int)EAnimType::eTackle, false);
		mStateStep++;
		break;
	case 1:	// ステップ１：攻撃時の移動処理
	{
		// 攻撃アニメーションが移動開始フレームを超えた場合
		float frame = GetAnimationFrame();
		AttackStart();
		if (!IsMoveAttackRange()) {
			if (frame >= TACKLE_MOVE_START)
			{
				// 移動終了フレームまで到達してない場合
				if (frame < TACKLE_MOVE_END)
				{
					// 線形補間で移動開始位置から移動終了位置まで移動する
					float moveFrame = TACKLE_MOVE_END - TACKLE_MOVE_START;
					float percent = (frame - TACKLE_MOVE_START) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				// 移動終了フレームまで到達した場合
				else
				{
					Position(mMoveEndPos);
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
			// タックルの待ち状態に移行する
			ChangeAttackType((int)EAttackType::eTackleWait);
		}
		break;
	}
}

// タックル終了時の予備動作の更新処理
void CEnemyA::UpdateTackleWait()
{
	switch (mStateStep)
	{
	case 0:
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos + VectorZ() * TACKLE_WAIT_DIST;
		ChangeAnimation((int)EAnimType::eTackleWait);
		mStateStep++;
		break;
	case 1:
	{
		float frame = GetAnimationFrame();
		if (frame >= TACKLE_WAIT_START)
		{
			if (frame < TACKLE_WAIT_END)
			{
				// 線形補間で移動開始位置から移動終了位置まで移動する
				float moveFrame = TACKLE_WAIT_END - TACKLE_WAIT_START;
				float percent = (frame - TACKLE_WAIT_START) / moveFrame;
				CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
				Position(pos);
			}
			else
			{
				Position(mMoveEndPos);
				mStateStep++;
			}
		}
		break;
	}
	case 2:
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eBattleIdle);
		}
		break;
	}
}

// 頭突き攻撃の更新処理
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
		if (frame >= data.attackParryEndFrame)
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

// 三連攻撃の更新処理
void CEnemyA::UpdateTripleAttack()
{
	switch (mAttackCount)
	{
	case 0:
	{
		int attackrand = Math::Rand(0, 99);
		// 三項演算子
		ChangeAttackType(attackrand < FIFTY_FIFTY_PROB ? 
			(int)EAttackType::eBlowL :(int)EAttackType::eBlowR);
		break;
	}
	case 1:
	{
		if (mElapsedTime < TRIPLE_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			ChangeAttackType(attackrand < FIFTY_FIFTY_PROB ?
				(int)EAttackType::eBlowL : (int)EAttackType::eBlowR);
			mElapsedTime = 0;
		}
		break;
	}
	case 2:
	{
		if (mElapsedTime < TRIPLE_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			int attackrand = Math::Rand(0, 99);
			ChangeAttackType(attackrand < FIFTY_FIFTY_PROB ?
				(int)EAttackType::eBlowL : (int)EAttackType::eBlowR);
			mElapsedTime = 0;
		}
		break;
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
	case (int)EState::eIdle:		return "待機";
	case (int)EState::ePatrol:		return "巡回";
	case (int)EState::eBattleIdle:	return "待機(戦闘)";
	case (int)EState::eChase:		return "追跡";
	case (int)EState::eLost:		return "見失う";
	case (int)EState::eAttack:		return "攻撃";
	case (int)EState::eGuard:		return "防御";
	case (int)EState::eAvoid:		return "回避";
	case (int)EState::eHit:			return "仰け反り";
	case (int)EState::eStun:		return "混乱";
	case (int)EState::eStunWait:	return "混乱待ち時間";
	case (int)EState::eParried:		return "パリィ";
	case (int)EState::eDeath:		return "死亡";
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
	case (int)EAttackType::eTackle:			return "タックル";
	case (int)EAttackType::eTackleWait:		return "タックル終了時の予備動作";
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
	case (int)EState::eStun:	return CColor::cyan;
	}
	return CColor::white;
}

// 横移動(horizontal)
void CEnemyA::UpdateHorizonMove()
{
	// 自身の座標
	CVector pos = Position();
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();
	// プレイヤーから自身までのベクトルを求める
	CVector vec = pos - targetPos;

	// 壁に当たったら逆向きに移動する
	if (mIsHitWall)
	{
		float rand = Math::Rand(-10.0f, 10.0f);
		mRandMoveAngle = -mRandMoveAngle + rand;
	}


	float rotAngle = mRandMoveAngle * Times::DeltaTime();
	// 求めたベクトルをY軸に回転
	CVector rotVec = CQuaternion(0.0f, rotAngle, 0.0f) * vec;
	// プレイヤーの座標＋回転後のベクトルで、移動後の座標を求める
	pos.X(targetPos.X() + rotVec.X());
	pos.Z(targetPos.Z() + rotVec.Z());

	// プレイヤーの方を向く
	LookAtBattleTarget();

	if (!IsPlayerAttackRange()) return;

	Position(pos);

}

void CEnemyA::UpdateForwardMove()
{
	// 自身の座標
	CVector pos = Position();
	CPlayer* player = CPlayer::Instance();
	CVector targetPos = player->Position();
	// プレイヤーから自身までのベクトルを求める
	CVector vec = pos - targetPos;
	float dist = vec.Length();
	float moveForward = 15.0f * Times::DeltaTime();
	vec = vec.Normalized() * (dist - moveForward);

	pos.X(targetPos.X() + vec.X());
	pos.Z(targetPos.Z() + vec.Z());

	// プレイヤーの方を向く
	LookAtBattleTarget();

	Position(pos);

}

void CEnemyA::UpdateBackMove()
{

}
