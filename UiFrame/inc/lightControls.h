#ifndef UIFRAME_LIGHT_CONTROLS
#define UIFRAME_LIGHT_CONTROLS

#include <QObject>

class QButtonGroup;
class QWidget;

class CLightControls : public QObject
{
	Q_OBJECT

public:
	CLightControls();
	~CLightControls();

	QWidget* createControls();

private slots:
	void colorChange(int index);

private:
	QButtonGroup* colorButtonGroup;	 


	double rdiffuse,gdiffuse,bdiffuse;					/**< 光源漫反射颜色 */

};


#endif	//UIFRAME_LIGHT_CONTROLS