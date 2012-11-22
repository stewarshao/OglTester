#include <QtGui>
#include "uiframe.h"
#include "transformControls.h"
#include "lightControls.h"
#include "grabControls.h"
#include "glwidgetManager.h"
#include "glSceneManager.h"
#include "sceneWizard.h"

#define image_prefix ":/UiFrame/Output/images/"

UiFrame::UiFrame()
{
	if (NULL == CGlWidgetManager::Instance())
	{
		QMessageBox::critical(this, tr("Initial error"),
									tr("<b>GLWidget Manager</b> encounter an error during initialization"));
		exit(1);
	}

	if (NULL == CSceneManager::Instance())
	{
		QMessageBox::critical(this, tr("Initial error"),
			tr("<b>Scene Manager</b> encounter an error during initialization"));
		exit(1);
	}

	newWizard = NULL;
	
	for (int i = 0; i < 50; ++i)
		TabIndexToMapIndex[i] = -1;

	indexToMap = 0;

	createActions();		/**< 创建动作对象 */
	createMunus();			/**< 创建菜单对象 */
	createToolBox();		/**< 创建控制工具栏 */

	// 添加工具窗口
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(toolbox,0,0);

	// 装载gl tab窗口的mainwin
	glMainWin = new QMainWindow();

	layout->addWidget(glMainWin,0,1,1,2);  // 使用占列时，必须选择ignore
	layout->setColumnStretch(2, 10);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);
	setCentralWidget(widget);

	setWindowTitle(tr("OGL Tester"));	 /**< 设置应用程序名 */

	// 显示欢迎壁纸
	showWallpaper(true);

	toolbox->setVisible(false);
}

UiFrame::~UiFrame()
{
	if (NULL != newWizard)
		delete newWizard;
}

/************************************************************************** 
* @brief UiFrame::createMunu 
* 
* 创建主菜单.
**************************************************************************/
void UiFrame::createMunus()
{
	testProcedureMenu = menuBar()->addMenu(tr("&TestProcedure"));
	testProcedureMenu->addAction(newTestAction);
	testProcedureMenu->addAction(exitAction);

	aboutMenu = menuBar()->addMenu(tr("&Help"));
	aboutMenu->addAction(aboutAction);
}

/************************************************************************** 
* @brief UiFrame::createActions 
* 
* 创建动作.
**************************************************************************/
void UiFrame::createActions()
{
	// 注意图片的加载地址在qrc_*.cpp中qt_resource_struct中查找
	newTestAction = new QAction(QIcon(QString("%1%2").arg(image_prefix).arg("new.png")),tr("&New test procedure"), this);
	newTestAction->setShortcut(tr("Ctrl+N"));
	newTestAction->setStatusTip(tr("Create a new test procedure"));
	connect(newTestAction, SIGNAL(triggered()),this, SLOT(createNewTestProcedure()));

	exitAction = new QAction(QIcon(QString("%1%2").arg(image_prefix).arg("exit.png")),tr("&Exit"), this);
	exitAction->setShortcut(tr("Ctrl+E"));
	exitAction->setStatusTip(tr("Exit program"));
	connect(exitAction, SIGNAL(triggered()),this, SLOT(close()));

	aboutAction = new QAction(QIcon(QString("%1%2").arg(image_prefix).arg("about.png")),tr("A&bout"), this);		// 按住alt时&后字母下出现下划线，标示快捷键
	aboutAction->setShortcut(tr("Ctrl+B"));
	connect(aboutAction, SIGNAL(triggered()),this, SLOT(about()));
}

void UiFrame::createToolBox()
{
	transformC = new CTransformControls();
	QWidget* trasnformWidget = transformC->createControls();
 
	lightC = new CLightControls();
	QWidget* lightWidget = lightC->createControls();

	grabC = new CGrabControls(this);
	QWidget* grabWidget = grabC->createControls();

	toolbox = new QToolBox;
	toolbox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	toolbox->setMinimumWidth(grabWidget->sizeHint().width());
	toolbox->addItem(trasnformWidget,QIcon(QString("%1%2").arg(image_prefix).arg("transform.png")) ,tr("Transform controls"));
	toolbox->addItem(lightWidget,QIcon(QString("%1%2").arg(image_prefix).arg("light.png")),tr("Light controls"));
	toolbox->addItem(grabWidget,QIcon(QString("%1%2").arg(image_prefix).arg("grab.png")),tr("Grab controls"));
}

/************************************************************************** 
* @brief UiFrame::createNewTestProcedure 
* 
* 创建一个新的测试例程.
**************************************************************************/
void UiFrame::createNewTestProcedure()
{	
	// 清除贴图
	showWallpaper(false);

	newTestAction->setDisabled(true);

	if (NULL == newWizard)
	{
		newWizard = new CNewSceneWizard;
		if (NULL == newWizard)
		{
			QMessageBox::critical(this, tr("Initial error"),
				tr("<b>New Scene Wizard</b> encounter an error during initialization"));
			exit(1);
		}
	}

	newWizard->reload();

	newWizard->show();

	return;
}

void UiFrame::enableNewWizard()
{
	newTestAction->setDisabled(false);
	showWallpaper(true);
}

/************************************************************************** 
* @brief UiFrame::about 
* 
* 关于.
**************************************************************************/
void UiFrame::about()
{
	QMessageBox::about(this, tr("About OGL Tester"),
							 tr("The <b>OGL Tester</b> uses to  "
								"verify the correctness of Ogl driver."));
}

void UiFrame::grab(QString saveLocation)
{
	int tabIndex = -1;

	// get the QTabBar
	QList<QTabBar *> tabList = findChildren<QTabBar *>();
	if(!tabList.isEmpty())
	{
		QTabBar *tabBar = tabList.at(0);
		
		tabIndex = tabBar->currentIndex();
		//connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(testSlot(int)));
	}

	if (-1 == tabIndex && -1 == TabIndexToMapIndex[0])
		return;	//对应无dock页的情况

	if (-1 == tabIndex && -1 != TabIndexToMapIndex[0])
		tabIndex = 0;

	CGlWidgetManager::Instance()->Grab(TabIndexToMapIndex[tabIndex],saveLocation);
}

void UiFrame::dockWidgetToogled(bool bToggled)
{

#ifdef WIN32
	if (!isVisible() || bToggled || isMinimized())
#else
	if (!isVisible() || bToggled || !isMinimized())
#endif
		return;

	int tabIndex = -1;

	// get the QTabBar
	QList<QTabBar *> tabList = findChildren<QTabBar *>();
	if(!tabList.isEmpty())
	{
		QTabBar *tabBar = tabList.at(0);

		tabIndex = tabBar->currentIndex();
	}

	if (tabIndex < 0)
		tabIndex = 0;

	CGlWidgetManager::Instance()->DeleteGlWidget(TabIndexToMapIndex[tabIndex]);

	TabIndexToMapIndex[tabIndex] = -1;

	if (indexToMap != (tabIndex + 1))
	{
		// 从中间删除的情况
		for (int i = tabIndex; i < (indexToMap - 1); ++i)
			TabIndexToMapIndex[i] = TabIndexToMapIndex[i+1];
	}

	indexToMap--;
}

void UiFrame::showWallpaper(bool bShow)
{
	if (NULL == glMainWin)
		return;

	if (bShow)
	{
		// glMain壁纸,使用css设置
		glMainWin->setStyleSheet("QWidget{background-image:url(../../images/backgroud.jpg);\
								 background-color: white;\
								 background-repeat:no-repeat;\
								background-position:center}");
		//background-position:center;		居中
		//background-repeat:no-repeat		不重复
	}
	else
		glMainWin->setStyleSheet("");
}

GlwHandle UiFrame::CreateNewGLWin(CScene* pScene)
{
	//static QDockWidget *pDocWinWelcom = NULL;

	// 添加包含gl窗口的tab页
	QDockWidget* glWindowTab = NULL;	

	GlwHandle Hwin = CGlWidgetManager::Instance()->NewGlWidget(glWindowTab,pScene);
	if ((GlwHandle)-1 == Hwin)
		return Hwin;

	/*
	if (NULL == glWindowTab)
	{
		QMessageBox::critical(this, tr("Initial error"),
			tr("<b>Welcome GLWIDGET</b> encounter an error during initialization"));
		exit(1);
	}
	*/

	glMainWin->addDockWidget(Qt::BottomDockWidgetArea, glWindowTab);

	TabIndexToMapIndex[indexToMap++] = Hwin;
/*
	if (NULL != pDocWinWelcom)
		glMainWin->tabifyDockWidget(pDocWinWelcom, glWindowTab);
	else
		pDocWinWelcom = glWindowTab;
*/
	QAction* dockWidgetAction = glWindowTab->toggleViewAction();
	connect(dockWidgetAction, SIGNAL(toggled(bool)), this, SLOT(dockWidgetToogled(bool)));

	return Hwin;
}
