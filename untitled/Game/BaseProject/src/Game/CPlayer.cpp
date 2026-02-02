//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CBarbarian.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGameCamera2.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CField.h"
#include "CEnemyManager.h"
#include "CGaugeUI2D.h"
#include "CAttackPowerUI.h"
#include "CInteractObject.h"

#include "CNavNode.h"
#include "CNavManager.h"
#include "CDebugFieldOfView.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

#define PLAYER_HP			 100	// プレイヤーのHP
#define PLAYER_CAP_UP		13.5f	// プレイヤーの高さ
#define PLAYER_CAP_DWON		 2.8f	// プレイヤーの底
#define PLAYER_WIDTH		 3.0f	// プレイヤーの幅

#define PLAYER_ATTACK_ANGLE		35.0f	// 攻撃範囲の角度
#define PLAYER_ATTACK_LENGTH	25.0f	// 攻撃範囲の距離

#define ATTACK1_CAP_UP		100.0f	// 攻撃コライダー1の上
#define ATTACK1_CAP_DWON	0.0f	// 攻撃コライダー1の下
#define ATTACK2_CAP_UP		0.0f	// 攻撃コライダー2の上
#define ATTACK2_CAP_DWON	-30.0f	// 攻撃コライダー2の下

#define DAMAGE_S		12.0f	// 弱攻撃のダメージ
#define DAMAGE_M		18.0f	// 中攻撃のダメージ
#define DAMAGE_L		24.0f	// 強攻撃のダメージ
#define DAMAGE_DIA		2.0f	// ダメージの倍率

#define STAN_VAL_S		10.0f	// 弱攻撃のスタン値
#define STAN_VAL_M		20.0f	// 中攻撃のスタン値
#define STAN_VAL_L		30.0f	// 強攻撃のスタン値
#define STAN_VAL_DIA	2.0f	// スタン値の倍率

#define KNOCKBACK_S		4.0f	// 弱攻撃のノックバック距離
#define KNOCKBACK_M		7.0f	// 中攻撃のノックバック距離
#define KNOCKBACK_L		10.0f	// 強攻撃のノックバック距離

#define DEATH_RPOB 40		// 死亡アニメーション確率
#define DEATH_WAIT_TIME	5.0	// 死亡の待機時間

#define WALK_SPEED	0.8f		// 歩きのスピード
#define RUN_SPEED	1.15f		// 走りのスピード
#define JUMP_WALK_SPEED 1.0f	// 歩きジャンプの移動スピード
#define JUMP_RUN_SPEED 1.3f		// 走りジャンプの移動スピード
#define JUMP_SPEED	1.5f		// ジャンプのスピード
#define JUMP_END_Y	1.0f		// ジャンプの終わり
#define GRAVITY		0.08f		// 元の重力 (0.0625)

#define AVO_MOVE_SPEED	70.0f	// 回避時の移動速度
#define AVO_MOVE_START   5.0f	// 回避時の移動開始フレーム 
#define AVO_MOVE_END    40.0f	// 回避時の移動終了フレーム 
#define AVO_COOL_TIME	0.3f	// 回避のクールタイム

#define RUN_INPUT_INTERVAL 0.15f // ダッシュキーの入力インターバル

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

// アニメーションファイルのパス
#define PLAYER_ANIM_PATH "Character\\Player\\anim\\"
// 攻撃アニメーションファイルのパス
#define ATTACK_ANIM_PATH "Character\\Player\\anim\\Attack\\"

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",								 true,	  0.0f,	 1.0f},	// 戦闘時のTポーズ
	{ PLAYER_ANIM_PATH"BattleIdle.x",	 true,	170.0f,	 1.0f},	// 戦闘時の待機

	{ PLAYER_ANIM_PATH"BattleWalk.x",	 true,	 34.0f,	10.0f},	// 歩行
	{ PLAYER_ANIM_PATH"BackWalk.x",		 true,	 39.0f,	 1.0f},	// 後ろ方向歩行
	{ PLAYER_ANIM_PATH"LeftWalk.x",		 true,	 40.0f,	 1.0f},	// 左方向歩行
	{ PLAYER_ANIM_PATH"RightWalk.x",	 true,	 35.0f,	 1.0f},	// 右方向歩行

	{ PLAYER_ANIM_PATH"Run.x",			 true,	 35.0f,	 10.0f},	// 走る
	{ PLAYER_ANIM_PATH"BackRun.x",		 true,	 26.0f,	 1.0f},	// 後ろ方向へ走る
	{ PLAYER_ANIM_PATH"LeftRun.x",		 true,	 30.0f,	 0.01f},	// 左方向へ走る
	{ PLAYER_ANIM_PATH"RightRun.x",		 true,	 30.0f,	 0.01f},	// 右方向へ走る

	{ ATTACK_ANIM_PATH"UpAttackS.x",	false,	 54.0f,	 1.3f},	// 弱上攻撃
	{ ATTACK_ANIM_PATH"UpAttackM.x",	false,	 69.0f,	 0.2f},	// 中上攻撃
	{ ATTACK_ANIM_PATH"UpAttackL.x",	false,	 92.0f,	 1.0f},	// 強上攻撃

	{ ATTACK_ANIM_PATH"RightAttackS.x",	false,	 60.0f,	 1.3f},	// 弱右攻撃
	{ ATTACK_ANIM_PATH"RightAttackM.x",	false,	 65.0f,	 1.5f},	// 中右攻撃
	{ ATTACK_ANIM_PATH"RightAttackL.x",	false,	 86.0f,	 1.0f},	// 強右攻撃

	{ ATTACK_ANIM_PATH"LeftAttackS.x",	false,	 50.0f,	 1.3f},	// 弱左攻撃
	{ ATTACK_ANIM_PATH"LeftAttackM.x",	false,	 60.0f,	 1.0f},	// 中左攻撃
	{ ATTACK_ANIM_PATH"LeftAttackL.x",	false,	 99.0f,	 1.0f},	// 強左攻撃

	{ PLAYER_ANIM_PATH"GuardStart.x",	false,	 18.0f,	 0.01f},// 防御開始
	{ PLAYER_ANIM_PATH"Guard.x",		 true,	 43.0f,	 1.0f},	// 防御中
	{ PLAYER_ANIM_PATH"GuardEnd.x",		false,	 21.0f,	 0.01f},// 防御終了
	{ PLAYER_ANIM_PATH"GuardDamage.x",	false,	 24.0f,	 1.0f},	// 防御時の仰け反り
	{ PLAYER_ANIM_PATH"GuardParry.x",	false,	 26.0f,	 1.0f},	// 防御時のパリィ
	{ PLAYER_ANIM_PATH"Avoid.x",		false,	 50.0f,	 3.0f},	// 回避

	{ PLAYER_ANIM_PATH"jump_start.x",	false,	 25.0f,	 1.0f},	// ジャンプ開始
	{ PLAYER_ANIM_PATH"jump.x",			 true,	  1.0f,	 1.0f},	// ジャンプ中
	{ PLAYER_ANIM_PATH"jump_end.x",		false,	 26.0f,	 2.0f},	// ジャンプ終了
	{ PLAYER_ANIM_PATH"jump_Attack.x",	false,	 47.0f,	 1.0f},	// ジャンプ攻撃

	{ PLAYER_ANIM_PATH"Damage1.x",		false,	 30.0f,	 1.0f},	// 仰け反り1
	{ PLAYER_ANIM_PATH"Damage2.x",		false,	 27.0f,	 1.0f},	// 仰け反り2
	{ PLAYER_ANIM_PATH"Death1.x",		false,	118.0f,	 1.0f},	// 死亡１
	{ PLAYER_ANIM_PATH"Death2.x",		false,	 70.0f,	 1.0f},	// 死亡２
};


// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eBattleIdle)
	, mSelectAttackPower(EAttackPower::eAttackM)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mElapsedDemoTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mpRideObject(nullptr)
	, mpSearchCol(nullptr)
	, mIsGrounded(false)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mIsBattleMode(true)
	, mIsAvoiding(false)
	, mIsTimeStart(false)
	, mAttackAngle(PLAYER_ATTACK_ANGLE)
	, mAttackLength(PLAYER_ATTACK_LENGTH)
{
	//インスタンスの設定
	spInstance = this;

	mMaxHp = PLAYER_HP;
	mHp = mMaxHp;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eBattleTPose);

	// プレイヤー本体コライダー
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_CAP_DWON, 0.0f),
		CVector(0.0f, PLAYER_CAP_UP, 0.0f),
		PLAYER_WIDTH, true
	);
	mpBodyCol->SetCollisionTags
	({ ETag::eEnemy, ETag::eField, ETag::eRideableObject, 
		ETag::eWall, ETag::eInteractObject, ETag::eDoor });
	mpBodyCol->SetCollisionLayers
	({ ELayer::eField, ELayer::eWall,ELayer::eAttackCol,
		ELayer::eEnemy, ELayer::eInteractObj });


	// 攻撃用のコライダ１（剣の刃の部分）
	mpAttackCol1 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK1_CAP_DWON),
		CVector(0.0f, 0.0f, ATTACK1_CAP_UP),
		1.0f, true
	);
	mpAttackCol1->SetCollisionTags({ ETag::eEnemy, ETag::eInteractObject });
	mpAttackCol1->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall, 
		ELayer::eInteractObj });
	mpAttackCol1->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol1->SetEnable(false);

	// 攻撃用のコライダ２（剣の持ち手の部分）
	mpAttackCol2 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK2_CAP_UP),
		CVector(0.0f, 0.0f, ATTACK2_CAP_DWON),
		1.0, true
	);
	mpAttackCol2->SetCollisionTags({ ETag::eEnemy, ETag::eInteractObject });
	mpAttackCol2->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall, ELayer::eInteractObj });
	mpAttackCol2->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol2->SetEnable(false);

	// 攻撃用のコライダ３（盾の部分）
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		25.0, true
	);
	mpAttackCol3->SetCollisionTags({ ETag::eEnemy, ETag::eInteractObject });
	mpAttackCol3->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall, ELayer::eInteractObj });
	mpAttackCol3->Translate(0.0f, 0.0f, -5.0f);
	mpAttackCol3->SetEnable(false);

	// 調べるオブジェクトを探知するコライダー
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		10.0f
	);
	// 調べるオブジェクトのみ衝突するように設定
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj });
	mpSearchCol->Position(0.0f, 5.3f, 0.0f);

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// 剣のボーンを取得
	CModelXFrame* Sword = mpModel->FinedFrame("Armature_mixamorig_Sword_joint");
	const CMatrix& swordMTX = Sword->CombinedMatrix();
	// 盾のボーンを取得
	CModelXFrame* Shield = mpModel->FinedFrame("Armature_mixamorig_Shield_joint");
	const CMatrix& shieldMTX = Shield->CombinedMatrix();

	// 攻撃用のコライダーを行列に設定
	mpAttackCol1->SetAttachMtx(&swordMTX);
	mpAttackCol2->SetAttachMtx(&swordMTX);
	mpAttackCol3->SetAttachMtx(&shieldMTX);

	mpHpUI = new CGaugeUI2D();
	mpHpUI->SetMaxPoint(mMaxHp);
	mpHpUI->SetCurrPoint(mHp);
	mpHpUI->SetPos(CVector2(370.0f, 660.0f));

	mpPowerUI = new CAttackPowerUI();
	mpPowerUI->SetCurrPower((int)mSelectAttackPower);
	mpPowerUI->SetPos(CVector2(100.0f, 600.0f));

	mRandDeathAnim = Math::Rand(0, 99);

	// 攻撃範囲のデバッグ表示を作成
	mpDebugAttack = new CDebugFieldOfView(this, mAttackAngle, mAttackLength);
}

CPlayer::~CPlayer()
{
	// コライダーを破棄
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpAttackCol1);
	SAFE_DELETE(mpAttackCol2);
	SAFE_DELETE(mpAttackCol3);
	SAFE_DELETE(mpSearchCol);

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		mpNavNode->Kill();
	}

	spInstance = nullptr;
}

// 攻撃中か
bool CPlayer::IsAttacking() const
{
	if (mState == EState::eAttack) return true;
	return false;
}

// 攻撃開始
void CPlayer::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();

	if (IsAttackType(EAttackPower::eAttackS, EAttackDir::eUp))
	{
		mpAttackCol2->SetEnable(true);
	}
	else if (IsAttackType(EAttackPower::eAttackS, EAttackDir::eLeft))
	{
		mpAttackCol3->SetEnable(true);
	}
	else
	{
		mpAttackCol1->SetEnable(true);
	}
}

// 攻撃終了
void CPlayer::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃コライダーをオフ
	mpAttackCol1->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
}

bool CPlayer::IsAvoiding() const
{
	if (mState == EState::eAvoid) return true;
	return false;
}

// 防御中か
bool CPlayer::IsGuarding() const
{
	if (mState == EState::eGuard) return true;
	return false;
}

bool CPlayer::IsGuardParry() const
{
	if (mState == EState::eGuardParry) return true;
	return false;
}

bool CPlayer::IsJumping() const
{
	if (mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		return true;
	}
	return false;
}

void CPlayer::TakeDamage(int damage, float stan, float knockback, CCharaBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CCharaBase::TakeDamage(damage, stan, knockback, causer);
	// 死亡しなければ
	if (!IsDeath())
	{
		// 攻撃を加えた相手の方向へ向く
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 仰け反り処理
void CPlayer::Hit()
{
	mRandHitAnim = Math::Rand(0, 1);
	ChangeState(EState::eHit);
}

// 死亡処理
void CPlayer::Death()
{
	ChangeState(EState::eDeath);
}

// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (CInput::PushKey('T'))
	{
		mIsBattleMode = !mIsBattleMode;
	}

	// TODO:通常時待機と戦闘時待機の切り替え
	//ChangeState(EState::eBattleIdle);

	// マウスホイールの回転量の差分
	int WheelDelta = CInput::GetDeltaMouseWheel();
	int powerNum = (int)EAttackPower::Num;

	// マウスホイールが上にスクロールされていたら、現在の攻撃威力をアップ
	if (WheelDelta > 0)
	{
		int power = ((int)mSelectAttackPower + 1) % powerNum;
		mSelectAttackPower = (EAttackPower)power;
	}
	// マウスホイールが下にスクロールされていたら、現在の攻撃威力ダウン
	else if (WheelDelta < 0)
	{
		int power = ((int)mSelectAttackPower + powerNum - 1) % powerNum;
		mSelectAttackPower = (EAttackPower)power;
	}

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
//	case EState::eIdle:			UpdateIdle();		break;
	case EState::eBattleIdle:	UpdateBattleIdle();	break;
	case EState::eAttack:		UpdateAttack();		break;
	case EState::eGuardStart:	UpdateGuardStart();	break;
	case EState::eGuard:		UpdateGuard();		break;
	case EState::eGuardEnd:		UpdateGuardEnd();	break;
	case EState::eGuardParry:	UpdateGuardParry();	break;
	case EState::eAvoid:		UpdateAvoid();		break;
	case EState::eJumpStart:	UpdateJumpStart();	break;
	case EState::eJump:			UpdateJump();		break;
	case EState::eJumpEnd:		UpdateJumpEnd();	break;
	case EState::eHit:			UpdateHit();		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eBattleIdle
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	if (mState == EState::eBattleIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		ChangeAttack();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// ロックオンと移動の仕方とバトルモード
	if (!mIsBattleMode)
	{
		// プレイヤーを移動方向へ向ける
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.15f); // 0.125f
		Rotation(CQuaternion::LookRotation(forward));
	}
	else if (mIsBattleMode && !mIsAvoiding)
	{
		// カメラにプレイヤーの向きを追従する
		// メインカメラを取得
		CCamera* camera = CCamera::MainCamera();
		// 注視点を取得
		CVector atPos = camera->GetAtPos();
		// 注視点とカメラの位置の差を求める
		CVector vec = atPos - camera->Position();
		vec.Y(0.0f);
		// 方向ベクトルにする
		CVector targetPos = vec.Normalized();
		Rotation(CQuaternion::LookRotation(targetPos));
		// TODO：プレイヤーを敵に向ける
		// TODO：敵に視点をボタンを押してロックするか、自動で敵に向くようにする


	}

	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// キャラクターの更新
	CXCharacter::Update();

	// コライダーの行列を変更
	mpAttackCol1->Update();
	mpAttackCol2->Update();
	mpAttackCol3->Update();

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	mIsGrounded = false;
	mIsHittingWall = false;

	mpHpUI->SetMaxPoint(mMaxHp);
	mpHpUI->SetCurrPoint(mHp);

	mpPowerUI->SetCurrPower((int)mSelectAttackPower);

	CDebugPrint::Print("FPS:%f\n \n", Times::FPS());
	CVector pos = Position();

	CDebugPrint::Print("■プレイヤーの情報\n");
	CDebugPrint::Print("　HP：%d\n", mHp);
	CDebugPrint::Print("　怯み度：%.2f\n", mStunPoints);
	CDebugPrint::Print("　回避のクールタイム：%.2f\n", mElapsedDemoTime);
//	CDebugPrint::Print("　座標：%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
//	CDebugPrint::Print("　状態：", GetStateStr(mState).c_str());
//	CDebugPrint::Print("　Grounded：%s\n", mIsGrounded ? "true" : "false");
//	CDebugPrint::Print("　選択中の攻撃の強さ：%d\n", (int)mSelectAttackPower);
//	CDebugPrint::Print("　ノックバック距離：%f\n", mKnockBack);
//	CDebugPrint::Print("　攻撃の強さ：%s\n", GetAttackPowerStr().c_str());
//	CDebugPrint::Print("　攻撃の方向：%s\n", GetAttackDirStr().c_str());
	CDebugPrint::Print("　\n");

	// 調べるオブジェクトのリストをクリア
	mNearInteractObjs.clear();
}


// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 当たり判定
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

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		else if (other->Layer() == ELayer::eWall)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			mIsHittingWall = true;
			mHitWallPos = Position() + adjust * hit.weight;
			mHitWallNormal = hit.adjust.forward.Normalized();

			// 押し戻しベクトルの分座標を移動
			Position(Position() + adjust * hit.weight);
		}
		else if (other->Layer() == ELayer::eInteractObj)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
	// 攻撃用コライダーとの当たり判定
	else if (self == mpAttackCol1 || self == mpAttackCol2 || self == mpAttackCol3)
	{
		if (other->Tag() == ETag::eEnemy && other->Layer() == ELayer::eEnemy)
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
				float knockback = 0.0f;
				CalcDamage(chara, &damage, &stan, &knockback);
				// ダメージを与える
				chara->TakeDamage(damage, stan, knockback, this);
			}
		}
		else if (other->Tag() == ETag::eInteractObject && other->Layer() == ELayer::eInteractObj)
		{
			CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
			if (obj != nullptr && !IsAttackHitObj(obj))
			{

				// 攻撃ヒット済みリストに登録
				AddAttackHitObj(obj);
				obj->Interact();
			}
		}
	}
	// 調べるオブジェクトの探知コライダーとの当たり判定
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// 衝突した調べるオブジェクトをリストに追加
			mNearInteractObjs.push_back(obj);

#if _DEBUG
			// 探知範囲内に入ったオブジェクトの名前を表示
			CDebugPrint::Print
			(
				"%s:%s\n",
				obj->GetDebugName().c_str(),
				obj->GetInteractStr().c_str()
			);
#endif
		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}

float CPlayer::GetAttackAngle() const
{
	return mAttackAngle;
}

float CPlayer::GetAttackLength() const
{
	return mAttackLength;
}


// 現在の状態を取得
CPlayer::EState CPlayer::GetState()
{
	return mState;
}


CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
}

// 攻撃方向や、攻撃の威力によって
// 切り替えるアニメーションの種類を取得
CPlayer::EAnimType CPlayer::GetAttackAnimType() const
{
	switch (mAttackDir)
	{
	case EAttackDir::eUp:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eUpAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eUpAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eUpAttackL;
		break;
	case EAttackDir::eLeft:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eLeftAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eLeftAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eLeftAttackL;
		break;
	case EAttackDir::eRight:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eRightAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eRightAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eRightAttackL;
		break;
	}
	return EAnimType::None;
}

void CPlayer::ChangeState(EState state)
{
	// すでに同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CPlayer::ChangeAvoid()
{
	// キーの入力情報から回避方向を求める
	mAvoidDir = CalcMoveVec(true);
	// 回避状態へ切り替え
	ChangeState(EState::eAvoid);
}

void CPlayer::ChangeAttack()
{

	if (mState == EState::eBattleIdle)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eLeft;
		}
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eRight;
		}
	}
	// 上入力で、上攻撃
	else if (IsJumping())
	{
		if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eUp;
		}
	}
	else if (mState == EState::eAttack)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eLeft;
		}
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eRight;
		}
	}
}

// ダメージ量を計算して返す
void CPlayer::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan, float* outKnockback) const
{
	if (mSelectAttackPower == EAttackPower::eAttackS)
	{
		// ダメージを与える
		*outDamage = DAMAGE_S;
		*outStan = STAN_VAL_S;
		*outKnockback = KNOCKBACK_S;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackM)
	{
		*outDamage = DAMAGE_M;
		*outStan = STAN_VAL_M;
		*outKnockback = KNOCKBACK_M;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackL)
	{
		*outDamage = DAMAGE_L;
		*outStan = STAN_VAL_L;
		*outKnockback = KNOCKBACK_L;
	}

	if (taker->IsGuarding())
	{
		*outDamage *= 0.7f;
		*outStan *= 0.7f;
		*outKnockback *= 0.3f;
	}
	else if (taker->IsAvoiding())
	{
		*outDamage *= 0.0f;
		*outStan *= 0.0f;
		*outKnockback *= 0.0f;
	}

	// パリィ出来るかどうか、判断する
	if (taker->CheckAttackParry(mAttackDir, mAttackPower))
	{
		if (mSelectAttackPower == EAttackPower::eAttackS)
		{
			// ダメージと怯み度を加算する
			*outDamage = DAMAGE_S * 2;
			*outStan *= STAN_VAL_DIA;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackM)
		{
			*outDamage = DAMAGE_M * 2.4f;
			*outStan *= STAN_VAL_DIA;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackL)
		{
			*outDamage = DAMAGE_L * 3;
			*outStan *= STAN_VAL_DIA;
		}
	}
	
}

//bool CPlayer::HittingWallOfForward(float range)
//{
//	CVector forward = CVector::forward.Normalized();
//
//	// 自身の正面方向のベクトルの内積を求めて角度を出す
//	float dot = CVector::Dot(mHitWallNormal, forward);
//	// 視野角度のラジアンを求める
//	float angleR = Math::DegreeToRadian(range);
//	// 求めた内積と視野角度で、視野範囲か判断する
//	if (dot < cosf(angleR)) return true;
//
//	return false;
//}

// 非戦闘時の待機状態
//void CPlayer::UpdateIdle()
//{
//	// 接地していれば、
//	if (mIsGrounded)
//	{
//		// SPACEキーでジャンプ開始へ移行
//		if (CInput::PushKey(VK_SPACE))
//		{
//			mState = EState::eJumpStart;
//		}
//	}
//}

// 戦闘時の待機状態
void CPlayer::UpdateBattleIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		if (CInput::Key(VK_SHIFT))
		{
			ChangeState(EState::eGuardStart);
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
		else if (CInput::Key('I'))
		{
			CInteractObject* obj = GetNearInteractObj();

			if (obj == nullptr) return;

			obj->Interact();
		}

		// シフトで回避

		if (mIsTimeStart)
		{
			// クールタイムを計測
			if (mElapsedDemoTime > AVO_COOL_TIME )
			{
				if (CInput::Key('F'))
				{
					mIsTimeStart = false;
					ChangeAvoid();
					mElapsedDemoTime = 0.0f;
				}
			}
			else
			{
				mElapsedDemoTime += Times::DeltaTime();
			}
		}
		else
		{
			if (CInput::Key('F'))
			{
				ChangeAvoid();
			}
		}
	}
}

// 攻撃
void CPlayer::UpdateAttack()
{
	// プレイヤーの攻撃状態に合わせてアニメーションを切り替える
	EAnimType anim = GetAttackAnimType();

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(anim, true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrameRatio() >=  0.25f)
		{
			// 攻撃開始
			AttackStart();
			mStateStep++;
		}
		break;
	case 2:
		if (GetAnimationFrameRatio() >= 0.75f)
		{
			ChangeAttack();
			// 攻撃終了
			AttackEnd();
			mStateStep++;
		}
		break;
	case 3:
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			// 戦闘時の待機状態へ移行
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}

void CPlayer::UpdateGuardStart()
{
	switch (mStateStep)
	{
	case 0:
		mMoveSpeed = CVector::zero;
		ChangeAnimation(EAnimType::eGuardStart);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eGuard);
		}

		if (CInput::PullKey(VK_SHIFT))
		{
			ChangeState(EState::eGuardEnd);
		}
		break;
	}
}

// 防御
void CPlayer::UpdateGuard()
{
	mMoveSpeed = CVector::zero;
	ChangeAnimation(EAnimType::eGuard);

	if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
	{
		ChangeState(EState::eGuardParry);
	}

	if (CInput::PullKey(VK_SHIFT))
	{
		ChangeState(EState::eGuardEnd);
	}
}

void CPlayer::UpdateGuardEnd()
{
	switch (mStateStep)
	{
	case 0:
		mMoveSpeed = CVector::zero;
		ChangeAnimation(EAnimType::eGuardEnd);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}


void CPlayer::UpdateGuardParry()
{

	switch (mStateStep)
	{
	case 0:
		ChangeAnimation(EAnimType::eGuardParry);
		mStateStep++;
		break;
	case 1:
		if (IsAnimationFinished())
		{
			ChangeState(EState::eGuard);
		}
		break;
	}
	if (CInput::PullKey(VK_SHIFT))
	{
		ChangeState(EState::eBattleIdle);
	}
}

// 回避
void CPlayer::UpdateAvoid()
{
	mMoveSpeed = CVector::zero;

	switch (mStateStep)
	{
	case 0:
		mIsAvoiding = true;
		ChangeAnimation(EAnimType::eAvoid);
		mStateStep++;
		break;
	case 1:
	{
		float frame = GetAnimationFrame();
		if (frame >= AVO_MOVE_START)
		{
			if (frame < AVO_MOVE_END)
			{
				CVector current = VectorZ();
				CVector forward = CVector::Slerp(current, mAvoidDir, 0.5);
				Rotation(CQuaternion::LookRotation(forward));
				// 回避時の移動速度を求める
				mMoveSpeed = mAvoidDir * AVO_MOVE_SPEED * Times::DeltaTime();
			}
			else
			{
				mStateStep++;
			}
		}
		break;
	}
	case 2:
		if (IsAnimationFinished())
		{
			mIsAvoiding = false;
			mIsTimeStart = true;
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}

// ジャンプ開始
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState(EState::eJumpEnd);
	}
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eBattleIdle);
	}
}

// キーの入力情報から移動ベクトルを求める
CVector CPlayer::CalcMoveVec(bool force) const
{
	CVector move = CVector::zero;

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	float inputLen = input.LengthSqr();
	// 強制で移動するモードで、どのキーも入力されていなければ
	// 正面方向へ強制移動する
	if (force && inputLen == 0.0f)
	{
		input.Y(-1.0f);
		inputLen = 1.0f;
	}

	// 入力ベクトルの長さで入力されているか判定
	if (inputLen > 0.0f)
	{
		// 上方向ベクトル(設置している場合は、地面の法線)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// カメラの向きに合わせた移動ベクトルに変換
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// カメラの正面方向ベクトルと上方向ベクトルの外積から
		// 横方向の移動ベクトルを求める
		CVector moveSide = CVector::Cross(up, camForward);
		// 横方向の移動ベクトルと上方向ベクトルの外積から
		// 正面方向の移動ベクトルを求める
		CVector moveForward = CVector::Cross(moveSide, up);

		// 求めた各方向の移動ベクトルから、
		// 最終的なプレイヤーの移動ベクトルを求める
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}


// 移動の更新処理
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// このフレームで押された移動キーを記憶しておく
	int pushKey = -1;
	if (CInput::PushKey('W')) pushKey = 'W';
	if (CInput::PushKey('S')) pushKey = 'S';
	if (CInput::PushKey('A')) pushKey = 'A';
	if (CInput::PushKey('D')) pushKey = 'D';

	// ダッシュキーが登録されているかどうか
	if (mPushRunKey != -1)
	{
		// ダッシュ中でない
		if (!mIsRun)
		{
			// 移動開始してからの経過時間を計測
			mMoveElapsedTime += Times::DeltaTime();
			// 再度いずれかの移動キーが入力された
			if (pushKey != -1)
			{
				// １回目に押されたキーと同じ場合は、ダッシュ開始
				if (pushKey == mPushRunKey)
				{
					mIsRun = true;
				}
				// 違うキーだった場合は、ダッシュキーの入力状態を解除
				else
				{
					mPushRunKey = pushKey;
					mMoveElapsedTime = 0.0f;
				}
			}
		}
	}
	// ダッシュキーが入力されていない場合
	else
	{
		// 今回入力されたキーをダッシュキーに登録
		mPushRunKey = pushKey;
	}

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		if (IsJumping())
		{
			float speed = (mIsRun ? JUMP_RUN_SPEED : JUMP_WALK_SPEED);
			mMoveSpeed = move * speed;
		}
		else
		{
			float speed = (mIsRun ? RUN_SPEED : WALK_SPEED);
			mMoveSpeed = move * speed;
		}


		// 戦闘待機状態時の歩行アニメーション
		if (mState == EState::eBattleIdle)
		{
			EAnimType moveAnim = EAnimType::eBattleWalk;
			if (CInput::Key('A'))
			{
				mIsRun ? moveAnim = EAnimType::eLeftRun : moveAnim = EAnimType::eBattleLeftWalk;
			}
			else if (CInput::Key('D'))
			{
				mIsRun ? moveAnim = EAnimType::eRightRun : moveAnim = EAnimType::eBattleRightWalk;
			}
			else if (CInput::Key('W'))
			{
				mIsRun ? moveAnim = EAnimType::eRun : moveAnim = EAnimType::eBattleWalk;
			}
			else if (CInput::Key('S'))
			{
				mIsRun ? moveAnim = EAnimType::eBackRun : moveAnim = EAnimType::eBattleBackWalk;
			}

			ChangeAnimation(moveAnim);
		}
	}
	// 移動キーを入力していない
	else
	{
		if (mState == EState::eBattleIdle)
		{
			ChangeAnimation(EAnimType::eBattleIdle);
		}
		// ダッシュ中もしくわ、
		// 移動開始からダッシュキーの入力インターバルを超えた場合
		if (mIsRun || mMoveElapsedTime > RUN_INPUT_INTERVAL)
		{
			mIsRun = false;
			mPushRunKey = -1;
			mMoveElapsedTime = 0.0f;
		}
	}
}

void CPlayer::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
	{
		EAnimType animType = EAnimType::eHit1;
		mMoveStartPos = Position();
		mMoveEndPos = mMoveStartPos - VectorZ() * mKnockBack;

		if (mRandHitAnim == 0)
		{
			animType = EAnimType::eHit1;
		}
		else if (mRandHitAnim == 1)
		{
			animType = EAnimType::eHit2;
		}
		ChangeAnimation(animType, true);
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
				if (!mIsHittingWall)
				{
					// 線形補間で移動開始位置から移動終了位置まで移動する
					float moveFrame = moveEndFrame - moveStartFrame;
					float percent = (frame - moveStartFrame) / moveFrame;
					CVector pos = CVector::Lerp(mMoveStartPos, mMoveEndPos, percent);
					Position(pos);
				}
				else
				{
					Position(mHitWallPos);
					mStateStep++;
				}
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
			ChangeState(EState::eBattleIdle);
		}
		break;
	}
}

void CPlayer::UpdateDeath()
{
	if (mRandDeathAnim < DEATH_RPOB)
	{
		ChangeAnimation(EAnimType::eDeath1);
		mStateStep++;
	}
	else
	{
		ChangeAnimation(EAnimType::eDeath2);
		mStateStep++;
	}
}


std::string CPlayer::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eBattleIdle:	return "待機(戦闘)";
	case EState::eAttack:		return "攻撃";
	case EState::eGuard:		return "防御";
	case EState::eGuardParry:	return "防御パリィ";
	case EState::eAvoid:		return "回避";
	case EState::eHit:			return "仰け反り";
	case EState::eStun:			return "気絶";
	case EState::eDeath:		return "死亡";
	}
	return "";
}

CInteractObject* CPlayer::GetNearInteractObj() const
{
	// 一番近くの調べるオブジェクトのポイント
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// 現在一番近くにある調べつオブジェクトまでの距離
	CVector pos = Position();
	// 探知範囲内の調べるオブジェクトを順番に調べる
	for (CInteractObject* obj : mNearInteractObjs)
	{
		// 現在調べられない状態であれば、スルー
		if (!obj->CanInteract()) continue;

		float dist = (obj->Position() - pos).LengthSqr();
		// 一番最初の調べるオブジェクトが、
		// 求めた距離が現在の一番近いオブジェクトよりも近い場合は、
		if (nearObj == nullptr || dist < nearDist)
		{
			// 一番近いオブジェクトを更新
			nearObj = obj;
			nearDist = dist;
		}
	}
	return nearObj;
}
