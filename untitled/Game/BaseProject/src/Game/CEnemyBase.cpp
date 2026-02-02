#include "CEnemyBase.h"
#include "CPlayer.h"
#include "CField.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CColliderCapsule.h"
#include "CGaugeUI3D.h"
#include "CEnemyManager.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "Maths.h"

#define GRAVITY 0.0625f;
#define PATROL_INTERVAL 3.0f		// 次の巡回に移動開始するまでの時間
#define PATROL_NEAR_DIST 10.0f		// 巡回開始時に選択される巡回ポイントの最短距離
#define ROTATE_SPEED 6.0f			// 回転速度
#define DEFAULT_WALK_SPEED 20.0f	// 歩きの速度
#define DEFAULT_RUN_SPEED 60.0f		// 走っている時の速度

#define NAVMOVE_STOP_DIST 0.05f		// 経路探索での移動時に停止する距離
#define UPDATE_MOVE_NODE_DIST 2.0f	// 移動用の経路探索ノードの更新処理

// コンストラクタ
CEnemyBase::CEnemyBase()
	:CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mElapsedTime(0.0f)
	, mBattleElapsedTime(0.0f)
	, mIdleTime(0.0f)
	, mBattleIdletime(0.0f)
	, mpAnimData(nullptr)
	, mpAttackData(nullptr)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mWalkSpeed(DEFAULT_WALK_SPEED)
	, mRunSpeed(DEFAULT_RUN_SPEED)
	, mIsAttackParry(false)
	, mIsGuardParry(false)
	, mIsBattle(false)
	, mIsGuard(false)
	, mIsAvoid(false)
	, mIsGrounded(false)
	, mIsHitWall(false)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
	, mpHpGauge(nullptr)
	, mpMoveNavNode(nullptr)
	, mpBattleTarget(nullptr)
	, mNextMoveIndex(0)
	, mIsUpdateMoveRoute(false)
{
	CEnemyManager* enemy = CEnemyManager::Instance();
	//　HPゲージを作成
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);

	// エネミー管理クラスに自身を追加
	enemy->Add(this);

	// 移動用の経路探索ノードを作成
	mpMoveNavNode = new CNavNode(CVector::zero, true);
	// 移動用の経路探索ノードは最初は無効化しておく
	mpMoveNavNode->SetEnable(false);
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

	// 移動用の経路探索ノードを削除
	mpMoveNavNode->Kill();
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

// アタックパリィ出来るかどうか
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

// ガードパリィ出来るかどうか
bool CEnemyBase::CheckGuardParry() const
{
	const AttackData& data = (*mpAttackData)[mAttackType];
	if (!data.guardParry)	return false;

	float frame = GetAnimationFrame();
	if (!(data.guardParryStartFrame <= frame && frame <= data.guardParryEndFrame))
		return false;

	return true;
}

CStateBase* CEnemyBase::GetCurrentState() const
{
	return mStateMachine.GetCurrentState();
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

bool CEnemyBase::MoveTo(const CVector& targetPos, float speed)
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

// 指定した位置まで経路探索で移動する
bool CEnemyBase::NavMoveTo(const CVector& targetPos, float speed)
{
	// 自分自身のノードが存在しなければ
	if (mpNavNode == nullptr)
	{
		// 普通の移動処理を行う
		return MoveTo(targetPos, speed);
	}

	// 目的地までの残りの距離
	float remain = (targetPos - Position()).LengthSqr();
	if (remain < NAVMOVE_STOP_DIST * NAVMOVE_STOP_DIST) return true;

	// 次の目的地まで一定距離離れてたら、移動経路を更新
	float dist = (mpMoveNavNode->GetPos() - targetPos).LengthSqr();
	if (dist >= UPDATE_MOVE_NODE_DIST * UPDATE_MOVE_NODE_DIST)
	{
		// 移動先ノードをオンにして、目的地に設定
		mpMoveNavNode->SetEnable(true);
		mpMoveNavNode->SetPos(targetPos);
		// 移動経路再計算用の初期化
		/*mNextMoveIndex = -1;*/
		mIsUpdateMoveRoute = true;
	}

	// 移動経路再計算フラグが立っているか
	if (mIsUpdateMoveRoute)
	{
		// 自身のノードと目的地のノードが更新中でなければ
		if (!mpNavNode->IsUpdating() && !mpMoveNavNode->IsUpdating())
		{
			// 移動経路を計算
			bool success = CNavManager::Instance()->Navigate(mpNavNode, mpMoveNavNode, mMoveRoute);
			if (success)
			{
				// 移動開始
				mIsUpdateMoveRoute = false;
				mNextMoveIndex = 1;
			}
			else
			{
				mNextMoveIndex = -1;
			}
		}
	}

	// 次の移動先のインデックス値が設定されてたら（不正値でなければ）
	if (mNextMoveIndex > 0)
	{
		CNavNode* nextNode = mMoveRoute[mNextMoveIndex];
		if (MoveTo(nextNode->GetPos(), speed))
		{
			// 移動が終われば、次のノードへ切り替え
			mNextMoveIndex++;
			// 移動が終わったノードが目的地ノードだった場合は、移動終了
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// 移動用のノードをオフ
				mpMoveNavNode->SetEnable(false);
				return true;
			}
		}
	}


	return false;
}


// 移動速度を設定
void CEnemyBase::SetMoveSpeed(const CVector& moveSpeed)
{
	mMoveSpeed = moveSpeed;
}

// 移動速度を取得
const CVector& CEnemyBase::GetMoveSpeed() const
{
	return mMoveSpeed;
}

// 戦闘状態か
bool CEnemyBase::GetIsBattle() const
{
	return mIsBattle;
}

CObjectBase* CEnemyBase::GetBattleTarget() const
{
	return mpBattleTarget;
}

void CEnemyBase::SetIsBattle(bool isbattle)
{
	mIsBattle = isbattle;
}

void CEnemyBase::SetBattleTarget(CObjectBase* target)
{
	mpBattleTarget = target;
}

// 戦闘相手の方へ向く
void CEnemyBase::LookAtBattleTarget(bool immediate)
{
	// 戦闘相手がいなければ、処理しない
	if (GetBattleTarget() == nullptr) return;

	// 戦闘相手までの方向ベクトルを求める
	CVector targetPos = GetBattleTarget()->Position();
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
			mLookAtSpeed * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// プレイヤーが視野範囲内に入ったかどうか
bool CEnemyBase::IsFoundPlayer() const
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
	CVector forward = GetHeadForwardVec();
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
bool CEnemyBase::IsLookPlayer() const
{
	// プライヤーが存在しない場合は、見えていない
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;
	// フィールドが存在しない場合は、遮蔽物がないので見える
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, mEyeHeight, 0.0f);
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

// プレイヤーの攻撃を検知したか？
bool CEnemyBase::IsPlayerAttackDetected() const
{
	// プレイヤーが存在しない場合は、範囲外とする
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	if (!player->IsAttacking()) return false;
	// プレイヤーの攻撃範囲か？
	if (CanAttackPlayer(player->GetAttackLength())) return true;
}

// 攻撃範囲を追加（敵によって違うので）
void CEnemyBase::AddAttackRange(float closeRng, float nearRng, float middleRng, float longRng, float detectRng)
{
	mRangeData.emplace_back(closeRng, nearRng, middleRng, longRng, detectRng);
}

// 
const CEnemyBase::AttackRangeData& CEnemyBase::GetRangeData(int index) const
{
	// インデックスが範囲外なら、０番目を返す処理
	if (index < 0 || index >= mRangeData.size()) {
		return mRangeData[0];
	}
	return mRangeData[index];
}

// プレイヤーを攻撃出来るかどうか
bool CEnemyBase::CanAttackPlayer(float range) const
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
bool CEnemyBase::IsMoveAttackRange(const AttackRangeData& range)
{

	// プレイヤーがいない場合は、攻撃できない
	CPlayer* player = CPlayer::Instance();

	if (player == nullptr) return false;

	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);
	float dist = vec.Length();

	if (dist > range.detectRange) return false;

	// 全ての条件をみたした
	return true;
}

// プレイヤーの攻撃を検知時の処理
bool CEnemyBase::DetectedPlayerAttack()
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

			//if (rand < GUARD_PROB)
			//{
			//	ChangeState((int)EState::eGuard);
			//	return true;
			//}
			//else if (rand < AVOID_PROB)
			//{
			//	ChangeState((int)EState::eAvoid);
			//	mIsAvoid = true;
			//	return true;
			//}
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

// 視野に関する情報を設定
void CEnemyBase::SetFovs(float angle, float length, float eyeHeight, float lookAtSpeed)
{
	mFovAngle = angle;
	mFovLength = length;
	mEyeHeight = eyeHeight;
	mLookAtSpeed = lookAtSpeed;
}

CVector CEnemyBase::GetHeadForwardVec() const
{
	if (mpHeadMtx == nullptr) return VectorZ();

	CMatrix m;
	m.RotateX(-90.0f);
	m = m * (*mpHeadMtx);
	CVector vec = m.VectorZ();
	vec.Y(0.0f);
	return vec.Normalized();
}

// 状態切り替え処理
void CEnemyBase::ChangeState(int state)
{
	mStateMachine.ChangeState(state);
}

// 攻撃タイプ切り替え処理
void CEnemyBase::ChangeAttackType(int attacktype)
{
	if (attacktype == mAttackType) return;

	mAttackType = attacktype;
	//mStateStep = 0;
	mElapsedTime = 0.0f;

	// 攻撃の種類から攻撃のデータを取得
	const AttackData& data = (*mpAttackData)[mAttackType];
	mAttackDir = data.dir;
	mAttackPower = data.power;
}

// アニメーション切り替え処理
void CEnemyBase::ChangeAnimation(int type, int no, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart,
		no
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

void CEnemyBase::ChangeStateAnimation(int stateIndex, int no)
{
}


// 更新
void CEnemyBase::Update()
{
	// 自身のノードの更新処理
	if (mpNavNode != nullptr)
	{
		mpNavNode->Update();
	}
	// 移動先のノードの更新処理
	if (mpMoveNavNode != nullptr)
	{
		mpMoveNavNode->Update();
	}

	// ステートマシンを更新
	mStateMachine.Update();

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

void CEnemyBase::SelectAttack(float range, int rand)
{

	if (CanAttackPlayer())
}

float CEnemyBase::GetWalkSpeed() const
{
	return mWalkSpeed;
}

float CEnemyBase::GetRunSpeed() const
{
	return mRunSpeed;
}

int CEnemyBase::GetAttackType() const
{
	return mAttackType;
}

const std::vector<CEnemyBase::AttackData>* CEnemyBase::GetAttackData() const
{
	return mpAttackData;
}



CVector CEnemyBase::GetLostPlayerNodePos() const
{
	return mpLostPlayerNode->GetPos();
}

void CEnemyBase::SetLostPlayerNodePos(const CVector& pos) const
{
	return mpLostPlayerNode->SetPos(pos);
}






