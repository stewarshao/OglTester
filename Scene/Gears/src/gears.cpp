#include "gears.h"
#include <algorithm>

CSceneGears::CSceneGears()
{
	m_gear1 = 0;
	m_gear2 = 0;
	m_gear3 = 0;

	m_xRot = 0;
	m_yRot = 0;
	m_zRot = 0;
	m_gear1Rot = 0;
}

CSceneGears::~CSceneGears()
{
	glDeleteLists(m_gear1, 1);
	glDeleteLists(m_gear2, 1);
	glDeleteLists(m_gear3, 1);

}

const char* CSceneGears::Title()
{
	return "gears";
}

const char* CSceneGears::Author()
{
	return "shao";
}

const char* CSceneGears::Version()
{
	return "1.0";
}

const char* CSceneGears::Describe()
{
	return "Gears Demo";
}

void CSceneGears::Init()
{
	static const GLfloat lightPos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
	static const GLfloat reflectance1[4] = { 0.8f, 0.1f, 0.0f, 1.0f };
	static const GLfloat reflectance2[4] = { 0.0f, 0.8f, 0.2f, 1.0f };
	static const GLfloat reflectance3[4] = { 0.2f, 0.2f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	m_gear1 = makeGear(reflectance1, 1.0, 4.0, 1.0, 0.7, 20);
	m_gear2 = makeGear(reflectance2, 0.5, 2.0, 2.0, 0.7, 10);
	m_gear3 = makeGear(reflectance3, 1.3, 2.0, 0.5, 0.7, 10);

	glEnable(GL_NORMALIZE);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void CSceneGears::Paint()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);	// 注意，由于QT中开启了某些UI特性后，init时开启的gl状态有可能被关闭，所以此处再次打开

	glPushMatrix();
	glRotated(m_xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(m_yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(m_zRot / 16.0, 0.0, 0.0, 1.0);

	drawGear(m_gear1, -3.0, -2.0, 0.0, m_gear1Rot / 16.0);
	drawGear(m_gear2, +3.1, -2.0, 0.0, -2.0 * (m_gear1Rot / 16.0) - 9.0);

	glRotated(+90.0, 1.0, 0.0, 0.0);
	drawGear(m_gear3, -3.1, -1.8, -2.2, +2.0 * (m_gear1Rot / 16.0) - 2.0);

	glPopMatrix();

	Notify(OT_MSG_COMPLETE_FRAME,NULL);
}

void CSceneGears::Resize(int width,int height)
{
	int side = (width < height) ? width : height;
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, +1.0, -1.0, 1.0, 5.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -40.0);
}

void CSceneGears::setXRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != m_xRot) 
	{
		m_xRot = angle;
		Notify(OT_MSG_REPAINT,NULL);
	}
}

void CSceneGears::setYRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != m_yRot)
	{
		m_yRot = angle;
		Notify(OT_MSG_REPAINT,NULL);	
	}
}

void CSceneGears::setZRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != m_zRot)
	{
		m_zRot = angle;
		Notify(OT_MSG_REPAINT,NULL);
	}
}

int CSceneGears::xRotation()
{
	return m_xRot;
}

int CSceneGears::yRotation()
{
	return m_yRot;
}

int CSceneGears::zRotation()
{
	return m_zRot;
}

///////////////////////////////////Test////////////////////////////////////
int CSceneGears::Translate_Test(char* pReason)
{
	int nInterval = 10;
	Notify(OT_MSG_TIMER_START,&nInterval);

	m_nFrame = 0;

	return 0;
}

int CSceneGears::Rotate_Test(char* pReason)
{
	return 0;
}


/////////////////////////////////////Private/////////////////////////////////
GLuint CSceneGears::makeGear(const GLfloat *reflectance, GLdouble innerRadius,
						  GLdouble outerRadius, GLdouble thickness,
						  GLdouble toothSize, GLint toothCount)
{
	const double Pi = 3.14159265358979323846;

	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectance);

	GLdouble r0 = innerRadius;
	GLdouble r1 = outerRadius - toothSize / 2.0;
	GLdouble r2 = outerRadius + toothSize / 2.0;
	GLdouble delta = (2.0 * Pi / toothCount) / 4.0;
	GLdouble z = thickness / 2.0;
	int i, j;

	glShadeModel(GL_FLAT);

	for (i = 0; i < 2; ++i) {
		GLdouble sign = (i == 0) ? +1.0 : -1.0;

		glNormal3d(0.0, 0.0, sign);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= toothCount; ++j) {
			GLdouble angle = 2.0 * Pi * j / toothCount;
			glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
			glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
			glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
			glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
				sign * z);
		}
		glEnd();

		glBegin(GL_QUADS);
		for (j = 0; j < toothCount; ++j) {
			GLdouble angle = 2.0 * Pi * j / toothCount;
			glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
			glVertex3d(r2 * cos(angle + delta), r2 * sin(angle + delta),
				sign * z);
			glVertex3d(r2 * cos(angle + 2 * delta), r2 * sin(angle + 2 * delta),
				sign * z);
			glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
				sign * z);
		}
		glEnd();
	}

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < toothCount; ++i) {
		for (j = 0; j < 2; ++j) {
			GLdouble angle = 2.0 * Pi * (i + (j / 2.0)) / toothCount;
			GLdouble s1 = r1;
			GLdouble s2 = r2;
			if (j == 1)
				std::swap(s1, s2);

			glNormal3d(cos(angle), sin(angle), 0.0);
			glVertex3d(s1 * cos(angle), s1 * sin(angle), +z);
			glVertex3d(s1 * cos(angle), s1 * sin(angle), -z);

			glNormal3d(s2 * sin(angle + delta) - s1 * sin(angle),
				s1 * cos(angle) - s2 * cos(angle + delta), 0.0);
			glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), +z);
			glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), -z);
		}
	}
	glVertex3d(r1, 0.0, +z);
	glVertex3d(r1, 0.0, -z);
	glEnd();

	glShadeModel(GL_SMOOTH);

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= toothCount; ++i) {
		GLdouble angle = i * 2.0 * Pi / toothCount;
		glNormal3d(-cos(angle), -sin(angle), 0.0);
		glVertex3d(r0 * cos(angle), r0 * sin(angle), +z);
		glVertex3d(r0 * cos(angle), r0 * sin(angle), -z);
	}
	glEnd();

	glEndList();

	return list;
}

void CSceneGears::drawGear(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
						GLdouble angle)
{
	glPushMatrix();
	glTranslated(dx, dy, dz);
	glRotated(angle, 0.0, 0.0, 1.0);
	glCallList(gear);
	glPopMatrix();
}

void CSceneGears::normalizeAngle(int *angle)
{
	while (*angle < 0)
		*angle += 360 * 16;
	while (*angle > 360 * 16)
		*angle -= 360 * 16;
}

void CSceneGears::timerFunc()
{
	m_gear1Rot += 2 * 16;
	Notify(OT_MSG_REPAINT,NULL);

	if (++m_nFrame == 1500)
		Notify(OT_MSG_TEST_FINISH,NULL);
}