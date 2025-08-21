#pragma once
class GameCamera;
class Player:public IGameObject
{
public:
	Player();
	~Player();
	void Update();
	void Render(RenderContext& rc);
	void Move();
	void Rotation();
	void ManageState();
	void PlayAnimation();

	ModelRender m_modelRender;//���f�������_�[
	Vector3 m_position;//���W
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_Jump,
		enAnimClip_Run,
		enAnimClip_Num,
	};
	
	AnimationClip m_animationClips[enAnimClip_Num];
	CharacterController characterController;
	Vector3 m_moveSpeed;
	Quaternion rot;
	int JumpCount = 0;
	int MaxJumpCount = 2;
	int m_playerState;
	int m_itemCount = 0;
	GameCamera* m_gameCamera;
};