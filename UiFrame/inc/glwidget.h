#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class QTimer;
class QElapsedTimer;
class CScene;
class QGLFormat;
class QPainter;
class QActionGroup;
class QAction;

#define MAX_SUB_MENU 24

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(CScene* pScene,QGLFormat& glFormat,QWidget *parent = 0);
	~GLWidget();

	void stop();		// ֹͣ��Ⱦ

	int xRotation();
	int yRotation();
	int zRotation();
	
	static void MsgProcess(int nMsgID,void *pParam1,void *pParam2);
	void MsgDoProcess(int nMsgID,void *pParam);

	GLWidget *m_pFullScreen;		/**< ȫ������ */
	bool m_bLastTest;

public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void FullScreen();
	void SelectSubScene(QAction *action);

signals:

	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

protected:
	void initializeGL();
	//void paintEvent(QPaintEvent *event);
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent * event);
	void DrawFps(QPainter* pPainter);
	void contextMenuEvent(QContextMenuEvent *event);			// �Ҽ��˵�����

private slots:
	void timerUpdate();

private:
	/************************************************************************** 
	* @brief deleteFullScreenObj 
	* 
	* ��fullscreen�Ӷ����е��ã�ɾ��fullscreen�Ӷ���.
	**************************************************************************/
	void deleteFullScreenObj();

private:
	QTimer *m_timer;				/**< ��ʱ�� */
	int m_nInterval;				/**< timer interval */
	QPoint m_lastPos;

	GLWidget *m_pParent;			/**< ������ȫ������ */
	bool m_bFullScreen;				/**< �Ƿ�ȫ�� */

	CScene* m_pScene;				/**< �������� */
	QGLFormat m_GlFormat;

	unsigned long long m_llFrameCount;	
	unsigned long m_nFps;
	QElapsedTimer* m_timerFPS;	
	bool m_bTesting;				/**< �Ƿ����ڽ��в��� */

	int m_nSubSceneCount;
	char (*m_pSubScene)[256];
	QActionGroup* m_SubSceneActions;
	unsigned int *m_SubSceneAs;
	int m_nSelectedSubScene;
	int m_nSubMenu[MAX_SUB_MENU];				/**< ֧��24���Ӳ˵� */
	int m_nSubMenuCount;
};

#endif