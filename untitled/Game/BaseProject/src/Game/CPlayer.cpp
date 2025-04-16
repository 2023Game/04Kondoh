//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CEnemyA.h"
#include "CEnemyManager.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGameCamera2.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CField.h"
#include "CDebugFieldOfView.h"

#include "CEnemyManager.h"
#include "CEnemyA.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;



#define PLAYER_CAP_UP    13.5f     // プレイヤーの高さ
#define PLAYER_CAP_DWON   2.5f     // プレイヤーの底
#define PLAYER_WIDTH      2.0f     // プレイヤーの幅

#define ATTACK1_CAP_UP		80.0f	// 攻撃コライダー1の上
#define ATTACK1_CAP_DWON	0.0f	// 攻撃コライダー1の下
#define ATTACK2_CAP_UP		0.0f	// 攻撃コライダー2の上
#define ATTACK2_CAP_DWON	-30.0f	// 攻撃コライダー2の下
#define ATTACK_START_FRAME	24.0f	// 攻撃開始フレーム
#define ATTACK_END_FRAME	50.0f	// 攻撃終了フレーム
#define ATTACK_LENGTH		10.0f	// 攻撃範囲の距離
#define ATTACK_ANGLE		30.0f	// 攻撃範囲の角度

#define MOVE_SPEED 0.375f * 2.0f
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f  
#define EVA_MOVE_SPEED 200.0f  // 回避時の移動速度
#define EVA_MOVE_START   8.0f  // 回避時の移動開始フレーム 
#define EVA_MOVE_END    24.0f  // 回避時の移動終了フレーム
#define EVA_WAIT_TIME    0.1f  // 回避終了時の待機時間

// モーションブラーを掛ける時間
#define MOTION_BLUR_TIME 3.0f
// モーションブラーの幅
#define MOTION_BLUR_WIDTH 1.0f
// モーションブラーの反復回数
#define MOTION_BLUR_COUNT 5

#define PLAYER_ANIM_PATH "Character\\New Player\\anim\\"
// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",								true,	0.0f,	1.0f	},	// 戦闘時のTポーズ
	{ PLAYER_ANIM_PATH"AttackIdle.x",	true,	170.0f,	1.0f	},	// 戦闘時の待機

	{ PLAYER_ANIM_PATH"AttackWalk.x",	true,	34.0f,	10.0f	},	// 歩行
	{ PLAYER_ANIM_PATH"BackWalk.x",		true,	39.0f,	1.0f	},	// 後ろ方向歩行
	{ PLAYER_ANIM_PATH"LeftWalk.x",		true,	40.0f,	1.0f	},	// 左方向歩行
	{ PLAYER_ANIM_PATH"RightWalk.x",	true,	35.0f,	1.0f	},	// 右方向歩行

	{ PLAYER_ANIM_PATH"UpAttackS.x",	false,	54.0f,	1.0f	},	// 弱上攻撃
	{ PLAYER_ANIM_PATH"UpAttackM.x",	false,	69.0f,	0.1f	},	// 中上攻撃
	{ PLAYER_ANIM_PATH"UpAttackL.x",	false,	92.0f,	1.0f	},	// 強上攻撃

	{ PLAYER_ANIM_PATH"DwonAttackS.x",	false,	50.0f,	1.0f	},	// 弱下攻撃
	{ PLAYER_ANIM_PATH"DwonAttackM.x",	false,	55.0f,	1.0f	},	// 中下攻撃
	{ PLAYER_ANIM_PATH"DwonAttackL.x",	false,	90.0f,	1.0f	},	// 強下攻撃

	{ PLAYER_ANIM_PATH"RightAttackS.x",	false,	60.0f,	1.0f	},	// 弱右攻撃
	{ PLAYER_ANIM_PATH"RightAttackM.x",	false,	65.0f,	1.5f	},	// 中右攻撃
	{ PLAYER_ANIM_PATH"RightAttackL.x",	false,	86.0f,	1.0f	},	// 強右攻撃

	{ PLAYER_ANIM_PATH"LeftAttackS.x",	false,	50.0f,	1.0f	},	// 弱左攻撃
	{ PLAYER_ANIM_PATH"LeftAttackM.x",	false,	60.0f,	1.0f	},	// 中左攻撃
	{ PLAYER_ANIM_PATH"LeftAttackL.x",	false,	99.0f,	1.0f	},	// 強左攻撃

	{ PLAYER_ANIM_PATH"Defense.x",		true,	43.0f,	1.0f	},  // 防御
	{ PLAYER_ANIM_PATH"Evasion.x",		false,	50.0f,	1.0f	},  // 回避

	{ PLAYER_ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// ジャンプ開始
	{ PLAYER_ANIM_PATH"jump.x",			true,	1.0f,	1.0f	},	// ジャンプ中
	{ PLAYER_ANIM_PATH"jump_end.x",		false,	26.0f,	1.0f	},	// ジャンプ終了
	{ PLAYER_ANIM_PATH"jump_Attack.x",	false,	47.0f,	1.0f	},	// ジャンプ攻撃
};


// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mSelectAttackPower(EAttackPower::eAttackM)
	, mMode(EMode::eBattle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mpRideObject(nullptr)
	, mpLockOnTarget(nullptr)
	, mIsGrounded(false)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mIsLockOn(false)
{
	//インスタンスの設定
	spInstance = this;

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
	ChangeAnimation(EAnimType::eAttackTPose);

	// カプセルコライダー作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_CAP_DWON, 0.0f),
		CVector(0.0f, PLAYER_CAP_UP, 0.0f),
		PLAYER_WIDTH, true
	);
	mpBodyCol->SetCollisionTags({ ETag::eEnemy, ETag::eField, ETag::eWall});
	mpBodyCol->SetCollisionLayers
	({ ELayer::eField, ELayer::eWall,ELayer::eAttackCol,ELayer::eEnemy });


	// 攻撃用のコライダ１（剣の刃の部分）
	mpAttackCol1 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK1_CAP_DWON),
		CVector(0.0f, 0.0f, ATTACK1_CAP_UP),
		0.7f, true
	);
	mpAttackCol1->SetCollisionTags({ ETag::eEnemy });
	mpAttackCol1->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall});
	mpAttackCol1->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol1->SetEnable(false);

	// 攻撃用のコライダ２（剣の持ち手の部分）
	mpAttackCol2 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK2_CAP_UP),
		CVector(0.0f, 0.0f, ATTACK2_CAP_DWON),
		0.8, true
	);
	mpAttackCol2->SetCollisionTags({ ETag::eEnemy });
	mpAttackCol2->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall});
	mpAttackCol2->Rotate(CVector(-4.5f, 14.1f, 0.0f));
	mpAttackCol2->SetEnable(false);

	// 攻撃用のコライダ３（盾の部分）
	mpAttackCol3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		15.0, true
	);
	mpAttackCol3->SetCollisionTags({ ETag::eEnemy });
	mpAttackCol3->SetCollisionLayers({ ELayer::eEnemy, ELayer::eWall});
	mpAttackCol3->Translate(0.0f, 0.0f, -12.0f);
	mpAttackCol3->SetEnable(false);

	// 視野範囲のデバッグ表示を作成
	mpDebugAttack = new CDebugFieldOfView(this, mAttackAngle, mAttackLength);

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


}

CPlayer::~CPlayer()
{
	// コライダーを破棄
	SAFE_DELETE(mpBodyCol);

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
	}

	spInstance = nullptr;
}


bool CPlayer::IsAttacking() const
{
	return mState == EState::eAttack
		|| mState == EState::eAttackWait;
}

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

void CPlayer::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃コライダーをオフ
	mpAttackCol1->SetEnable(false);
	mpAttackCol2->SetEnable(false);
	mpAttackCol3->SetEnable(false);
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
	case EAttackDir::eDown:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eDwonAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eDwonAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eDwonAttackL;
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

void CPlayer::ChangeEvasion()
{
	// キーの入力情報から回避方向を求める
	mEvaDir = CalcMoveVec(true);
	// 回避状態へ切り替え
	ChangeState(EState::eEvasion);
}

void CPlayer::ChangeAttack()
{
	
	if (mState == EState::eIdle)
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
	else if (mState == EState::eJump
		|| mState == EState::eJumpStart
		|| mState == EState::eJumpEnd)
	{
		if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eUp;
		}
	}
	
	// 下入力で、下攻撃
	else if (mState == EState::eDefense)
	{
		if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
			mAttackPower = mSelectAttackPower;
			mAttackDir = EAttackDir::eDown;
		}
	}
}


//void CPlayer::LockOnTarget()
//{
//	// ロックオン開始
//	if (mIsLockOn)
//	{
//		// ロックオンする敵を取得
//		CEnemyBase* target = CEnemyManager::Instance()->FindLockOnTarget(45, 300);
//		mpLockOnTarget = target;
//		// ロックオンする敵が存在する
//		if (mpLockOnTarget != nullptr )
//		{
//				if (CInput::PushKey('B'))
//				{
//					// TODO:ロックオンする敵を変更
//				}
//				// ロックオン処理
//				target->Position();
//		}
//		// ロックオンする敵が存在しなかったら、
//		// バトルモードフラグをオフにする
//		else
//		{
//			mIsLockOn = false;
//		}
//	}
//	else
//	{
//		mpLockOnTarget = nullptr;
//	}
//}


// ダメージ量を計算して返す
void CPlayer::CalcDamage(CCharaBase* taker, int* outDamage, float* outStan) const
{
	
	if (mSelectAttackPower == EAttackPower::eAttackS)
	{
		// ダメージを与える
		*outDamage = 1;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackM)
	{
		*outDamage = 2;
	}
	else if (mSelectAttackPower == EAttackPower::eAttackL)
	{
		*outDamage = 3;
	}

	// パリィ出来るかどうか、判断する
	if (taker->CheckParry(mAttackDir, mAttackPower))
	{
		if (mSelectAttackPower == EAttackPower::eAttackS)
		{
			*outDamage *= 2;
			// 怯み度を加算する
			*outStan = 10.0f;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackM)
		{
			*outDamage *= 2;
			*outStan = 50.0f;
		}
		else if (mSelectAttackPower == EAttackPower::eAttackL)
		{
			*outDamage *= 2;
			*outStan = 100.0f;
		}
	}
	
}

// 非戦闘時の待機状態
void CPlayer::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// SPACEキーでジャンプ開始へ移行
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
}

// 戦闘時の待機状態
void CPlayer::UpdateAttackIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		////左クリックで攻撃
		//if (CInput::PushKey(VK_LBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack);
		//	mAttackPower = mSelectAttackPower;
		//	mAttackDir = EAttackDir::eLeft;
		//	ChangeAttack();
		//}
		//else if (CInput::PushKey(VK_RBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack);
		//	mAttackPower = mSelectAttackPower;
		//	mAttackDir = EAttackDir::eRight;
		//}
		if (CInput::Key(VK_SHIFT))
		{
			ChangeState(EState::eDefense);
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
		// シフトで回避
		else if (CInput::Key('F'))
		{
			ChangeEvasion();
		}
		// まだ使えない
		//else if (CInput::PushKey('R'))
		//{
		//	mIsLockOn = !mIsLockOn;
		//	LockOnTarget();
		//}
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
			// 攻撃終了
			AttackEnd();
			mStateStep++;
		}
		break;
	case 3:
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eAttackIdle);
		}
		break;
	}
}

// 攻撃終了待ち
//void CPlayer::UpdateAttackWait()
//{
//	// 斬撃SEを再生していないかつ、アニメーションが25%以上進行したら、
//	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
//	{
//		// 斬撃SEを再生
//		mpSlashSE->Play();
//		mIsPlayedSlashSE = true;
//	}
//
//	// 斬撃エフェクトを生成していないかつ、アニメーションが35%以上進行したら、
//	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
//	{
//		// 斬撃エフェクトを生成して、正面方向へ飛ばす
//		CSlash* slash = new CSlash
//		(
//			this,
//			Position() + CVector(0.0f, 10.0f, 0.0f),
//			VectorZ(),
//			300.0f,
//			100.0f
//		);
//		// 斬撃エフェクトの色設定
//		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));
//
//		mIsSpawnedSlashEffect = true;
//	}
//
//	// 攻撃アニメーションが終了したら、
//	if (IsAnimationFinished())
//	{
//		// 待機状態へ移行
//		mState = EState::eIdle;
//		mAttackWay = EAttackWay::eIdle;
//		ChangeAnimation(EAnimType::eAttackIdle);
//	}
//}

// 回避
void CPlayer::UpdateEvasion()
{
	mMoveSpeed = CVector::zero;

	switch (mStateStep)
	{
	case 0:
		
		ChangeAnimation(EAnimType::eEvasion);
		mStateStep++;
		break;
	case 1:
	{
		float frame = GetAnimationFrame();
		if (frame >= EVA_MOVE_START)
		{
			if (frame < EVA_MOVE_END)
			{
				// 回避時の移動速度を求める
				mMoveSpeed = mEvaDir * EVA_MOVE_SPEED * Times::DeltaTime();
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
			mStateStep++;
		}
		break;
	case 3:
		if (mElapsedTime < EVA_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// 時間が経過したら、待機状態へ移行
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// 防御
void CPlayer::UpdateDefense()
{
	mMoveSpeed = CVector::zero;
	ChangeAnimation(EAnimType::eDefense);
	if (CInput::PullKey(VK_RBUTTON))
	{
		mState = EState::eIdle;
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
		ChangeState(EState::eIdle);
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

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
			mMoveSpeed += move * MOVE_SPEED;

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle && mMode == EMode::eBattle)
		{
			if (CInput::Key('W'))
			{
				ChangeAnimation(EAnimType::eAttackWalk);
			}
			else if (CInput::Key('S'))
			{
				ChangeAnimation(EAnimType::eAttackBackWalk);
			}
			else if (CInput::Key('A'))
			{
				ChangeAnimation(EAnimType::eAttackLeftWalk);
			}
			else if (CInput::Key('D'))
			{
				ChangeAnimation(EAnimType::eAttackRightWalk);
			}
		}
		else if (mState == EState::eIdle && mMode == EMode::eNotBattle)
		{
			ChangeAnimation(EAnimType::eAttackWalk);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eAttackIdle);
		}
	}
}

bool CPlayer::IsAttackRange() const
{
	// 敵が存在しない場合は、範囲外とする
	CEnemyManager* enemy = CEnemyManager::Instance();
	if (enemy == nullptr) return false;

	// プレイヤー座標の取得
	CVector enemyPos = enemy->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自身からプレイヤーまでのベクトルを求める
	CVector vec = pos - playerPos;
	vec.Y(0.0f);

	// 1: 攻撃範囲の角度内か求める
	// ベクトルを正規化して方向要素のみにするため
	// 長さを１にする
	CVector dir = pos.Normalized();
	// 自身の正面方向のベクトルを取得
	CVector forward = VectorZ();
	// プレイヤーまでのベクトルと
	// 自身の正面方向のベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野角度のラジアンを求める
	float angleR = Math::DegreeToRadian(mAttackAngle);
	// 求めた内積と視野角度で、視野範囲か判断する
	if (dot < cosf(angleR)) return false;

	// 2: 視野距離内か求める
	// 終わりまでの距離と攻撃範囲の距離で、攻撃範囲内か判断する
	float dist = Length();
	if (dist > mAttackLength) return false;

	return true;
}

// ↓ポーションブラーの更新処理、使ってない
/*
// モーションブラーの更新処理
void CPlayer::UpdateMotionBlur()
{
	// モーションブラーの残り時間が残っていなければ、処理しない
	if (mMotionBlurRemainTime <= 0.0f) return;
	// 現在のカメラを取得し、存在しなければ処理しない
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// カメラの向きと反対方向へブラーを掛けるため、
	// 反転したカメラの向きを取得
	CVector camDir = -currentCamera->VectorZ();

	// 残り時間から経過時間の割合を取得（経過時間の割合 = 1 - 残り時間の割合）
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// ブラーの幅をサインカーブで経過時間に合わせて増減させる
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// モーションブラーのパラメータを設定
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// 残り時間を経過時間分減少させる
	mMotionBlurRemainTime -= Times::DeltaTime();
	// 残り時間がなくなれば、
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// モーションブラーをオフにする
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}
*/

// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (CInput::PushKey('T'))
	{
		mIsBattleMode = !mIsBattleMode;
	}
	// バトルモードかどうか
	if (mIsBattleMode)
	{
		mMode = EMode::eBattle;
	}
	else
	{
		mMode = EMode::eNotBattle;
	}

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
		// 待機状態
	case EState::eIdle:
		if (mMode == EMode::eBattle)
		{
			UpdateAttackIdle();
		}
		else
		{
			UpdateIdle();
		}
		break;
		// 攻撃
	case EState::eAttack:
		UpdateAttack();
		break;
		// 攻撃終了待ち
	//case EState::eAttackWait:
	//	UpdateAttackWait();
		break;
		// 防御
	case EState::eDefense:
		UpdateDefense();
		break;
		// 回避
	case EState::eEvasion:
		UpdateEvasion();
		break;
		// ジャンプ開始
	case EState::eJumpStart:
		UpdateJumpStart();
		break;
		// ジャンプ中
	case EState::eJump:
		UpdateJump();
		break;
		// ジャンプ終了
	case EState::eJumpEnd:
		UpdateJumpEnd();
		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd
		|| mState == EState::eDefense)
	{
		ChangeAttack();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// ロックオンと移動の仕方とバトルモード
	if (!(mMode == EMode::eBattle))
	{
		// プレイヤーを移動方向へ向ける
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.15f); // 0.125f
		Rotation(CQuaternion::LookRotation(forward));
	}
	else
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

	// ↓炎や弾丸、モーションブラーなどの処理、使ってない
	/* 
	// 右クリックで弾丸発射
	if (CInput::PushKey(VK_RBUTTON))
	{
		 //弾丸を生成
		new CBullet
		(
			// 発射位置
			Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 20.0f,
			VectorZ(),	// 発射方向
			1000.0f,	// 移動距離
			1000.0f		// 飛距離
		);
	}

	 「E」キーで炎の発射をオンオフする
	if (CInput::PushKey('E'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
	}

	 「B」キーを押したら、モーションブラー開始
	if (CInput::PushKey('B'))
	{
		// モーションブラーを掛けている最中であれば、
		// 新しくモーションブラーを掛け直さない
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}

	 モーションブラーの更新処理
	UpdateMotionBlur();

	*/

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

	CDebugPrint::Print("FPS:%f\n \n", Times::FPS());

	CDebugPrint::Print("■プレイヤーの情報\n");
	CDebugPrint::Print("　Grounded：%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("　選択中の攻撃の強さ：%d\n", (int)mSelectAttackPower);

	CDebugPrint::Print("　攻撃の強さ：%s\n", GetAttackPowerStr().c_str());
	CDebugPrint::Print("　攻撃の方向：%s\n", GetAttackDirStr().c_str());
	CDebugPrint::Print("　\n");

	mIsGrounded = false;
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
			//adjust.Y(0.0f);

			// 押し戻しベクトルの分座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
	else if (self == mpAttackCol1 || self == mpAttackCol2 || self == mpAttackCol3)
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
			CalcDamage(chara, &damage, &stan);

			// ダメージを与える
			chara->TakeDamage(damage, stan, this);

		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}
