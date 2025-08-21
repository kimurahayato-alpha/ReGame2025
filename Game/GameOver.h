#include "stdafx.h"
#include "GameClear.h"
//#include "Title.h"
#pragma once
class GameOver : public IGameObject
{
public:
	GameOver();
	~GameOver();

	void Update();
	void Render(RenderContext& rc);

	SpriteRender m_spriteRender;
};

