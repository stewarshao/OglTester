#include <QtGui>
#include "lightControls.h"

CLightControls::CLightControls()
{
	rdiffuse = gdiffuse = bdiffuse = 0.0f;
}

CLightControls::~CLightControls()
{
}

QWidget* CLightControls::createControls()
{
	colorButtonGroup = new QButtonGroup();
	colorButtonGroup->setExclusive(false);
	connect(colorButtonGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(colorChange(int)));

	QPixmap pix(16, 16);
	pix.fill(QColor(0,0,0));

	QToolButton *diffusecolorButton = new QToolButton;
	diffusecolorButton->setIcon(QIcon(pix));
	diffusecolorButton->setIconSize(QSize(50, 50));
	diffusecolorButton->setCheckable(true);
	colorButtonGroup->addButton(diffusecolorButton,0);

	QGridLayout *diffuseLayout = new QGridLayout;
	diffuseLayout->addWidget(diffusecolorButton, 0, 0, Qt::AlignHCenter);
	diffuseLayout->addWidget(new QLabel("Diffuse"), 1, 0, Qt::AlignCenter);
	QWidget *diffuseWidget = new QWidget;
	diffuseWidget->setLayout(diffuseLayout);

	QGridLayout *colorLayout = new QGridLayout;
	colorLayout->addWidget(diffuseWidget, 0, 0);

	colorLayout->setRowStretch(1, 10);
	colorLayout->setColumnStretch(1, 10);

	
	QWidget *colorWidget = new QWidget;
	colorWidget->setLayout(colorLayout);

	return colorWidget;
}

void CLightControls::colorChange(int index)
{
	QColor srcColor;
	if (0 == index)
	{
		// diffuse color
		srcColor.setRgbF(rdiffuse,gdiffuse,bdiffuse);
		QColor col = QColorDialog::getColor(srcColor);
		if (!col.isValid())
			return;
		
		col.getRgbF(&rdiffuse,&gdiffuse,&bdiffuse);
		QPixmap pix(16, 16);
		pix.fill(col);
		colorButtonGroup->button(index)->setIcon(QIcon(pix));
	}
}