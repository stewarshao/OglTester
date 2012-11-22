#ifndef __WIZARD_NEW_SCENE__
#define __WIZARD_NEW_SCENE__

#include <QObject>
#include <QtGui/QWizardPage>
#include <string>
#include "glSceneManager.h"

class QWizard;
class QTableWidget;
class CSceneSeclectPage;
class QLineEdit;
class CTestSelectPage;
class QTreeWidget;
class QTreeWidgetItem;
class CMiscellaneousPage;
class QGroupBox;
class QSpinBox;
class QDial;
class QCheckBox;

// 场景信息结构体
typedef struct
{
	string	Title;			/**< 场景名称 */
	string	Author;			/**< 场景作者 */
	string	Version;		/**< 场景版本 */
	string	Description;	/**< 场景描述 */
	string	ModuleName;		/**< 文件名 */
} SCENE_INFO;

class CNewSceneWizard : public QObject
{
	Q_OBJECT

public:
	CNewSceneWizard();
	~CNewSceneWizard();

	void show();								/**< 显示wizard */

public slots:
	void reload();								/**< 刷新场景 */
	void reloadTest(const string& sScene);		/**< 刷新测试 */

private slots:
	void pageChange(int id);					/**< 向导页改变的信号 */
	void next();								
	void finish();
	void cancel();

private:
	QWizard* m_wizard;					/**< 场景创建wizard */
	CSceneSeclectPage *m_sselect;		/**< 场景选择page */
	CTestSelectPage *m_tselect;			/**< 测试选择page */
	CMiscellaneousPage *m_mis;			/**< 杂项选择page */

};

///////////////////////////////////////////////////////////////////////////////////////////
//////////						CSceneSeclectPage
///////////////////////////////////////////////////////////////////////////////////////////
class CSceneSeclectPage : public QWizardPage
{
	Q_OBJECT

public:
	CSceneSeclectPage(QWidget *parent = 0);

	void RefreshTable(int nScene,SCENE_INFO *pInfo);

	void clearView();

	string getSelcetScene();

private slots:
	void itemClicked(int row,int cloumn);
	void headerClicked(int i);
	
private:
	QTableWidget* m_pTable;				/**< 场景列表 */
	QLineEdit *m_NameLineEdit;			/**< 主要用于disable next */
};

///////////////////////////////////////////////////////////////////////////////////////////
//////////						CTestSelectPage
///////////////////////////////////////////////////////////////////////////////////////////
class CTestSelectPage : public QWizardPage
{
	Q_OBJECT

public:
	CTestSelectPage(QWidget *parent = 0);

	void RefreshTestTree(int nTest,TEST_INFO *pInfo);

	unsigned long long  GetTestMask();

	void clearSelect();

private slots:
	void updateStyles(QTreeWidgetItem *item, int column);

private:
	QTreeWidget *m_pTestTree;			 /**< 测试列表 */
	unsigned long long m_TestMask;		 /**< 测试不会多于64个，所以每一位代表一个测试 */
};

///////////////////////////////////////////////////////////////////////////////////////////
//////////						CMiscellaneousPage
///////////////////////////////////////////////////////////////////////////////////////////
class CMiscellaneousPage : public QWizardPage
{
	Q_OBJECT

public:
	CMiscellaneousPage(QWidget *parent = 0);

	void TestValid(bool be);

	void GetGrabRange(unsigned int *from,unsigned int *to);
	unsigned int GetTestInterval();
	bool IsFullScreen();
	string GetGrabPath();

	bool validatePage();

private slots:
	void FromRange(const QString& text);
	void ToRange(const QString& text);
	void grapGroupClicked(bool beOn);
	void setExistingDirectory();
	void intervalGroupClicked(bool beOn);
	void intervalValueChange(int value);
	void FullScrenClicked(bool bOn);

private:
	QGroupBox *m_GrabGroup;				/**< 抓图相关的控件 */
	QGroupBox *m_TestIntervalGroup;		/**< 测试间隔设置控件 */
	QLineEdit *m_picfrom;
	QLineEdit *m_picto;
	QLineEdit *m_dirLine;
	QSpinBox *m_intervalSpinBox;
	QDial *m_dial;
	QCheckBox *m_checkBoxFullScree;

	unsigned int m_nPicGrabFrom;		/**< 抓图的起始点 */
	unsigned int m_nPicGrabTo;			/**< 抓图的终止点 */
	unsigned int m_nInterval;			/**< 测试用例的间隔时间 */
	bool m_bFullscreen;					/**< 是否全屏 */

	bool m_haveTest;					/**< 是否选择了test */
};

#endif	//__WIZARD_NEW_SCENE__

