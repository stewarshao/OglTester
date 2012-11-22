#include "uiframe.h"
#include <QtGui/QApplication>
#include <QFSFileEngine>

UiFrame *g_MainWnd = NULL;

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(uiframe);	// 注：参数为qrc文件名，而且qrc中指定的文件必须存在，因为rcc会根据qrc文件指定的内容生成相应的qrc_*.cpp，将资源二进制化

#if (defined(WIN32) && !defined(QT_NO_DEBUG))
	QString path;  
	QDir dir;  
	path=dir.currentPath();  

	if (!path.contains("Output"))
	{
		// 从vc调试启动
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
