#include "uiframe.h"
#include <QtGui/QApplication>
#include <QFSFileEngine>

UiFrame *g_MainWnd = NULL;

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(uiframe);	// ע������Ϊqrc�ļ���������qrc��ָ�����ļ�������ڣ���Ϊrcc�����qrc�ļ�ָ��������������Ӧ��qrc_*.cpp������Դ�����ƻ�

#if (defined(WIN32) && !defined(QT_NO_DEBUG))
	QString path;  
	QDir dir;  
	path=dir.currentPath();  

	if (!path.contains("Output"))
	{
		// ��vc��������
		QFSFileEngine::setCurrentPath(QString("../Output/bin/Debug"));
	}
#endif	

	QApplication a(argc, argv);
	UiFrame w;
	w.setGeometry(100, 100, 800, 500);
	w.showMaximized();
	g_MainWnd = &w;
	//w.show();
	return a.exec();
}
