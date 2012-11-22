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

	void grab(QString saveLocation);	 /**< ץͼ */
	GlwHandle CreateNewGLWin(CScene* pScene);				/**< �����µ�ogl���� */
	void enableNewWizard();

private slots:
	void createNewTestProcedure();	 /**< �����µĲ������� */
	void about();					 /**< ���� */
	void dockWidgetToogled(bool bToggled); /**< �ر�dockwin,��dockwin�͹ر�dockwinʱ������ô˺�������ʱbToggledΪtrue���ر�ʱΪfalse*/

private:
	void createMunus();				 /**< �����˵� */
	void createActions();			 /**< �������� */
	void createToolBox();			 /**< ������๤���� */

	void showWallpaper(bool bShow);		/**< ��ʾ��ӭ��ֽ */

private:
	QMenu	*testProcedureMenu;		 /**< ���Կ��Ʋ˵� */
	QMenu	*aboutMenu;				 /**< about�˵� */

	QAction	*newTestAction;			 /**< ����-�����µĲ������� */
	QAction	*exitAction;			 /**< ����-�˳� */
	QAction	*aboutAction;			 /**< ����-���� */

	QToolBox	*toolbox;			 /**< ���toolbox */
	QMainWindow *glMainWin;			 /**< װ��tabҳ��mainwin */
	CTransformControls *transformC;	 /**< tranformҳ�ؼ������� */
	CLightControls *lightC;			 /**< lightҳ�ؼ������� */
	CGrabControls *grabC;			 /**< grabҳ�ؼ������� */
	CNewSceneWizard *newWizard;		 /**< ���������� */
	
	int TabIndexToMapIndex[50];  /**< tab���������ڹ�����map�����Ķ��ձ� */
	int	indexToMap;				 /**< ������������� */
};

extern UiFrame* g_MainWnd;

#endif // UIFRAME_H
