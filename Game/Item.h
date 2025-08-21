#pragma once

class Player;
//class Game;

class Item : public IGameObject
{
public:
	Item();
	~Item();
	void Update();
	void Render(RenderContext& rc);
	void Rotation();

	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_firstPosition;
	Quaternion m_rotation;
private:
	Player* m_player;
	//Game* m_game;
};

