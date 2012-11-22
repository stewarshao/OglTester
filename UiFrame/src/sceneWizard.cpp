#include <QtGui>
#include "sceneWizard.h"
#include "uiframe.h"

CNewSceneWizard::CNewSceneWizard()
{
	m_sselect = NULL;
	m_tselect = NULL;
	m_mis = NULL;
	m_wizard = new QWizard;		// wizard时非模对话框
	if (NULL != m_wizard)
	{
		m_sselect = new CSceneSeclectPage;
		m_wizard->addPage(m_sselect);

		m_tselect = new CTestSelectPage;
		m_wizard->addPage(m_tselect);

		m_mis = new CMiscellaneousPage;
		m_wizard->addPage(m_mis);

		m_wizard->setWindowTitle("Scene Creation Wizard");
		m_wizard->setOption(QWizard::HaveCustomButton1);
		m_wizard->setButtonText(QWizard::CustomButton1,"Refresh");
		connect(m_wizard->button(QWizard::CustomButton1), SIGNAL(clicked()),this, SLOT(reload()));
		connect(m_wizard,SIGNAL(currentIdChanged(int)),this,SLOT(pageChange(int)));
		connect(m_wizard->button(QWizard::NextButton), SIGNAL(clicked()),this, SLOT(next()));
		connect(m_wizard->button(QWizard::FinishButton), SIGNAL(clicked()),this, SLOT(finish()));
		connect(m_wizard->button(QWizard::CancelButton), SIGNAL(clicked()),this, SLOT(cancel()));
	}
}

CNewSceneWizard::~CNewSceneWizard()
{
	if (NULL != m_wizard)
		delete m_wizard;
} 

void CNewSceneWizard::show()
{
	if (NULL != m_wizard)
		m_wizard->show();
}

void CNewSceneWizard::reload()
{
	string *sceneTitleList = NULL;
	string *sceneAuthorList = NULL;
	string *sceneVersionList = NULL;
	string *sceneDecList = NULL;
	string *moduleName = NULL;

	// 刷新场景信息
	CSceneManager::Instance()->RefreshScene();

	int nScene = CSceneManager::Instance()->GetSceneList(sceneTitleList,sceneAuthorList,sceneVersionList,sceneDecList,moduleName);
	if (0 == nScene)
		return;

	SCENE_INFO *pSceneInfo = new SCENE_INFO[nScene];
	
	for (int i = 0; i < nScene; ++i)
	{
		pSceneInfo[i].Title = sceneTitleList[i];
		pSceneInfo[i].Author = sceneAuthorList[i];
		pSceneInfo[i].Version = sceneVersionList[i];
		pSceneInfo[i].Description = sceneDecList[i];
		pSceneInfo[i].ModuleName = moduleName[i];
	}
	
	delete []sceneTitleList;
	delete []sceneAuthorList;
	delete []sceneVersionList;
	delete []sceneDecList;
	delete []moduleName;

	// UI
	m_sselect->clearView();
	m_sselect->RefreshTable(nScene,pSceneInfo);
}

void CNewSceneWizard::reloadTest(const string& sScene)
{
	TEST_INFO* pTestInfo = NULL;
	// 获取测试例程信息
	int nTest = CSceneManager::Instance()->GetTestList(sScene,pTestInfo);
	if (0 == nTest)
		return;

	// UI
	m_tselect->RefreshTestTree(nTest,pTestInfo);
}

void CNewSceneWizard::pageChange(int id)
{
	if (1 <= id)
	{
		m_wizard->button(QWizard::CustomButton1)->setVisible(false);
	}
	else if (0 == id)
	{
		m_wizard->button(QWizard::CustomButton1)->setVisible(true);
	}
}

void CNewSceneWizard::next()
{
	int nPage = m_wizard->currentId();

	if (1 == nPage)
	{
		m_tselect->clearSelect();

		string p = m_sselect->getSelcetScene();

		m_tselect->setTitle(QString("Test Selection of %1").arg(p.c_str()));
		// 获取测试列表
		reloadTest(p);
	}
	else if (2 == nPage)
	{
		m_mis->TestValid(m_tselect->GetTestMask()?true:false);
	}
}

void CNewSceneWizard::finish()
{
	// 检测测试参数，开启测试场景
	unsigned int grabfrom,grabto;
	m_mis->GetGrabRange(&grabfrom,&grabto);

	if (grabfrom != (unsigned int)-1 && grabto != (unsigned int)-1 && grabfrom > grabto)
		return;

	string strgrabpath;
	if (grabfrom != (unsigned int)-1 && grabto != (unsigned int)-1 && grabfrom <= grabto)
	{
		strgrabpath = m_mis->GetGrabPath();
	}

	unsigned int testinterval = m_mis->GetTestInterval();

	string p = m_sselect->getSelcetScene();

	CSceneManager::Instance()->TriggerTest(p,m_tselect->GetTestMask(),grabfrom,grabto,strgrabpath,testinterval,m_mis->IsFullScreen());
}

void CNewSceneWizard::cancel()
{
	g_MainWnd->enableNewWizard();
}

///////////////////////////////////////////////////////////////////////////////////////////
//////////						CSceneSeclectPage
///////////////////////////////////////////////////////////////////////////////////////////
CSceneSeclectPage::CSceneSeclectPage(QWidget *parent)
: QWizardPage(parent)
{
	setTitle("Scene Selection");

	m_pTable = new QTableWidget();
	//m_pTable->setWindowModality(Qt::WindowModal);					// 设置为模态对话框
	m_pTable->setEditTriggers(QAbstractItemView::NoEditTriggers);			// 禁止编辑
	m_pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pTable->setSelectionMode(QAbstractItemView::SingleSelection);
	m_pTable->setColumnCount(5);

	QHeaderView *header;
	header = m_pTable->horizontalHeader();
	//header->setResizeMode(QHeaderView::Fixed);
	header->setDefaultAlignment(Qt::AlignLeft);
	m_pTable->setColumnWidth(0,80);
	m_pTable->setColumnWidth(1,80);
	m_pTable->setColumnWidth(2,50);
	m_pTable->setColumnWidth(3,150);
	m_pTable->setColumnWidth(4,80);
	QHeaderView *verticalHeader = m_pTable->verticalHeader();
	connect(verticalHeader,SIGNAL(sectionClicked(int)),this,SLOT(headerClicked(int)));

	m_NameLineEdit = new QLineEdit;
	m_NameLineEdit->setVisible(false);
	registerField("SceneName*", m_NameLineEdit);		// 千万不要忘了这个*

	connect(m_pTable,SIGNAL(cellClicked(int,int)),this,SLOT(itemClicked(int,int)));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_pTable);
	//layout->addWidget(classNameLineEdit);
	setLayout(layout);
}

void CSceneSeclectPage::RefreshTable(int nScene,SCENE_INFO *pInfo)
{
	QStringList headerLabels;
	headerLabels << "Title" << "Author" << "Version" << "Description" << "ModuleName";
	m_pTable->setHorizontalHeaderLabels(headerLabels);
	m_pTable->setRowCount(nScene);
	for (int row = 0; row < nScene; ++row) 
	{
		QTableWidgetItem *item0 = new QTableWidgetItem(pInfo[row].Title.c_str());
		QTableWidgetItem *item1 = new QTableWidgetItem(pInfo[row].Author.c_str());
		QTableWidgetItem *item2 = new QTableWidgetItem(pInfo[row].Version.c_str());
		QTableWidgetItem *item3 = new QTableWidgetItem(pInfo[row].Description.c_str());
		QTableWidgetItem *item4 = new QTableWidgetItem(pInfo[row].ModuleName.c_str());

		m_pTable->setItem(row, 0, item0);
		m_pTable->setItem(row, 1, item1);
		m_pTable->setItem(row, 2, item2);
		m_pTable->setItem(row, 3, item3);
		m_pTable->setItem(row, 4, item4);
	}
	//m_pTable->resizeColumnsToContents();
}

void CSceneSeclectPage::itemClicked(int row,int cloumn)
{
	m_NameLineEdit->insert(QString("%1").arg(row));
}

void CSceneSeclectPage::headerClicked(int i)
{
	m_NameLineEdit->insert(QString("%1").arg(i));
}

string CSceneSeclectPage::getSelcetScene()
{
	int nScene = m_pTable->currentRow();

	return m_pTable->item(nScene,4)->data(Qt::DisplayRole).toString().toStdString();
}

void CSceneSeclectPage::clearView()
{
	m_pTable->clear();
	m_NameLineEdit->clear();
}

///////////////////////////////////////////////////////////////////////////////////////////
//////////						CTestSelectPage
///////////////////////////////////////////////////////////////////////////////////////////
CTestSelectPage::CTestSelectPage(QWidget *parent)
	:QWizardPage(parent)
{
	m_TestMask = 0;

	setTitle("Test Selection");

	m_pTestTree = new QTreeWidget();

	m_pTestTree->setColumnCount(1);
	m_pTestTree->setHeaderLabels(QStringList() << tr("Test"));
	connect(m_pTestTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
		this, SLOT(updateStyles(QTreeWidgetItem*,int)));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_pTestTree);
	setLayout(layout);
}

void CTestSelectPage::RefreshTestTree(int nTest,TEST_INFO *pInfo)
{
	list<TEST_INFO*>::iterator pos;
	for (int i = 0; i < nTest; ++i)
	{
		if (pInfo[i].Name == "Sub Scene")
			continue;
		QTreeWidgetItem *MajorTest = new QTreeWidgetItem(m_pTestTree);
		MajorTest->setText(0, pInfo[i].Name.c_str());
		MajorTest->setCheckState(0, Qt::Unchecked);
		if (!pInfo[i].SubTest.empty())
		{
			for (pos = pInfo[i].SubTest.begin(); pos != pInfo[i].SubTest.end(); ++pos)
			{
				TEST_INFO *pSub = *pos;
				QTreeWidgetItem *MinorTest = new QTreeWidgetItem(MajorTest);
				MinorTest->setText(0, pSub->Name.c_str());
				MinorTest->setCheckState(0, Qt::Unchecked);
				MinorTest->setData(0, Qt::UserRole,QVariant(pSub->TestIndex));
			}
		}
	}
}

void CTestSelectPage::updateStyles(QTreeWidgetItem *item, int column)
{
	if (!item || column != 0)
		return;

	Qt::CheckState state = item->checkState(0);
	QTreeWidgetItem *parent = item->parent();

	if (parent) 
	{
		// Only count style items.
		int nBit = item->data(0, Qt::UserRole).toInt();
		if (state == Qt::Checked)
			m_TestMask |= (1 << nBit);
		else
			m_TestMask &= ~(1 << nBit);

		if (state == Qt::Checked && parent->checkState(0) == Qt::Unchecked) 
		{
			// Mark parent items when child items are checked.
			parent->setCheckState(0, Qt::Checked);

		} 
		else if (state == Qt::Unchecked && parent->checkState(0) == Qt::Checked) 
		{
			bool marked = false;
			for (int row = 0; row < parent->childCount(); ++row)
			{
				if (parent->child(row)->checkState(0) == Qt::Checked) 
				{
					marked = true;
					break;
				}
			}
			
			// Unmark parent items when all child items are unchecked.
			if (!marked)
				parent->setCheckState(0, Qt::Unchecked);
		}
	}
	else
	{
		int row;
		int number = 0;
		for (row = 0; row < item->childCount(); ++row) 
		{
			if (item->child(row)->checkState(0) == Qt::Checked)
				++number;
		}

		// Mark/unmark all child items when marking/unmarking top-level
		// items.
		if (state == Qt::Checked && number == 0)
		{
			for (row = 0; row < item->childCount(); ++row)
			{
				if (item->child(row)->checkState(0) == Qt::Unchecked)
					item->child(row)->setCheckState(0, Qt::Checked);  // 调用此函数，会在updateStyles内递归
			}
		} 
		else if (state == Qt::Unchecked && number > 0) {
			for (row = 0; row < item->childCount(); ++row)
			{
				if (item->child(row)->checkState(0) == Qt::Checked)
					item->child(row)->setCheckState(0, Qt::Unchecked);
			}
		}
	}
}

unsigned long long CTestSelectPage::GetTestMask()
{
	return m_TestMask;
}

void CTestSelectPage::clearSelect()
{
	m_TestMask = 0;
	m_pTestTree->clear();
}

///////////////////////////////////////////////////////////////////////////////////////////
//////////						CMiscellaneousPage
///////////////////////////////////////////////////////////////////////////////////////////
CMiscellaneousPage::CMiscellaneousPage(QWidget *parent)
	:QWizardPage(parent)
{
	setTitle("Miscellaneous");

	// grab option
	m_GrabGroup = new QGroupBox(tr("G&rab Options"));
	m_GrabGroup->setCheckable(true);
	m_GrabGroup->setChecked(false);
	connect(m_GrabGroup, SIGNAL(clicked(bool)),
		this, SLOT(grapGroupClicked(bool)));

	// 以调用glswapbuffer/glfinish/glflush的次数范围来抓图
	QLabel *fromLabel = new QLabel(tr("Pic F&rom:"));
	m_picfrom = new QLineEdit;
	m_picfrom->setText(tr("1"));
	fromLabel->setBuddy(m_picfrom);
	m_nPicGrabFrom = (unsigned int)-1;
	connect(m_picfrom, SIGNAL(textEdited(const QString&)),
		this, SLOT(FromRange(const QString&)));
	
	QLabel *toLabel = new QLabel(tr("Pic T&o:"));
	m_picto = new QLineEdit;
	m_picto->setText(tr("1"));
	toLabel->setBuddy(m_picto);
	m_nPicGrabTo = (unsigned int)-1;
	connect(m_picto, SIGNAL(textEdited(const QString&)),
		this, SLOT(ToRange(const QString&)));

	QLabel *dirLabel = new QLabel(tr("Grab Directory:"));
	m_dirLine = new QLineEdit;
	m_dirLine->setReadOnly(true);
	m_dirLine->setText(QDir::currentPath());

	QPushButton *dirBrowse = new QPushButton("Browse..");
	connect(dirBrowse, SIGNAL(clicked()),this, SLOT(setExistingDirectory()));

	QGridLayout *grabLayout = new QGridLayout;
	grabLayout->setSizeConstraint(QLayout::SetNoConstraint);
	grabLayout->addWidget(fromLabel, 0, 0);
	grabLayout->addWidget(m_picfrom, 0, 1);
	grabLayout->addWidget(toLabel,1,0);
	grabLayout->addWidget(m_picto,1,1);
	grabLayout->addWidget(dirLabel, 2, 0);
	grabLayout->addWidget(m_dirLine, 2, 1);
	grabLayout->addWidget(dirBrowse,2,2);
	m_GrabGroup->setLayout(grabLayout);

	// Test interval
	m_TestIntervalGroup = new QGroupBox(tr("Test I&nterval Options"));
	m_TestIntervalGroup->setCheckable(true);
	m_TestIntervalGroup->setChecked(false);
	connect(m_TestIntervalGroup, SIGNAL(clicked(bool)),
		this, SLOT(intervalGroupClicked(bool)));
	m_nInterval = (unsigned int)-1;

	m_dial = new QDial;
	m_dial->setFocusPolicy(Qt::StrongFocus);
	m_dial->setMaximum(10);
	m_dial->setMinimum(0);

	QLabel *intervalLabel = new QLabel(tr("Interval:"));
	m_intervalSpinBox = new QSpinBox;
	m_intervalSpinBox->setRange(0, 10);
	m_intervalSpinBox->setSingleStep(1);

	connect(m_dial, SIGNAL(valueChanged(int)), m_intervalSpinBox, SLOT(setValue(int)));
	connect(m_intervalSpinBox, SIGNAL(valueChanged(int)), m_dial, SLOT(setValue(int)));
	connect(m_intervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(intervalValueChange(int)));

	QGridLayout *intervalLayout = new QGridLayout;
	intervalLayout->addWidget(intervalLabel,0,0);
	intervalLayout->addWidget(m_intervalSpinBox,0,1,1,2);
	intervalLayout->addWidget(m_dial,0,3);
	intervalLayout->setColumnStretch(4,5);
	m_TestIntervalGroup->setLayout(intervalLayout);

	// fullscreen
	QCheckBox *checkBoxFullScreen = new QCheckBox(tr("&FullScreen"));
	connect(checkBoxFullScreen, SIGNAL(clicked(bool)),
		this, SLOT(FullScrenClicked(bool)));
	m_bFullscreen = false;

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(m_GrabGroup,0,0,1,2);
	layout->addWidget(m_TestIntervalGroup,1,0);
	layout->addWidget(checkBoxFullScreen,1,1);
	setLayout(layout);
}

void CMiscellaneousPage::FromRange(const QString& text)
{
	m_nPicGrabFrom = text.toUInt();
	if (m_nPicGrabFrom <= 1)
		m_nPicGrabFrom =1;
	m_picfrom->setText(QString("%1").arg(m_nPicGrabFrom));
}

void CMiscellaneousPage::ToRange(const QString& text)
{
	m_nPicGrabTo = text.toUInt();
	if (m_nPicGrabTo <= 1)
		m_nPicGrabTo = 1;
	m_picto->setText(QString("%1").arg(m_nPicGrabTo));
}

void CMiscellaneousPage::GetGrabRange(unsigned int *from,unsigned int *to)
{
	*from = m_nPicGrabFrom;
	*to = m_nPicGrabTo;
}

void CMiscellaneousPage::grapGroupClicked(bool beOn)
{
	if (!beOn)
	{
		m_nPicGrabFrom = (unsigned int)-1;
		m_nPicGrabTo = (unsigned int)-1;
		m_picfrom->setText(tr("1"));
		m_picto->setText(tr("1"));
		m_dirLine->setText(QDir::currentPath());
	}
	else
	{
		m_nPicGrabFrom = 1;
		m_nPicGrabTo = 1;
	}
}

void CMiscellaneousPage::setExistingDirectory()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	options |= QFileDialog::DontUseNativeDialog;
	QString directory = QFileDialog::getExistingDirectory(0,
		tr("Grab Save as..."),
		m_dirLine->text(),
		options);
	if (!directory.isEmpty())
		m_dirLine->setText(directory);
}

void CMiscellaneousPage::intervalGroupClicked(bool beOn)
{
	if (!beOn)
	{
		m_dial->setMinimum(0);
		m_intervalSpinBox->setMinimum(0);
		m_intervalSpinBox->setValue(0);
		m_nInterval = (unsigned int)-1;
	}
	else
	{
		if (!m_haveTest)
		{
			m_TestIntervalGroup->setChecked(false);
			return;
		}
		m_dial->setMinimum(1);
		m_intervalSpinBox->setMinimum(1);
		m_intervalSpinBox->setValue(1);
	}
}

void CMiscellaneousPage::intervalValueChange(int value)
{
	m_nInterval = m_intervalSpinBox->value();
}

void CMiscellaneousPage::FullScrenClicked(bool bOn)
{
	m_bFullscreen = bOn;
}

unsigned int CMiscellaneousPage::GetTestInterval()
{
	return m_nInterval;
}

bool CMiscellaneousPage::IsFullScreen()
{
	return m_bFullscreen;
}

void CMiscellaneousPage::TestValid(bool be)
{
	m_haveTest = be;
}

string CMiscellaneousPage::GetGrabPath()
{
	return m_dirLine->text().toStdString();
}

bool CMiscellaneousPage::validatePage() 
{
	QString error;
	if (m_nPicGrabFrom != (unsigned int)-1 && m_nPicGrabTo != (unsigned int)-1 && (m_nPicGrabTo < m_nPicGrabFrom))
	{
		QMessageBox::warning(this, tr("Miscellaneous error"),
			QString("Grabe range error![from %1 > to %2]").arg(m_nPicGrabFrom).arg(m_nPicGrabTo));
		return false;
	}

	return true;
}