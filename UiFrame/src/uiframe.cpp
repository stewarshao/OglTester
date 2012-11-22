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

	createActions();		/**< ������������ */
	createMunus();			/**< �����˵����� */
	createToolBox();		/**< �������ƹ����� */

	// ��ӹ��ߴ���
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(toolbox,0,0);

	// װ��gl tab���ڵ�mainwin
	glMainWin = new QMainWindow();

	layout->addWidget(glMainWin,0,1,1,2);  // ʹ��ռ��ʱ������ѡ��ignore
	layout->setColumnStretch(2, 10);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);
	setCentralWidget(widget);

	setWindowTitle(tr("OGL Tester"));	 /**< ����Ӧ�ó����� */

	// ��ʾ��ӭ��ֽ
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
* �������˵�.
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
* ��������.
**************************************************************************/
void UiFrame::createActions()
{
	// ע��ͼƬ�ļ��ص�ַ��qrc_*.cpp��qt_resource_struct�в���
	newTestAction = new QAction(QIcon(QString("%1%2").arg(image_prefix).arg("new.png")),tr("&New test procedure"), this);
	newTestAction->setShortcut(tr("Ctrl+N"));
	newTestAction->setStatusTip(tr("Create a new test procedure"));
	connect(newTestAction, SIGNAL(triggered()),this, SLOT(createNewTestProcedure()));

	exitAction = new QAction(QIcon(QString("%1%2").arg(image_prefix).arg("exit.png")),tr("&Exit"), this);
	exitAction->setShortcut(tr("Ctrl+E"));
	exitAction->setStatusTip(tr("Exit program"));
	connect(exitAction, SIGNAL(triggered()),this, SLOT(close()));

	aboutAction = new QAction(QIcon(QString("%1%2").arg(image_prefix).arg("about.png")),tr("A&bout"), this);		// ��סaltʱ&����ĸ�³����»��ߣ���ʾ��ݼ�
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
* ����һ���µĲ�������.
**************************************************************************/
void UiFrame::createNewTestProcedure()
{	
	// �����ͼ
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
* ����.
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
		return;	//��Ӧ��dockҳ�����

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
		// ���м�ɾ�������
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
		// glMain��ֽ,ʹ��css����
		glMainWin->setStyleSheet("QWidget{background-image:url(../../images/backgroud.jpg);\
								 background-color: white;\
								 background-repeat:no-repeat;\
								background-position:center}");
		//background-position:center;		����
		//background-repeat:no-repeat		���ظ�
	}
	else
		glMainWin->setStyleSheet("");
}

GlwHandle UiFrame::CreateNewGLWin(CScene* pScene)
{
	//static QDockWidget *pDocWinWelcom = NULL;

	// ��Ӱ���gl���ڵ�tabҳ
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
