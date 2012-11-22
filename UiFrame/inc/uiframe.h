/**************************************************************************
* @file uiframe.h
* @brief OGL Tester Ui Frame
* @author Stewart Shao
* @date 7/3/2012 2:36:57 PM
* @version 1.0
**************************************************************************/
#ifndef UIFRAME_H
#define UIFRAME_H

#include "globaldef.h"
#include <QtGui/QMainWindow>

class QAction;
class QToolBox;
class CTransformControls;
class CLightControls;
class CGrabControls;
class CNewSceneWizard;
class CScene;

class UiFrame : public QMainWindow
{
	Q_OBJECT

public:
	UiFrame();
	~UiFrame();

	void grab(QString saveLocation);	 /**< 抓图 */
	GlwHandle CreateNewGLWin(CScene* pScene);				/**< 创建新的ogl窗口 */
	void enableNewWizard();

private slots:
	void createNewTestProcedure();	 /**< 创建新的测试例程 */
	void about();					 /**< 关于 */
	void dockWidgetToogled(bool bToggled); /**< 关闭dockwin,打开dockwin和关闭dockwin时都会调用此函数，打开时bToggled为true，关闭时为false*/

private:
	void createMunus();				 /**< 创建菜单 */
	void createActions();			 /**< 创建动作 */
	void createToolBox();			 /**< 创建左侧工具栏 */

	void showWallpaper(bool bShow);		/**< 显示欢迎壁纸 */

private:
	QMenu	*testProcedureMenu;		 /**< 测试控制菜单 */
	QMenu	*aboutMenu;				 /**< about菜单 */

	QAction	*newTestAction;			 /**< 动作-创建新的测试例程 */
	QAction	*exitAction;			 /**< 动作-退出 */
	QAction	*aboutAction;			 /**< 动作-关于 */

	QToolBox	*toolbox;			 /**< 左侧toolbox */
	QMainWindow *glMainWin;			 /**< 装入tab页的mainwin */
	CTransformControls *transformC;	 /**< tranform页控件集对象 */
	CLightControls *lightC;			 /**< light页控件集对象 */
	CGrabControls *grabC;			 /**< grab页控件集对象 */
	CNewSceneWizard *newWizard;		 /**< 场景创建向导 */
	
	int TabIndexToMapIndex[50];  /**< tab索引到窗口管理器map索引的对照表 */
	int	indexToMap;				 /**< 上述数组的索引 */
};

extern UiFrame* g_MainWnd;

#endif // UIFRAME_H
