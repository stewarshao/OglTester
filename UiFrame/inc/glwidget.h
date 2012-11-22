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

	void stop();		// 停止渲染

	int xRotation();
	int yRotation();
	int zRotation();
	
	static void MsgProcess(int nMsgID,void *pParam1,void *pParam2);
	void MsgDoProcess(int nMsgID,void *pParam);

	GLWidget *m_pFullScreen;		/**< 全屏对象 */
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
	void contextMenuEvent(QContextMenuEvent *event);			// 右键菜单处理

private slots:
	void timerUpdate();

private:
	/************************************************************************** 
	* @brief deleteFullScreenObj 
	* 
	* 从fullscreen子对象中调用，删除fullscreen子对象.
	**************************************************************************/
	void deleteFullScreenObj();

private:
	QTimer *m_timer;				/**< 定时器 */
	int m_nInterval;				/**< timer interval */
	QPoint m_lastPos;

	GLWidget *m_pParent;			/**< 仅用于全屏对象 */
	bool m_bFullScreen;				/**< 是否全屏 */

	CScene* m_pScene;				/**< 场景对象 */
	QGLFormat m_GlFormat;

	unsigned long long m_llFrameCount;	
	unsigned long m_nFps;
	QElapsedTimer* m_timerFPS;	
	bool m_bTesting;				/**< 是否正在进行测试 */

	int m_nSubSceneCount;
	char (*m_pSubScene)[256];
	QActionGroup* m_SubSceneActions;
	unsigned int *m_SubSceneAs;
	int m_nSelectedSubScene;
	int m_nSubMenu[MAX_SUB_MENU];				/**< 支持24个子菜单 */
	int m_nSubMenuCount;
};

#endif