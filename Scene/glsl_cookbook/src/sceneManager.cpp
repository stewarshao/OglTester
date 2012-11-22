#include "sceneManager.h"
#include "glutils.h"
#include <iostream>
using std::cout;
using std::endl;
#include <cstdio>
#include <algorithm>

///////////////////////////////////chapter 01//////////////////////////////
#include "scenebasic.h"
#include "scenebasic_layout.h"
#include "scenebasic_uniform.h"
#include "scenebasic_uniformblock.h"

///////////////////////////////////chapter 02/////////////////////////////
#include "scenediffuse.h"
#include "scenediscard.h"
#include "scenetwoside.h"
#include "sceneflat.h"
#include "sceneads.h"

/////////////////////////////////chapter 03////////////////////////////
#include "scenetoon.h"
#include "scenedirectional.h"
#include "scenefog.h"
#include "scenemultilight.h"
#include "sceneperfragment.h"
#include "scenespot.h"

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
}

const char* CSceneManager::Title()
{
	return "glsl_cookbook";
}

const char* CSceneManager::Author()
{
	return "shao";
}

const char* CSceneManager::Version()
{
	return "1.0";
}

const char* CSceneManager::Describe()
{
	return "glsl Demo";
}

void CSceneManager::Init()
{
	m_scene = new SceneBasic();

	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		cout <<"Error initializing GLEW: " << glewGetErrorString(err) << endl;
	}
	GLUtils::checkForOpenGLError(__FILE__,__LINE__);

	GLUtils::dumpGLInfo();

	m_scene->initScene();
}

void CSceneManager::Paint()
{
	GLUtils::checkForOpenGLError(__FILE__,__LINE__);
	
	m_scene->render();

	Notify(OT_MSG_COMPLETE_FRAME,NULL);
}

void CSceneManager::Resize(int width,int height)
{
	m_width = width;
	m_height = height;
	m_scene->resize(width,height);
}

void CSceneManager::setXRotation(int angle)
{
	return;
}

void CSceneManager::setYRotation(int angle)
{
	return;
}

void CSceneManager::setZRotation(int angle)
{
	return;
}

int CSceneManager::xRotation()
{
	return 0;
}

int CSceneManager::yRotation()
{
	return 0;
}

int CSceneManager::zRotation()
{
	return 0;
}

///////////////////////////////////Test////////////////////////////////////
int CSceneManager::Translate_Test(char* pReason)
{
	int nInterval = 10;
	Notify(OT_MSG_TIMER_START,&nInterval);

	m_nFrame = 0;

	return 0;
}

int CSceneManager::Rotate_Test(char* pReason)
{
	return 0;
}

void CSceneManager::timerFunc()
{
	m_scene->update(0.0f);
	Notify(OT_MSG_REPAINT,NULL);
/*
	if (++m_nFrame == 1500)
		Notify(OT_MSG_TEST_FINISH,NULL);
	*/
}

int CSceneManager::SwitchToScene(int nScene)
{
	Notify(OT_MSG_TIMER_STOP,NULL);
	if (m_scene)
		delete m_scene;
	switch (nScene)
	{
	case 0:
		m_scene = new SceneBasic();
		break;
	case 1:
		m_scene = new SceneBasic_Layout();
		break;
	case 2:
		{
			m_scene = new SceneBasic_Uniform();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
		}
		break;
	case 3:
		m_scene = new SceneBasic_UniformBlock();
		break;
	case 4:
		m_scene = new SceneDiffuse();
		break;
	case 5:
		{
			m_scene = new SceneADS();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 6:
		{
			m_scene = new SceneTwoSide();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 7:
		{
			m_scene = new SceneFlat();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 8:
		{
			m_scene = new SceneDiscard();
			int nInterval = 30;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 9:
		{
			m_scene = new SceneToon();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 10:
		{
			m_scene = new SceneDirectional();
			int nInterval = 600;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 11:
		{
			m_scene = new SceneFog();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 12:
			m_scene = new SceneMultiLight();
			break;
	case 13:
		{
			m_scene = new ScenePerFragment();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	case 14:
		{
			m_scene = new SceneSpot();
			int nInterval = 10;
			Notify(OT_MSG_TIMER_START,&nInterval);
			break;
		}
	}
	m_scene->initScene();
	m_scene->resize(m_width,m_height);
	Notify(OT_MSG_REPAINT,NULL);
	return 0;
}