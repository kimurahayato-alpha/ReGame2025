#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "GameClear.h"
#include "GameOver.h"
#include "Item.h"

Game::Game()
{
	
}

Game::~Game()
{
	DeleteGO(FindGO<Item>("item1"));
	DeleteGO(FindGO<Item>("item2"));
	DeleteGO(FindGO<Item>("item3"));
	DeleteGO(FindGO<Item>("item4"));
	DeleteGO(FindGO<Item>("item5"));
	DeleteGO(m_player);
	DeleteGO(m_gameCamera);
	DeleteGO(m_skyCube);
	DeleteGO(m_backGround);
}

bool Game::Start()
{
	m_player = NewGO<Player>(0, "BattleCharacter");
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_backGround = NewGO<BackGround>(0, "background");
	m_skyCube = NewGO<SkyCube>(0, "skycube");

	Item* item1 = NewGO<Item>(0, "item1");
	item1->m_position = { 0.0f,25.0f,1100.0f };
	item1->m_firstPosition = item1->m_position;

	Item* item2 = NewGO<Item>(0, "item2");
	item2->m_position = { 0.0f,420.0f,2460.0f };
	item2->m_firstPosition = item2->m_position;

	Item* item3 = NewGO<Item>(0, "item3");
	item3->m_position = { 0.0f,740.0f,3950.0f };
	item3->m_firstPosition = item3->m_position;

	Item* item4 = NewGO<Item>(0, "item4");
	item4->m_position = { 0.0f,-870.0f,4100.0f };
	item4->m_firstPosition = item4->m_position;

	Item* item5 = NewGO<Item>(0, "item5");
	item5->m_position = { 0.0f,-380.0f,6500.0f };
	item5->m_firstPosition = item5->m_position;

	return true;
}

void Game::Update()
{
	if (m_player->m_itemCount == 5)
	{
	NewGO<GameClear>(0);
		DeleteGO(this);
	}

	if (m_player->m_position.y <= -1200.0f)
	{
		NewGO<GameOver>(0);
		DeleteGO(this);
	}

	m_modelRender.Update();
	
}

void Game::Render(RenderContext& rc)
{
	
}