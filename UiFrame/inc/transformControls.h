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
	QSlider *xRotate;	 /**< x旋转 */
	QSlider *yRotate;	 /**< y旋转 */
	QSlider *zRotate;	 /**< z旋转 */

	int focusSlider;	 /**< 正在操作的slider */

};


#endif	//UIFRAME_TRANSFORM_CONTROLS