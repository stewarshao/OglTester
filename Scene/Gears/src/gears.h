#ifndef __GL_SCENE_GEARS__
#define __GL_SCENE_GEARS__

#include "glDsoCommon.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifdef WIN32
	#pragma comment (lib , "opengl32.lib")
	#pragma comment (lib , "glu32.lib")
#endif

#ifdef WIN32        // ��̬����ʱ����Ҫlib����̬���벻��Ҫ
	#ifdef __GL_STATIC_LOAD_DSO__ 
			#pragma comment ( lib , "Gears.lib")
	#endif
#endif

class CSceneGears
{
public:
	CSceneGears();
	~CSceneGears();

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
	* ���α任����.
	* @param[in] pReason	ʧ��ԭ��
	* @return int 0--�ɹ� ����--ʧ�� 
	**************************************************************************/
	int Translate_Test(char* pReason);

	int Rotate_Test(char* pReason);

private:
	GLuint makeGear(const GLfloat *reflectance, GLdouble innerRadius,
		GLdouble outerRadius, GLdouble thickness,
		GLdouble toothSize, GLint toothCount);

	void drawGear(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
		GLdouble angle);

	void normalizeAngle(int *angle);

private:
	GLuint m_gear1;
	GLuint m_gear2;
	GLuint m_gear3;

	int m_xRot;
	int m_yRot;
	int m_zRot;
	int m_gear1Rot;

	int m_nFrame;
};

#endif
