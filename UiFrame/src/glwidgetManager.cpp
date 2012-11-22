#include "glwidgetManager.h"
#include "glwidget.h"
#include <time.h>
#include <QFile>
#include <QDir>
#include <QtGui/QDockWidget>
#include "glScene.h"
#include <QGLFormat>

CGlWidgetManager* CGlWidgetManager::m_pMan = NULL;

CGlWidgetManager::CGlWidgetManager()
{
}

CGlWidgetManager::~CGlWidgetManager()
{
}

int CGlWidgetManager::init()
{
	m_nNextGlHandle = 0;

	m_bFullGlExist = false;

	return 0;
}

void CGlWidgetManager::deinit()
{
}

CGlWidgetManager* CGlWidgetManager::Instance()
{
	if (NULL == m_pMan)
	{
		m_pMan = new CGlWidgetManager();
		if (NULL != m_pMan)
		{
			int nRet = m_pMan->init();
			if (0 != nRet)
			{
				m_pMan->deinit();
				delete m_pMan;
				m_pMan = NULL;
			}
		}
	}
	return m_pMan;
}

void CGlWidgetManager::destroy()
{
	if (NULL != m_pMan)
	{
		m_pMan->deinit();
		delete m_pMan;
		m_pMan = NULL;
	}
}

GlwHandle CGlWidgetManager::NewGlWidget(QDockWidget*& pNewWidget,CScene* pScene)
{
	unsigned int uMajor,uMinor,uContextProfile;
	pScene->GetGlContextVersion(&uMajor,&uMinor,&uContextProfile);
	QGLFormat format;
	format.setVersion(uMajor,uMinor);
	format.setProfile((QGLFormat::OpenGLContextProfile)uContextProfile);
	format.setDepth(true);

	GLWidget* newWidget = new GLWidget(pScene,format);

	if (NULL == newWidget)
		return GlwHandle(-1);
	
	QDockWidget *dw = new QDockWidget;
	dw->setObjectName(QString("%1_%2_%3").arg(pScene->Title()).arg(pScene->Version()).arg(m_nNextGlHandle));
	dw->setWindowTitle(QString("%1_%2").arg(pScene->Title()).arg(pScene->Version()));
	dw->setFeatures(dw->features() & ~QDockWidget::DockWidgetMovable); 
	dw->setFeatures(dw->features() & ~QDockWidget::DockWidgetFloatable); 
	dw->setFeatures(dw->features() & ~QDockWidget::DockWidgetClosable);		//允许多测试实例时，应该允许删除tab页

	dw->setWidget(newWidget);

	m_mapGlWidget.insert(map<int,QDockWidget*>::value_type(m_nNextGlHandle,dw));

	pNewWidget = dw;
		
	return m_nNextGlHandle++;
}

int CGlWidgetManager::Grab(int index, QString FileRoot)
{
	bool bRet = false;

	if (m_mapGlWidget.empty())
		return 2;

	if (index < 0)
		index = 0;

	map<int,QDockWidget*>::iterator iter;

	iter = m_mapGlWidget.find(index);

	if (iter == m_mapGlWidget.end())
		return 3;

	QDockWidget* pDw = iter->second;

	if (NULL == pDw)
		return 4;

	GLWidget* pWidget = (GLWidget*)(pDw->widget());
	if (NULL != pWidget)
	{
		QImage imageFb = pWidget->grabFrameBuffer();

		time_t tNow = time(NULL);

		QString destDirName = FileRoot;
		while (destDirName.endsWith(QChar('/')))
		{
			destDirName = destDirName.left(destDirName.length()-1);	
		}

		// 创建目录
		QDir destDir;
		destDir.mkpath(destDirName.append(QString("/%1").arg(pDw->objectName())));

		// 创建文件
		QString destFile = destDirName.append(QString("/%2.bmp").arg(tNow));
		QFile file(destFile);
		if (file.exists())
			file.remove();
		else
		{
			file.open(QIODevice::WriteOnly);
			file.close();
		}

		bRet = imageFb.save(destFile,"BMP");
	}
	
	return bRet ? 0 : 1;
}

void CGlWidgetManager::DeleteGlWidget(GlwHandle index)
{
	map<int,QDockWidget*>::iterator iter;

	iter = m_mapGlWidget.find(index);

	if (iter == m_mapGlWidget.end())
		return;

	QDockWidget* pDw = iter->second;

	if (NULL == pDw)
		return;

	//delete pDw;		//这里不能删除此对象，让QT自动垃圾回收，后续的signal还需使用，但要停止render

	GLWidget* pWidget = (GLWidget*)(pDw->widget());

	if (NULL == pWidget)
		return;
	
	pWidget->stop();


	m_mapGlWidget[index] = NULL;
}

int CGlWidgetManager::FullScreen(int index,bool bF)
{
	bool bRet = false;

	if (m_mapGlWidget.empty())
		return 2;

	if (index < 0)
		index = 0;

	map<int,QDockWidget*>::iterator iter;

	iter = m_mapGlWidget.find(index);

	if (iter == m_mapGlWidget.end())
		return 3;

	QDockWidget* pDw = iter->second;

	if (NULL == pDw)
		return 4;

	GLWidget* pWidget = (GLWidget*)(pDw->widget());
	if (NULL != pWidget)
	{
		if (!bF)
			pWidget->m_pFullScreen->m_bLastTest = true;
		else if (pWidget)
			pWidget->FullScreen();
	}

	return bRet ? 0 : 1;
}
