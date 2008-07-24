/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "team.h"
#include "scene/scenemanager.h"
#include "utility/vishelper.h"
#include "player.h"
#include "../framework/avatar/calavatar.h"

using namespace ft;

Team::Team(void)
{
	CalVector vCenter = Field::WEST * 300;
	m_restPositions[0] = CalVector(vCenter + CalVector(0,0,300));
	m_restPositions[1] = CalVector(vCenter + CalVector(0,0,-300));

	for (int i=0; i<TEAM_SIZE; i++)
	{
		std::string id = "m_restCircles" + i;
		m_restCircles[i] = new Circle(id);
		SceneManager::getInstance()->AddObject(m_restCircles[i]);
		m_restCircles[i]->setRadius(50).setDisk(false).setSegmentsNumber(50).setColor(VisualizationHelper::COLOR_YELLOW, 0.8f);	
	}
}

Team::~Team(void)
{
}

void Team::PrepareToGame()
{
	
}

void Team::Update(const double elapsedTime, Field* field)
{
	UpdateRestPositions(field);
}

void Team::UpdateRestPositions(Field* field)
{
	for (int i=0; i<TEAM_SIZE; i++)
	{
		CalVector newPos = m_restPositions[i] + field->getBall()->getPosition();
		m_restCircles[i]->setPosition(newPos);
		Player* player = (Player*)m_vPlayers[i]->getImplementation();
		player->setRestPos(newPos);
		//((CalAvatar*)m_vPlayers[i]->getImplementation())->setPosition(newPos);
	}



}

/**
 */
bool Team::AddPlayer(Avatar* player)
{
    m_vPlayers.push_back(player);
	return true;
}

/**
 */
bool Team::RemovePlayer(Avatar* player)
{
	return false;
}

