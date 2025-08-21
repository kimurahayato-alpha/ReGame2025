#include "stdafx.h"
#include "Item.h"
#include "Player.h"
//#include "Game.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Item::Item()
{
	m_modelRender.Init("Assets/modelData/item.tkm");

	m_player = FindGO<Player>("BattleCharacter");
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/Starse.wav");
}

Item::~Item()
{

}

void Item::Update()
{

	Rotation();

	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
	
	Vector3 diff = m_player->m_position - m_position;
	if (diff.Length() < 60.0f)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(2);
		se->SetVolume(3.5f);
		se->Play(false);
		m_player->m_itemCount += 1;
		DeleteGO(this);
	}

}

void Item::Rotation()
{
	m_rotation.AddRotationDegY(2.0f);

	m_modelRender.SetRotation(m_rotation);
}

void Item::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}