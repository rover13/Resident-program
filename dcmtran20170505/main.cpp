#include <QtCore>
#include <QApplication>
//#include "MainWindow.h"
#include "SendPacsFile.h"
#include "OperationXML.h"
#include "FileManage.h"
#include <windows.h>
#include "YYTFileSystemWatcher.h"


int
main(int argc, char* argv[])
{
	QApplication app{argc, argv};

    auto fileWatcher = YYTFileSystemWatcher::getInstance();
    fileWatcher->addWatchPath(Settings().watcherPath());

	return app.exec();
}
