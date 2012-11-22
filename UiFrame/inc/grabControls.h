#ifndef UIFRAME_GRAB_CONTROLS
#define UIFRAME_GRAB_CONTROLS

#include <QObject>

class QToolButton;
class QLineEdit;
class QWidget;

class CGrabControls : public QObject
{
	Q_OBJECT

public:
	CGrabControls(void* pMain);
	~CGrabControls();

	QWidget* createControls();

private slots:
	void grabFb();
	void setExistingDirectory();

private:
	QToolButton* grabButton;				/**< ����ץͼ�İ�ť */ 
	QLineEdit *directoryLabel;				/**< Ŀ¼ѡ�� */

	void *pMainWin;							/**< ������ָ�� */
};


#endif	//UIFRAME_GRAB_CONTROLS