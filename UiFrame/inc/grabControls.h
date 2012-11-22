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
	QToolButton* grabButton;				/**< 开启抓图的按钮 */ 
	QLineEdit *directoryLabel;				/**< 目录选择 */

	void *pMainWin;							/**< 父窗口指针 */
};


#endif	//UIFRAME_GRAB_CONTROLS