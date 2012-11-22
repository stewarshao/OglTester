#include "grabControls.h"
#include <QtGui>
#include "glwidgetManager.h"
#include "uiframe.h"

CGrabControls::CGrabControls(void* pMain)
{
	pMainWin = pMain;
}

CGrabControls::~CGrabControls()
{
}

QWidget* CGrabControls::createControls()
{
	grabButton = new QToolButton;
	grabButton->setText(tr("Grab"));
	grabButton->setIcon(QIcon(":UiFrame/images/grabbutton.png"));
	grabButton->setIconSize(QSize(50, 50));
	grabButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	
	connect(grabButton, SIGNAL(clicked()),this, SLOT(grabFb()));

	directoryLabel = new QLineEdit;
	directoryLabel->setReadOnly(true);
#ifdef WIN32
	directoryLabel->setText(QString("C:/GrabPic"));
#else
	directoryLabel->setText(QString("/opt/GrabPic"));
#endif
	directoryLabel->setMinimumWidth(150);
	QPushButton *directoryButton = new QPushButton(tr("Grab &Save as.."));
	connect(directoryButton, SIGNAL(clicked()),this, SLOT(setExistingDirectory()));

	QGridLayout *grabLayout = new QGridLayout;
	grabLayout->addWidget(directoryButton,0,0);
	grabLayout->addWidget(directoryLabel,0,1);
	grabLayout->addWidget(grabButton, 1, 0);
	grabLayout->setRowStretch(2, 10);
	grabLayout->setColumnStretch(2, 10);

	QWidget *grabrWidget = new QWidget;
	grabrWidget->setLayout(grabLayout);

	return grabrWidget;
}

void CGrabControls::grabFb()
{
	UiFrame *mainWindow = (UiFrame *)pMainWin;
	mainWindow->grab(directoryLabel->text());
}

void CGrabControls::setExistingDirectory()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	options |= QFileDialog::DontUseNativeDialog;
	QString directory = QFileDialog::getExistingDirectory(0,
		tr("Grab Save as..."),
		directoryLabel->text(),
		options);
	if (!directory.isEmpty())
		directoryLabel->setText(directory);
}