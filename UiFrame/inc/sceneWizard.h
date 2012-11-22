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

// ������Ϣ�ṹ��
typedef struct
{
	string	Title;			/**< �������� */
	string	Author;			/**< �������� */
	string	Version;		/**< �����汾 */
	string	Description;	/**< �������� */
	string	ModuleName;		/**< �ļ��� */
} SCENE_INFO;

class CNewSceneWizard : public QObject
{
	Q_OBJECT

public:
	CNewSceneWizard();
	~CNewSceneWizard();

	void show();								/**< ��ʾwizard */

public slots:
	void reload();								/**< ˢ�³��� */
	void reloadTest(const string& sScene);		/**< ˢ�²��� */

private slots:
	void pageChange(int id);					/**< ��ҳ�ı���ź� */
	void next();								
	void finish();
	void cancel();

private:
	QWizard* m_wizard;					/**< ��������wizard */
	CSceneSeclectPage *m_sselect;		/**< ����ѡ��page */
	CTestSelectPage *m_tselect;			/**< ����ѡ��page */
	CMiscellaneousPage *m_mis;			/**< ����ѡ��page */

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
	QTableWidget* m_pTable;				/**< �����б� */
	QLineEdit *m_NameLineEdit;			/**< ��Ҫ����disable next */
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
	QTreeWidget *m_pTestTree;			 /**< �����б� */
	unsigned long long m_TestMask;		 /**< ���Բ������64��������ÿһλ����һ������ */
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
	QGroupBox *m_GrabGroup;				/**< ץͼ��صĿؼ� */
	QGroupBox *m_TestIntervalGroup;		/**< ���Լ�����ÿؼ� */
	QLineEdit *m_picfrom;
	QLineEdit *m_picto;
	QLineEdit *m_dirLine;
	QSpinBox *m_intervalSpinBox;
	QDial *m_dial;
	QCheckBox *m_checkBoxFullScree;

	unsigned int m_nPicGrabFrom;		/**< ץͼ����ʼ�� */
	unsigned int m_nPicGrabTo;			/**< ץͼ����ֹ�� */
	unsigned int m_nInterval;			/**< ���������ļ��ʱ�� */
	bool m_bFullscreen;					/**< �Ƿ�ȫ�� */

	bool m_haveTest;					/**< �Ƿ�ѡ����test */
};

#endif	//__WIZARD_NEW_SCENE__

