//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CEnemyA.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;



#define PLAYER_CAP_UP    13.5f     // プレイヤーの高さ
#define PLAYER_CAP_DWON   2.5f     // プレイヤーの底
#define PLAYER_WIDTH      2.0f     // プレイヤーの幅
#define ATTACK1_CAP_UP     80.0f     // 攻撃コライダー1の上
#define ATTACK1_CAP_DWON    0.0f     // 攻撃コライダー1の下
#define ATTACK2_CAP_UP      0.0f     // 攻撃コライダー2の上
#define ATTACK2_CAP_DWON  -30.0f     // 攻撃コライダー2の下
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
	{ "",							        true,	0.0f	},	// 戦闘時のTポーズ
	{ PLAYER_ANIM_PATH"AttackIdle.x",	    true,	170.0f	},	// 戦闘時の待機

	{ PLAYER_ANIM_PATH"AttackWalk.x",	    true,	34.0f	},	// 歩行
	{ PLAYER_ANIM_PATH"BackWalk.x",	        true,	39.0f	},	// 後ろ方向歩行
	{ PLAYER_ANIM_PATH"LeftWalk.x",	        true,	40.0f	},	// 左方向歩行
	{ PLAYER_ANIM_PATH"RightWalk.x",	    true,	35.0f	},	// 右方向歩行

	{ PLAYER_ANIM_PATH"UpAttackS.x",	    false,	54.0f	},	// 弱上攻撃
	{ PLAYER_ANIM_PATH"UpAttackM.x",	    false,	69.0f	},	// 中上攻撃
	{ PLAYER_ANIM_PATH"UpAttackL.x",	    false,	92.0f	},	// 強上攻撃

	{ PLAYER_ANIM_PATH"DwonAttackS.x",	   false,	50.0f	},	// 弱下攻撃
	{ PLAYER_ANIM_PATH"DwonAttackM.x",	   false,	55.0f	},	// 中下攻撃
	{ PLAYER_ANIM_PATH"DwonAttackL.x",	   false,	90.0f	},	// 強下攻撃

	{ PLAYER_ANIM_PATH"RightAttackS.x",	   false,	60.0f	},	// 弱右攻撃
	{ PLAYER_ANIM_PATH"RightAttackM.x",	   false,	65.0f	},	// 中右攻撃
	{ PLAYER_ANIM_PATH"RightAttackL.x",	   false,	86.0f	},	// 強右攻撃

	{ PLAYER_ANIM_PATH"LeftAttackS.x",	   false,	50.0f	},	// 弱左攻撃
	{ PLAYER_ANIM_PATH"LeftAttackM.x",	   false,	60.0f	},	// 中左攻撃
	{ PLAYER_ANIM_PATH"LeftAttackL.x",	   false,	99.0f	},	// 強左攻撃

	{ PLAYER_ANIM_PATH"Defense.x",	        true,	43.0f	},  // 防御
	{ PLAYER_ANIM_PATH"Evasion.x",	       false,	50.0f	},  // 回避

	{ PLAYER_ANIM_PATH"jump_start.x",	   false,	25.0f	},	// ジャンプ開始
	{ PLAYER_ANIM_PATH"jump.x",		        true,	1.0f	},	// ジャンプ中
	{ PLAYER_ANIM_PATH"jump_end.x",	       false,	26.0f	},	// ジャンプ終了
};


// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mAttackWay(EAttackWay::eIdle)
	, mAttackPower(EAttackPower::eAttackM)
	, mCurrAttackPower(EAttackPower::eAttackM)
	, mMode(EMode::eBattle)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mMotionBlurRemainTime(0.0f)
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

	//// 縦方向のコライダー作成
	//mpColliderLine = new CColliderLine
	//(
	//	this, ELayer::ePlayer,
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	//);
	//mpColliderLine->SetCollisionLayers({ ELayer::eField });

	//float width = PLAYER_WIDTH * 0.5f;
	//float posY = PLAYER_HEIGHT * 0.5f;
	//// 横方向（X軸）のコライダー作成
	//mpColliderLineX = new CColliderLine
	//(
	//	this, ELayer::ePlayer,
	//	CVector(-width, posY, 0.0f),
	//	CVector( width, posY, 0.0f)
	//);
	//mpColliderLineX->SetCollisionLayers({ ELayer::eField });

	//// 横方向（Z軸）のコライダー作成
	//mpColliderLineZ = new CColliderLine
	//(
	//	this, ELayer::ePlayer,
	//	CVector(0.0f, posY, -width),
	//	CVector(0.0f, posY,  width)
	//);
	//mpColliderLineZ->SetCollisionLayers({ ELayer::eField });

	// カプセルコライダー作成
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_CAP_DWON, 0.0f),
		CVector(0.0f, PLAYER_CAP_UP, 0.0f),
		PLAYER_WIDTH,true
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall});

	// 攻撃用のコライダ１（剣の刃の部分）
	mpAttackCollider1 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK1_CAP_DWON),
		CVector(0.0f, 0.0f, ATTACK1_CAP_UP),
		0.3f, true
	);
	mpAttackCollider1->SetCollisionTags({ ETag::eEnemy });
	mpAttackCollider1->SetCollisionLayers({ ELayer::eEnemy });
	mpAttackCollider1->Rotate(CVector(-4.5f, 14.1f, 0.0f));

	// 攻撃用のコライダ２（剣の持ち手の部分）
	mpAttackCollider2 = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, ATTACK2_CAP_UP),
		CVector(0.0f, 0.0f, ATTACK2_CAP_DWON),
		0.8, true
	);
	mpAttackCollider2->SetCollisionTags({ ETag::eEnemy });
	mpAttackCollider2->SetCollisionLayers({ ELayer::eEnemy });
	mpAttackCollider2->Rotate(CVector(-4.5f, 14.1f, 0.0f));

	// 攻撃用のコライダ３（盾の部分）
	mpAttackCollider3 = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		15.0, true
	);
	mpAttackCollider3->SetCollisionTags({ ETag::eEnemy });
	mpAttackCollider3->SetCollisionLayers({ ELayer::eEnemy });
	mpAttackCollider3->Translate(0.0f, 0.0f, -12.0f);


	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// 剣のボーンを取得
	CModelXFrame* Sword = mpModel->FinedFrame("Armature_mixamorig_Sword_joint");
	const CMatrix& swordMTX = Sword->CombinedMatrix();
	// 盾のボーンを取得
	CModelXFrame* Shield = mpModel->FinedFrame("Armature_mixamorig_Shield_joint");
	const CMatrix& shieldMTX = Shield->CombinedMatrix();
	// 攻撃用のコライダーを剣の行列に設定
	mpAttackCollider1->SetAttachMtx(&swordMTX);
	mpAttackCollider2->SetAttachMtx(&swordMTX);
	mpAttackCollider3->SetAttachMtx(&shieldMTX);


}

CPlayer::~CPlayer()
{
	// コライダーを破棄
	//SAFE_DELETE(mpColliderLine);
	//SAFE_DELETE(mpColliderLineX);
	//SAFE_DELETE(mpColliderLineZ);
	SAFE_DELETE(mpColliderCapsule);

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
	}

	spInstance = nullptr;
}

// 現在の状態を取得
CPlayer::EState CPlayer::GetState()
{
	return mState;
}

// 攻撃の方向を取得
CPlayer:: EAttackWay CPlayer::GetAttackWay()
{
	return mAttackWay;
}

// 攻撃の威力を取得
CPlayer::EAttackPower CPlayer::GetAttackPower()
{
	return mAttackPower;
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 攻撃方向や、攻撃の威力によって
// 切り替えるアニメーションの種類を取得
CPlayer::EAnimType CPlayer::GetAttackAnimType() const
{
	switch (mAttackWay)
	{
	case EAttackWay::eUpAttack:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eUpAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eUpAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eUpAttackL;
		break;
	case EAttackWay::eDwonAttack:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eDwonAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eDwonAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eDwonAttackL;
		break;
	case EAttackWay::eLeftAttack:
		if (mAttackPower == EAttackPower::eAttackS) return EAnimType::eLeftAttackS;
		if (mAttackPower == EAttackPower::eAttackM) return EAnimType::eLeftAttackM;
		if (mAttackPower == EAttackPower::eAttackL) return EAnimType::eLeftAttackL;
		break;
	case EAttackWay::eRightAttack:
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
	mMoveSpeed = CVector::zero;
	ChangeState(EState::eAttack);
	mAttackPower = mCurrAttackPower;

	// 上入力で、上攻撃
	if (CInput::Key('W'))
	{
		mAttackWay = EAttackWay::eUpAttack;
	}
	// 下入力で、下攻撃
	else if (CInput::Key('S'))
	{
		mAttackWay = EAttackWay::eDwonAttack;
	}
	// 左入力で、左攻撃
	else if (CInput::Key('A'))
	{
		mAttackWay = EAttackWay::eLeftAttack;
	}
	// 右入力で、右攻撃
	else if (CInput::Key('D'))
	{
		mAttackWay = EAttackWay::eRightAttack;
	}
	// 何も入力をしていなければ、上攻撃
	else
	{
		mAttackWay = EAttackWay::eUpAttack;
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
		// 左クリックで攻撃
		if (CInput::PushKey(VK_LBUTTON))
		{
			ChangeAttack();
		}
		// 右クリックで防御
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mState = EState::eDefense;
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
		// シフトで回避
		else if (CInput::Key(VK_SHIFT))
		{
			ChangeEvasion();
		}
	}
}

// 攻撃
void CPlayer::UpdateAttack()
{
	// プレイヤーの攻撃状態に合わせてアニメーションを切り替える
	EAnimType anim = GetAttackAnimType();
	ChangeAnimation(anim);

	// 攻撃終了待ち状態へ移行
	mState = EState::eAttackWait;

	// 斬撃SEの再生済みフラグを初期化
	mIsPlayedSlashSE = false;
	// 斬撃エフェクトの生成済みフラグを初期化
	mIsSpawnedSlashEffect = false;
}

// 攻撃終了待ち
void CPlayer::UpdateAttackWait()
{
	// 斬撃SEを再生していないかつ、アニメーションが25%以上進行したら、
	if (!mIsPlayedSlashSE && GetAnimationFrameRatio() >= 0.25f)
	{
		// 斬撃SEを再生
		mpSlashSE->Play();
		mIsPlayedSlashSE = true;
	}

	// 斬撃エフェクトを生成していないかつ、アニメーションが35%以上進行したら、
	if (!mIsSpawnedSlashEffect && GetAnimationFrameRatio() >= 0.35f)
	{
		// 斬撃エフェクトを生成して、正面方向へ飛ばす
		CSlash* slash = new CSlash
		(
			this,
			Position() + CVector(0.0f, 10.0f, 0.0f),
			VectorZ(),
			300.0f,
			100.0f
		);
		// 斬撃エフェクトの色設定
		slash->SetColor(CColor(0.15f, 0.5f, 0.5f));

		mIsSpawnedSlashEffect = true;
	}

	// 攻撃アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
		mAttackWay = EAttackWay::eIdle;
		ChangeAnimation(EAnimType::eAttackIdle);
	}
}

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
	//mMoveSpeed = CVector::zero;
	//float moveFrame = _MOVE_END - ATTACK_MOVE_START;
	//float percent = (frame - ATTACK_MOVE_START) / moveFrame;

}

//void CPlayer::UpEvasion()
//{
//	
//}
//
//void CPlayer::DwonEvasion()
//{
//	for (int i = 0; i < 10; i++)
//	{
//		mMoveSpeed += VectorZ() * EVA_MOVE_MINUS;
//	}
//}
//
//void CPlayer::LeftEvasion()
//{
//	for (int i = 0; i <= 20; i++)
//	{
//		mMoveSpeed += VectorX() * EVA_MOVE_MINUS;
//	}
//}
//
//void CPlayer::RightEvasion()
//{
//	for (int i = 0; i <= 10; i++)
//	{
//		mMoveSpeed += VectorX() * EVA_MOVE_PLUS;
//	}
//}

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
	mState = EState::eJump;

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		mState = EState::eIdle;
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


// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	if (CInput::PushKey('T'))
	{
		IsBattleMode = !IsBattleMode;
	}

	if (IsBattleMode)
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
		int power = ((int)mCurrAttackPower + 1) % powerNum;
		mCurrAttackPower = (EAttackPower)power;
	}
	// マウスホイールが下にスクロールされていたら、現在の攻撃威力ダウン
	else if (WheelDelta < 0)
	{
		int power = ((int)mCurrAttackPower + powerNum - 1) % powerNum;
		mCurrAttackPower = (EAttackPower)power;
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
	case EState::eAttackWait:
		UpdateAttackWait();
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

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

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
		/*Rotation(0.0f, 90.0f, 0.0f);*/
		// TODO：プレイヤーを敵に向ける
		// TODO：敵に視点をボタンを押してロックするか、自動で敵に向くようにする
	}


	//// 右クリックで弾丸発射
	//if (CInput::PushKey(VK_RBUTTON))
	//{
	//	 //弾丸を生成
	//	new CBullet
	//	(
	//		// 発射位置
	//		Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 20.0f,
	//		VectorZ(),	// 発射方向
	//		1000.0f,	// 移動距離
	//		1000.0f		// 飛距離
	//	);
	//}

	// 「E」キーで炎の発射をオンオフする
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

	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// 「B」キーを押したら、モーションブラー開始
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

	// モーションブラーの更新処理
	UpdateMotionBlur();

	// キャラクターの更新
	CXCharacter::Update();

	// コライダーの行列を変更
	mpAttackCollider1->Update();
	mpAttackCollider2->Update();
	mpAttackCollider3->Update();

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	//	CDebugPrint::Print("Mode:%d\n",(int)mMode);
	CDebugPrint::Print("Power:%d\n", (int)mCurrAttackPower);

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}


// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 縦方向の衝突処理
	/*
	if (self == mpColliderLine)
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
	}

	// 横方向（X軸とZ軸）の衝突処理
	else if (self == mpColliderLineX || self == mpColliderLineZ)
	{
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 押し戻しベクトルの分座標を移動
			Position(Position() + adjust * hit.weight);

		}
	}
	*/

	// 当たり判定
	if (self == mpColliderCapsule)
	{
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			//adjust.X(0.0f);
			//adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			float sideX = CVector::Dot(normal, CVector::right);
			float sideZ = CVector::Dot(normal, CVector::forward);
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
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}
