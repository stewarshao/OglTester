#include <QtGui>
#include <QtOpenGL>
#include "glwidget.h"
#include "glwidgetManager.h"
#include "uiframe.h"
#include "glScene.h"
#include <QFile>
#include <QDir>
#ifndef	WIN32
	#include <unistd.h>
#endif

GLWidget::GLWidget(CScene* pScene,QGLFormat& glFormat,QWidget *parent)
: QGLWidget(glFormat,parent)
{
	m_pScene = pScene;
	m_GlFormat = glFormat;

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));

	m_pFullScreen = NULL;
	m_pParent = NULL;
	m_bFullScreen = false;

	m_pScene->SetNotifyFn(MsgProcess,this);

	m_nInterval = -1;

	setAutoFillBackground(false);			// 使用QPaintEvent，需要此设定

	m_nFps = 0;
	m_timerFPS = new QElapsedTimer;
	m_bTesting = false;
	m_bLastTest = false;

	m_nSubSceneCount = -1;
	m_pSubScene = NULL;
	m_SubSceneActions = NULL;
	m_SubSceneAs = NULL;
	m_nSelectedSubScene = 0;

	for (int i = 0; i < MAX_SUB_MENU; ++i)
		m_nSubMenu[i] = -1;

	m_nSubMenuCount = 0;
}

GLWidget::~GLWidget()
{
	//makeCurrent();
	if (NULL != m_pSubScene)
	{
		delete[] m_pSubScene;
		m_pSubScene = NULL;
	}
}

void GLWidget::initializeGL()
{
	// 初始化gl context
	m_pScene->Init();
}


void GLWidget::paintGL()
{
	m_pScene->Paint();
}

/*
void GLWidget::paintEvent(QPaintEvent *event)
{
	m_pScene->Paint();


	QPainter painter(this);

	// 绘制graphic layout
	DrawFps(&painter);	
	
	painter.end();

	// 使用paintEvent绘制OGL时，必须在函数内要使用QPainter,使用此种方式可以绘制graphic层，而paintGL不行
}
*/
void GLWidget::resizeGL(int width, int height)
{
	m_pScene->resize(width,height);
}

void GLWidget::setXRotation(int angle)
{
	m_pScene->setXRotation(angle);
}

void GLWidget::setYRotation(int angle)
{
	m_pScene->setYRotation(angle);
}

void GLWidget::setZRotation(int angle)
{
	m_pScene->setZRotation(angle);
}

int GLWidget::xRotation()
{ 
	return m_pScene->xRotation();
}

int GLWidget::yRotation()
{ 
	return m_pScene->yRotation();
}

int GLWidget::zRotation()
{
	return m_pScene->zRotation();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (m_bTesting)
		return;
	m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bTesting)
		return;
	int dx = event->x() - m_lastPos.x();
	int dy = event->y() - m_lastPos.y();

	if (event->buttons() & Qt::LeftButton) 
	{
		setXRotation(xRotation() + 8 * dy);
		setYRotation(yRotation() + 8 * dx);
	} 
	else if (event->buttons() & Qt::RightButton) 
	{
		setXRotation(xRotation() + 8 * dy);
		setZRotation(zRotation() + 8 * dx);
	}
	m_lastPos = event->pos();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (m_bTesting)
		return;

	if (event->buttons() & Qt::LeftButton) 
	{
		m_bFullScreen = !m_bFullScreen;
		if (m_bFullScreen)
		{
			// 只有QGLWidget对象不是另外一个对象的子控件时，才能够调用showFullScreen全屏显示
			// 因此每个GLWidget类需含有此fullscreen对象，用于显示
			// 只有dockwidget中的glwidget才能运行于此
			m_pFullScreen = new GLWidget(m_pScene,m_GlFormat);
			m_pFullScreen->showFullScreen();
			m_pFullScreen->m_bFullScreen = true;
			m_pFullScreen->m_pParent = this;
			g_MainWnd->hide();
			m_timer->stop();
			m_pFullScreen->m_timer->stop();
			m_pFullScreen->m_nInterval = m_nInterval;
			m_pFullScreen->m_timer->start(m_nInterval);
			m_pFullScreen->m_nFps = m_nFps;
		}
		else
		{
			// 只有fullscreen子对象才能运行于此
			// 此处需从fullscreen子对象中传出渲染参数
			showNormal();
			hide();
			g_MainWnd->showMaximized();
			m_pParent->m_bFullScreen = false;
			m_pParent->m_nInterval = m_nInterval;
			m_pParent->m_nFps = m_nFps;
			m_pParent->deleteFullScreenObj();	
		}
	}
}

void GLWidget::stop()
{
	m_timer->stop();
}

void GLWidget::deleteFullScreenObj()
{
	if (NULL != m_pFullScreen)
	{
		m_pScene->SetNotifyFn(MsgProcess,this);
		delete m_pFullScreen;
		m_pFullScreen = NULL;
		m_timer->stop();
		if (m_nInterval >= 0)
			m_timer->start(m_nInterval);
	}
}

void GLWidget::timerUpdate()
{
	m_pScene->timerFunc();

	update();
}

void GLWidget::MsgProcess(int nMsgID,void *pParam1,void *pParam2)
{
	GLWidget* pThis = (GLWidget*)pParam1;

	pThis->MsgDoProcess(nMsgID,pParam2);

	return ;
}

void GLWidget::MsgDoProcess(int nMsgID,void *pParam)
{
	switch (nMsgID)
	{
	case OT_MSG_REPAINT:
		//emit yRotationChanged(angle);
		update();
		break;
	case OT_MSG_TIMER_START:
		{
			m_timer->stop();
			m_nInterval = *(int*)pParam;
			m_timer->start(m_nInterval);
			break;
		}
	case OT_MSG_TIMER_STOP:
		{
			m_timer->stop();
			m_nInterval = -1;
			break;
		}
	case OT_MSG_COMPLETE_FRAME:
		{
			string dirName,testName;
			int nPicIndex;
			if (m_pScene->CanGrab(dirName,&nPicIndex,testName))
			{
				QImage imageFb = grabFrameBuffer();

				QString destDirName = dirName.c_str();
				while (destDirName.endsWith(QChar('/')))
				{
					destDirName = destDirName.left(destDirName.length()-1);	
				}

				// 创建目录
				QDir destDir;
				destDir.mkpath(destDirName.append(QString("/%1-%2").arg(m_pScene->Title()).arg(m_pScene->Version())));

				// 创建文件
				QString destFile = destDirName.append(QString("/%1_%2.bmp").arg(testName.c_str()).arg(nPicIndex));
				QFile file(destFile);
				if (file.exists())
					file.remove();
				else
				{
					file.open(QIODevice::WriteOnly);
					file.close();
				}

				imageFb.save(destFile,"BMP");
			}
			if (m_bTesting)
			{
				// 计算帧率
				m_llFrameCount++;
				m_nFps = 1000 * ((1.0 * m_llFrameCount) / m_timerFPS->elapsed());
			}
			break;
			
		}
	case OT_MSG_TEST_BEGIN:
		{
			//  开启帧率计算定时器
			m_timerFPS->restart();
			m_llFrameCount = 0;
			m_nFps = 0;
			m_bTesting = true;
			break;
		}
	case OT_MSG_TEST_FINISH:
		{
			m_timer->stop();
			m_nInterval = -1;
			int nInterval = m_pScene->needWait();
			if (nInterval)
			{
				// 需要在test间等待
#ifndef	WIN32
				sleep(1000*nInterval);
#else
				Sleep(1000*nInterval);
#endif
			}
			m_bTesting = false;
			qDebug() << "Total Frames:" << m_llFrameCount << "Elapsed time(ms):" << m_timerFPS->elapsed(); 
			if (m_bLastTest)
			{
				m_bLastTest = false;
				if (m_pParent)
					QTimer::singleShot(200, this, SLOT(FullScreen()));
			}
			break;
		}
	}
}

void GLWidget::FullScreen()
{
	QMouseEvent event(QEvent::MouseButtonDblClick, QPoint(0,0),Qt::NoButton,Qt::LeftButton,0);
	QApplication::sendEvent(this,&event);
}

void GLWidget::DrawFps(QPainter* pPainter)
{
	pPainter->setRenderHint(QPainter::Antialiasing);
	
	QString text = tr("FPS: %1").arg(m_nFps);

	QFont serifFont("Times", 30, QFont::Bold);
	pPainter->setFont(serifFont);
	
	QFontMetrics metrics = QFontMetrics(serifFont);
	int border = qMax(4, metrics.leading());

	QRect rect = metrics.boundingRect(0, 0, width()-2*border,int(height()*0.125),Qt::AlignCenter | Qt::TextWordWrap, text);
	pPainter-> setRenderHint(QPainter::TextAntialiasing);
	
	pPainter->fillRect(30, border,rect.width(), rect.height(),QColor(199, 237, 204, 15));
	pPainter->setPen(Qt::yellow);
	
	pPainter->drawText(30, border,
		rect.width(), rect.height(),
		Qt::AlignCenter | Qt::TextWordWrap, text);
}

void GLWidget::contextMenuEvent(QContextMenuEvent *event)
{
	if (-1 == m_nSubSceneCount)
	{
		m_nSubSceneCount = m_pScene->GetSubScene(NULL);
		if (m_nSubSceneCount > 0)
		{
			m_pSubScene = new char[m_nSubSceneCount][256];
			m_pScene->GetSubScene(m_pSubScene);

			m_SubSceneActions = new QActionGroup(this);
			m_SubSceneAs = new unsigned int[m_nSubSceneCount];

			int subMenu = 0;
			int subItem = 0;
			for (int i = 0; i < m_nSubSceneCount; ++i)
			{
				QString tmp = QString(m_pSubScene[i]);
				int nStar = tmp.indexOf("*");
				QString tmp1 = tmp.left(nStar);
				QString tmp2 = tmp.right(tmp.length()-nStar-1);
				strcpy(m_pSubScene[i],tmp1.toStdString().c_str());
				m_SubSceneAs[i] = (unsigned int)m_SubSceneActions->addAction(tmp1);
				((QAction*)m_SubSceneAs[i])->setCheckable(true);

				if (tmp2.contains("."))
				{
					// 子菜单的处理
					int nDot = tmp2.indexOf(".");
					int nMenuIndex = tmp2.left(nDot).toInt();
					if (subMenu != nMenuIndex)
					{
						if (subItem > 0)
						{
							m_nSubMenu[subMenu-1] = subItem;
							subItem = 0;
						}
						m_nSubMenuCount++;
						subMenu = nMenuIndex;
					}
					subItem++;
				}			
			}
			if (subItem > 0)
			{
				m_nSubMenu[subMenu-1] = subItem;
			}
			connect(m_SubSceneActions, SIGNAL(triggered(QAction *)), this, SLOT(SelectSubScene(QAction *)));
		}
	}

	if (0 == m_nSubSceneCount)
		return;

	QMenu menu(this);

	if (m_nSubMenuCount > 0)
	{
		// 存在子菜单
		int jIndex = 0;
		for (int i = 0; i < m_nSubMenuCount; ++i)
		{
			QMenu* tmpM = menu.addMenu(QString("Chapter %1").arg(i+1,2,10,QChar('0')));
			for (int j = 0; j < m_nSubMenu[i]; ++j)
				tmpM->addAction((QAction*)m_SubSceneAs[jIndex++]);
		}
	}
	else
	{
		for (int j = 0; j < m_nSubSceneCount; ++j)
		{
			menu.addAction((QAction*)m_SubSceneAs[j]);
		}
	}
	((QAction*)m_SubSceneAs[m_nSelectedSubScene])->setChecked(true);
	menu.exec(event->globalPos());
}

void GLWidget::SelectSubScene(QAction *action)
{
	for (int i = 0; i < m_nSubSceneCount; ++i)
	{
		if (action->text() == m_pSubScene[i])
		{
			m_nSelectedSubScene = i;
			m_pScene->SelectSubScene(i);
		}
	}
}
