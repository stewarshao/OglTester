#include <QtGui>
#include "transformControls.h"

CTransformControls::CTransformControls()
{
	focusSlider = -1;
}

CTransformControls::~CTransformControls()
{
}

QWidget* CTransformControls::createControls()
{
	xRotate = new QSlider(Qt::Horizontal);
	xRotate->setRange(0, 360 * 16);
	xRotate->setSingleStep(16);
	xRotate->setPageStep(15 * 16);
	xRotate->setTickInterval(15 * 16);
	xRotate->setTickPosition(QSlider::TicksAbove);
	xRotate->setMinimumWidth(150);
	connect(xRotate, SIGNAL(valueChanged(int)), this, SLOT(setXRotation(int)));

	yRotate = new QSlider(Qt::Horizontal);
	yRotate->setRange(0, 360 * 16);
	yRotate->setSingleStep(16);
	yRotate->setPageStep(15 * 16);
	yRotate->setTickInterval(15 * 16);
	yRotate->setTickPosition(QSlider::TicksAbove);
	yRotate->setMinimumWidth(150);
	connect(yRotate, SIGNAL(valueChanged(int)), this, SLOT(setYRotation(int)));

	zRotate = new QSlider(Qt::Horizontal);
	zRotate->setRange(0, 360 * 16);
	zRotate->setSingleStep(16);
	zRotate->setPageStep(15 * 16);
	zRotate->setTickInterval(15 * 16);
	zRotate->setTickPosition(QSlider::TicksAbove);
	zRotate->setMinimumWidth(150);
	connect(zRotate, SIGNAL(valueChanged(int)), this, SLOT(setZRotation(int)));

	QVBoxLayout *layout = new QVBoxLayout;
	
	QGridLayout *textLayoutx = new QGridLayout;
	textLayoutx->addWidget(new QLabel(tr("xRotation")), 0, 0, Qt::AlignLeft);
	textLayoutx->addWidget(xRotate, 0, 1, Qt::AlignCenter);
	textLayoutx->setColumnStretch(2,10);
	QWidget *textWidgetx = new QWidget;
	textWidgetx->setLayout(textLayoutx);
	layout->addWidget(textWidgetx);

	QGridLayout *textLayouty = new QGridLayout;
	textLayouty->addWidget(new QLabel(tr("yRotation")), 0, 0, Qt::AlignLeft);
	textLayouty->addWidget(yRotate, 0, 1, Qt::AlignCenter);
	textLayouty->setColumnStretch(2,10);
	QWidget *textWidgety = new QWidget;
	textWidgety->setLayout(textLayouty);
	layout->addWidget(textWidgety);	

	QGridLayout *textLayoutz = new QGridLayout;
	textLayoutz->addWidget(new QLabel(tr("zRotation")), 0, 0, Qt::AlignLeft);
	textLayoutz->addWidget(zRotate, 0, 1, Qt::AlignCenter);
	textLayoutz->setColumnStretch(2,10);
	QWidget *textWidgetz = new QWidget;
	textWidgetz->setLayout(textLayoutz);
	layout->addWidget(textWidgetz);

	layout->addStretch();

	QWidget *itemWidget = new QWidget;
	itemWidget->setLayout(layout);

	return itemWidget;
}

void CTransformControls::sliderPressed()
{

}

void CTransformControls::setXRotation(int angle)
{

}

void CTransformControls::setYRotation(int angle)
{

}

void CTransformControls::setZRotation(int angle)
{

}