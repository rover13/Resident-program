#include "WorkThread.h"
#include "Transfer.h"
#include "Settings.h"


WorkThread::WorkThread(QString host, QString title, int port, QString directory)
	: m_host(host), m_title(title), m_port(port), m_directory(directory)
{
}


static bool
in_blocklist(QString suffix)
{
	static auto blocklist =
		QSet<QString>::fromList(Settings().blocklist());

	return blocklist.contains(suffix.toLower());
}


template <class Cont>
static void
list_files(QString directory, Cont& files)
{
	QDir dir{directory};
	if (!dir.exists()) return;

	dir.setFilter(QDir::Files | QDir::Dirs);
	auto list = dir.entryInfoList();

	for (auto it : list) {
		QString name = it.fileName();
		if (name == "." || name == "..") continue;

		auto path = it.filePath();
		if (it.isDir())
			list_files(path, files);
		else if (!in_blocklist(it.suffix()))
			files.push_back(path);
	}
}


void
WorkThread::run()
{
	Transfer dcm{m_host, m_title, m_port};
	if (!dcm.isValid()) return;

	QStringList files;
	list_files(m_directory, files);
	emit OnStart(files.size());

	for (auto fname : files) {
		auto result = dcm.store(fname);
		emit OnTransfer(fname, result);
	}

	emit OnFinish();
}
