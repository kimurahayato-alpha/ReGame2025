#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

Player::Player()
{
	//アニメーションクリップをロードする。
	m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClips[enAnimClip_Jump].SetLoopFlag(false);
	m_animationClips[enAnimClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimClip_Run].SetLoopFlag(true);
	//モデルを初期化する。
	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimClip_Num, enModelUpAxisY);

	characterController.Init(25.0f, 75.0f, m_position);

}

Player::~Player()
{

}

void Player::Update()
{
	//キャラクターの移動。
	Move();

	//キャラクターの回転。
	Rotation();

	//ステート管理。
	ManageState();

	//キャラクターのアニメーション。
	PlayAnimation();

	//モデルを更新する
	m_modelRender.Update();

}

void Player::Move()
{
	//xzの移動速度を0.0fにする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forword = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y方向には移動させない。
	forword.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 180.0f;
	forword *= stickL.y * 180.0f;

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right + forword;

	if (g_pad[0]->IsPress(enButtonB))
	{
		m_moveSpeed.x *= 3;
		m_moveSpeed.z *= 3;
		m_playerState = 3;
	}

	//地面に付いていたら。
	if (characterController.IsOnGround() == true)
	{
		//重力を無くす。
		m_moveSpeed.y = 0.0f;
		JumpCount = 0;

	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		if (JumpCount < MaxJumpCount)
			m_moveSpeed.y = 230.0f;
		JumpCount++;
		m_playerState = 1;
	}
	//についていたら。
	else
	{
		//重力を発生させる。
		m_moveSpeed.y -= 2.0f;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	m_position = characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
	
}
	

void Player::Rotation()
{
	
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		rot.SetRotationYFromDirectionXZ(m_moveSpeed);
	}
	//回転を絵描きさんに教える。
	m_modelRender.SetRotation(rot);
}

//ステート管理。
	void Player::ManageState() {

		//地面に付いていなかったら。
		if (characterController.IsOnGround() == false)
		{
			//ステートを1(ジャンプ中)にする。
			m_playerState = 1;
			//ここでManageStateの処理を終わらせる。
			return;
		}

		//地面に付いていたら。
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			if (m_playerState  != 3)
			{
				//ステートを2(歩き)にする。
				m_playerState = 2;
			}
		}
		//xとzの移動速度が無かったら(スティックの入力が無かったら)。
		else
		{
			//ステートを0(待機)にする。
			m_playerState = 0;
		}
	}

	void Player::PlayAnimation()
{
	switch (m_playerState) {
		//プレイヤーステートが0(待機)だったら。
	case 0:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Idle);
		break;
		//プレイヤーステートが1(ジャンプ中)だったら。
	case 1:
		//ジャンプアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Jump);
		break;
		//プレイヤーステートが2(歩き)だったら。
	case 2:
		//歩きアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Walk);
		break;
	case 3:
		//歩きアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimClip_Run);
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	//モデルを表示する
	m_modelRender.Draw(rc);
}