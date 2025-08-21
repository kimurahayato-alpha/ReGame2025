#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

Player::Player()
{
	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimClip_Walk].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimClip_Jump].Load("Assets/animData/jump.tka");
	m_animationClips[enAnimClip_Jump].SetLoopFlag(false);
	m_animationClips[enAnimClip_Run].Load("Assets/animData/run.tka");
	m_animationClips[enAnimClip_Run].SetLoopFlag(true);
	//���f��������������B
	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimClip_Num, enModelUpAxisY);

	characterController.Init(25.0f, 75.0f, m_position);

}

Player::~Player()
{

}

void Player::Update()
{
	//�L�����N�^�[�̈ړ��B
	Move();

	//�L�����N�^�[�̉�]�B
	Rotation();

	//�X�e�[�g�Ǘ��B
	ManageState();

	//�L�����N�^�[�̃A�j���[�V�����B
	PlayAnimation();

	//���f�����X�V����
	m_modelRender.Update();

}

void Player::Move()
{
	//xz�̈ړ����x��0.0f�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forword = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//y�����ɂ͈ړ������Ȃ��B
	forword.y = 0.0f;
	right.y = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ�120.0f����Z�B
	right *= stickL.x * 180.0f;
	forword *= stickL.y * 180.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right + forword;

	if (g_pad[0]->IsPress(enButtonB))
	{
		m_moveSpeed.x *= 3;
		m_moveSpeed.z *= 3;
		m_playerState = 3;
	}

	//�n�ʂɕt���Ă�����B
	if (characterController.IsOnGround() == true)
	{
		//�d�͂𖳂����B
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
	//�ɂ��Ă�����B
	else
	{
		//�d�͂𔭐�������B
		m_moveSpeed.y -= 2.0f;
	}

	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);
	
}
	

void Player::Rotation()
{
	
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		rot.SetRotationYFromDirectionXZ(m_moveSpeed);
	}
	//��]���G�`������ɋ�����B
	m_modelRender.SetRotation(rot);
}

//�X�e�[�g�Ǘ��B
	void Player::ManageState() {

		//�n�ʂɕt���Ă��Ȃ�������B
		if (characterController.IsOnGround() == false)
		{
			//�X�e�[�g��1(�W�����v��)�ɂ���B
			m_playerState = 1;
			//������ManageState�̏������I��点��B
			return;
		}

		//�n�ʂɕt���Ă�����B
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			if (m_playerState  != 3)
			{
				//�X�e�[�g��2(����)�ɂ���B
				m_playerState = 2;
			}
		}
		//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
		else
		{
			//�X�e�[�g��0(�ҋ@)�ɂ���B
			m_playerState = 0;
		}
	}

	void Player::PlayAnimation()
{
	switch (m_playerState) {
		//�v���C���[�X�e�[�g��0(�ҋ@)��������B
	case 0:
		//�ҋ@�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimClip_Idle);
		break;
		//�v���C���[�X�e�[�g��1(�W�����v��)��������B
	case 1:
		//�W�����v�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimClip_Jump);
		break;
		//�v���C���[�X�e�[�g��2(����)��������B
	case 2:
		//�����A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimClip_Walk);
		break;
	case 3:
		//�����A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimClip_Run);
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	//���f����\������
	m_modelRender.Draw(rc);
}