#ifndef __GL_SCENE_MANAGER__
#define __GL_SCENE_MANAGER__

#include "glDsoCommon.h"

#ifdef WIN32
	#pragma comment (lib , "opengl32.lib")
	#pragma comment (lib , "glu32.lib")
#endif

#define MAX_SCENE 6

class Scene;
class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

	const char*	Title();
	const char*	Author();
	const char*	Version();
	const char*	Describe();

	void Init();
	void Paint();
	void Resize(int width,int height);

	void timerFunc();

	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

	int xRotation();
	int yRotation();
	int zRotation();

	/************************************************************************** 
	* @brief Translate_Test 
	* 
	* 几何变换测试.
	* @param[in] pReason	失败原因
	* @return int 0--成功 其他--失败 
	**************************************************************************/
	int Translate_Test(char* pReason);

	int Rotate_Test(char* pReason);

	int SwitchToScene(int nScene);

private:

	int m_nFrame;

	Scene *m_scene;

	int m_width,m_height;
};

#endif
