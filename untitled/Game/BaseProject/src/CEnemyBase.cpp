#include "CEnemyBase.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CColliderCapsule.h"
#include "CGaugeUI3D.h"
#include "CEnemyManager.h"
#include "Maths.h"

#define GRAVITY 0.0625f;

// コンストラクタ
CEnemyBase::CEnemyBase()
	:CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mState(0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mBattleElapsedTime(0.0f)
	, mIdleTime(0.0f)
	, mBattleIdletime(0.0f)
	, mpAnimData(nullptr)
	, mpAttackData(nullptr)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsAttackParry(false)
	, mIsGuardParry(false)
	, mIsGrounded(false)
	, mIsHitWall(false)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
	, mpHpGauge(nullptr)
{
	CEnemyManager* enemy = CEnemyManager::Instance();
	//　HPゲージを作成
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);

	// エネミー管理クラスに自身を追加
	enemy->Add(this);
}

//デストラクタ
CEnemyBase::~CEnemyBase()
{
	// エネミー管理クラスから自身を取り除く
	CEnemyManager::Instance()->Remove(this);

	// コライダー削除
	SAFE_DELETE(mpBodyCol);

	// HPゲージが存在したら、一緒に削除する
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

//オブジェクトを削除を伝える関数
void CEnemyBase::DeleteObject(CObjectBase* obj)
{
	// 削除されたのがHPゲージであれば
	// HPゲージのポインタを空にする
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

// パリィ出来るかどうか
bool CEnemyBase::CheckAttackParry(EAttackDir dir, EAttackPower power) const
{
	// ベースクラスのパリー条件を満たしていない場合、パリィ出来ない
	if (!CXCharacter::CheckAttackParry(dir, power)) return false;

	// パリィ出来ない攻撃だった場合、パリィ出来ない
	const AttackData& data = (*mpAttackData)[mAttackType];
	if (!data.attackParry) return false;

	// 現在のアニメーションフレームが、パリィ出来る範囲外だった場合、パリィ出来ない
	float frame = GetAnimationFrame();
	if (!(data.attackParryStartFrame <= frame && frame <= data.attackParryEndFrame)) 
		return false;

	// すべての条件を満たした場合、パリィ出来る
	return true;
}

bool CEnemyBase::CheckGaurdParry() const
{
	if (!CXCharacter::CheckGuardParry()) return false;

	const AttackData& data = (*mpAttackData)[mAttackType];
	if (!data.guardParry)	return false;

	float frame = GetAnimationFrame();
	if (!(data.guardParryStartFrame <= frame && frame <= data.guardParryEndFrame))
		return false;

	return true;
}

bool CEnemyBase::IsState(int state)
{
	if ((int)mState == state) return true;
	return false;
}

// 敵の初期化
void CEnemyBase::InitEnemy(std::string path, const std::vector<AnimData>* pAnimData)
{
	// アニメーションデータテーブルを設定
	mpAnimData = pAnimData;

	// モデルデータを読み込み
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// テーブル内のアニメーションデータを読み込み
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// CXCharacterの初期化
	Init(model);
}


// 衝突処理
void CEnemyBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体が衝突した
	if (self == mpBodyCol)
	{
		// フィールドと衝突した
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床化天井かを内積で判定
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
			// 内積の結果がマイナスであれば、天井と衝突した
			else
			{
				// ジャンプなどで天井に下から衝突した時　（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// プレイヤーと衝突した場合
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 横方向のみ押し戻すため
			// 押し戻したベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
		else if (other->Layer() == ELayer::eWall)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;

			mIsHitWall = true;

			// 押し戻しベクトルの分座標を移動
			Position(Position() + adjust * hit.weight);
		}
		else if (other->Tag() == ETag::ePlayer && other->Layer() == ELayer::eAttackCol)
		{
			CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
			if (CheckAttackParry(chara->GetAttackDir(), chara->GetAttackPower()))
			{
				mIsAttackParry = true;
				Parry();
			}
		}
	}
	
}

void CEnemyBase::Parry()
{
}

// アニメーション切り替え
void CEnemyBase::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// 状態切り替え
void CEnemyBase::ChangeState(int state)
{
	// 同じ状態の場合は切り替えない
	if (state == mState) return;

	// 状態を変更して、状態関連の変数を初期化
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CEnemyBase::ChangeAttackType(int attacktype)
{
	if (attacktype == mAttackType) return;

	mAttackType = attacktype;

	// 攻撃の種類から攻撃のデータを取得
	const AttackData& data = (*mpAttackData)[mAttackType];
	mAttackDir = data.dir;
	mAttackPower = data.power;
}


// 更新
void CEnemyBase::Update()
{
	// 重力
	mMoveSpeedY -= GRAVITY;

	// 移動
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;
	mIsHitWall = false;

	// HPゲージを更新
	mpHpGauge->Position(Position() + mGaugeOffsetPos);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
}

// 描画
void CEnemyBase::Render()
{
	CXCharacter::Render();
}



