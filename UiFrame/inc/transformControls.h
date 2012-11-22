#ifndef UIFRAME_TRANSFORM_CONTROLS
#define UIFRAME_TRANSFORM_CONTROLS

#include <QObject>

class QWidget;
class QSlider;

class CTransformControls : public QObject
{
	Q_OBJECT

public:
	CTransformControls();
	~CTransformControls();

	QWidget* createControls();

private slots:
	void sliderPressed();
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

private:
	QSlider *xRotate;	 /**< x��ת */
	QSlider *yRotate;	 /**< y��ת */
	QSlider *zRotate;	 /**< z��ת */

	int focusSlider;	 /**< ���ڲ�����slider */

};


#endif	//UIFRAME_TRANSFORM_CONTROLS