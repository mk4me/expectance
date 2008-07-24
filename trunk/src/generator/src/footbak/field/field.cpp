/* 
 * Copyright (c) 2007 - 2008, FutureToday. All rights reserved.
 * author: abak
 */
#include "field.h"

#include "scene/scenemanager.h"
#include "scene/transformmanager.h"
#include "utility/vishelper.h"

using namespace ft;

CalVector Field::NORTH = CalVector();
CalVector Field::SOUTH = CalVector();
CalVector Field::WEST = CalVector();
CalVector Field::EAST = CalVector();


Field::Field()
{
	NORTH = TransformManager::SCENE_RIGHT;
	SOUTH = TransformManager::SCENE_LEFT;
	WEST = TransformManager::SCENE_FORWARD * -1;
	EAST = TransformManager::SCENE_FORWARD;

	m_traceBorderLines = new TraceLine("Field-m_borderLines");
    SceneManager::getInstance()->AddObject(m_traceBorderLines);
	m_traceBorderLines->setColor(VisualizationHelper::COLOR_WHITE);
	m_traceBorderLines->setBlending(false); m_traceBorderLines->HideMarker();

	m_halfLine = new TraceLine("Field-m_halfLine");
    SceneManager::getInstance()->AddObject(m_halfLine);
	m_halfLine->setColor(VisualizationHelper::COLOR_WHITE);
	m_halfLine->setBlending(false); m_halfLine->HideMarker();

	m_traceForward = new TraceLine("Field-m_traceForward");
    SceneManager::getInstance()->AddObject(m_traceForward);
	m_traceForward->setColor(VisualizationHelper::COLOR_RED);

	m_leftPenaltyLines = new TraceLine("Field-m_leftPenaltyLines");
    SceneManager::getInstance()->AddObject(m_leftPenaltyLines);
	m_leftPenaltyLines->setColor(VisualizationHelper::COLOR_WHITE);
	m_leftPenaltyLines->setBlending(false); m_leftPenaltyLines->HideMarker();

	m_rightPenaltyLines = new TraceLine("Field-m_rightPenaltyLines");
    SceneManager::getInstance()->AddObject(m_rightPenaltyLines);
	m_rightPenaltyLines->setColor(VisualizationHelper::COLOR_WHITE);
	m_rightPenaltyLines->setBlending(false); m_rightPenaltyLines->HideMarker();

	CalVector forward(TransformManager::SCENE_FORWARD);
	forward *= 200;
	m_traceForward->AddPoint(CalVector(0,100,0));
	m_traceForward->AddPoint(CalVector(0,100,0) + forward);

	m_fScaleWidthFactor = 8;
	m_fScaleLengthFactor = 12;

}

void Field::setCorners(const CalVector &corner_NW, const CalVector &corner_NE, 
								const CalVector &corner_SE, const CalVector &corner_SW)
{
	m_corner_NW.x = corner_NW.x*m_fScaleLengthFactor;  m_corner_NW.y = 0; m_corner_NW.z = corner_NW.z*m_fScaleWidthFactor;
	m_corner_NE.x = corner_NE.x*m_fScaleLengthFactor;  m_corner_NE.y = 0; m_corner_NE.z = corner_NE.z*m_fScaleWidthFactor;
	m_corner_SE.x = corner_SE.x*m_fScaleLengthFactor;  m_corner_SE.y = 0; m_corner_SE.z = corner_SE.z*m_fScaleWidthFactor;
	m_corner_SW.x = corner_SW.x*m_fScaleLengthFactor;  m_corner_SW.y = 0; m_corner_SW.z = corner_SW.z*m_fScaleWidthFactor;

	CalVector lengthLine = m_corner_NW  - m_corner_NE;
	m_fLength = lengthLine.length();

	CalVector widthLine = m_corner_NE - m_corner_SE;
	m_fWidth = widthLine.length();
}

void Field::InitFieldObjects()
{
	//Left gate pos
	CalVector gateLine(m_corner_NW);
	gateLine -= m_corner_SW;
	gateLine /= 2;
	m_leftGatePos = CalVector(m_corner_SW);
	m_leftGatePos += gateLine;

	//Right gate pos
	CalVector leftGateLine(m_corner_NE);
	leftGateLine -= m_corner_SE;
	leftGateLine /= 2;
	m_rightGatePos = CalVector(m_corner_SE);
	m_rightGatePos += leftGateLine;

}

void Field::InitLines()
{
	if (m_traceBorderLines != NULL)
	{
		m_traceBorderLines->ClearTrace();
		m_traceBorderLines->AddPoint(m_corner_NW);
		m_traceBorderLines->AddPoint(m_corner_NE);
		m_traceBorderLines->AddPoint(m_corner_SE);
		m_traceBorderLines->AddPoint(m_corner_SW);
		m_traceBorderLines->AddPoint(m_corner_NW);
	}
	if (m_halfLine != NULL)
	{
		CalVector north_line = Field::EAST * (m_fLength/2);
		CalVector top_point = m_corner_NW + north_line;// + north;

		CalVector bottom_point = (top_point + Field::SOUTH * m_fWidth);

		m_halfLine->AddPoint(top_point);
		m_halfLine->AddPoint(bottom_point);
	}

	if (m_leftPenaltyLines != NULL)
	{
		CalVector west_line = Field::NORTH * (m_fWidth/4);
		CalVector left_top_point = m_leftGatePos + west_line;
		CalVector right_top_point = left_top_point + Field::EAST * (m_fLength/5);
		CalVector right_bootom_point = right_top_point + Field::SOUTH * (m_fWidth/2);
		CalVector left_bootom_point = right_bootom_point + Field::WEST * (m_fLength/5);

		m_leftPenaltyLines->AddPoint(left_top_point);
		m_leftPenaltyLines->AddPoint(right_top_point);
		m_leftPenaltyLines->AddPoint(right_bootom_point);
		m_leftPenaltyLines->AddPoint(left_bootom_point);
	}

	if (m_rightPenaltyLines != NULL)
	{
		CalVector east_line = Field::NORTH * (m_fWidth/4);
		CalVector right_top_point = m_rightGatePos + east_line;
		CalVector left_top_point = right_top_point + Field::WEST * (m_fLength/5);
		CalVector left_bootom_point = left_top_point + Field::SOUTH * (m_fWidth/2);
		CalVector right_bootom_point = left_bootom_point + Field::EAST * (m_fLength/5);

		m_rightPenaltyLines->AddPoint(right_top_point);
		m_rightPenaltyLines->AddPoint(left_top_point);
		m_rightPenaltyLines->AddPoint(left_bootom_point);
		m_rightPenaltyLines->AddPoint(right_bootom_point);
	}



}
/// \brief virtual base for inherited objects for rendering purposes
bool Field::Render()
{
	return true;
}
