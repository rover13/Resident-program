#include "YYTFileSystemWatcher.h"
#include <QDebug>
#include <QFileinfo>
#include <QDir>
#include <QSet>
#include "FileManage.h"


YYTFileSystemWatcher* YYTFileSystemWatcher::m_pInstance = NULL;

YYTFileSystemWatcher::YYTFileSystemWatcher(QObject *parent)
	: QObject(parent)
{

        m_pSystemWatcher = new QFileSystemWatcher();

		// ����QFileSystemWatcher��directoryChanged��fileChanged�źŵ���Ӧ�Ĳ�
        connect(m_pSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryUpdated(QString)));
        connect(m_pSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));
//	}
}

YYTFileSystemWatcher::~YYTFileSystemWatcher()
{

}

YYTFileSystemWatcher * YYTFileSystemWatcher::getInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new YYTFileSystemWatcher;
	}

	return m_pInstance;

}

void YYTFileSystemWatcher::addWatchPath(QString path)
{
	qDebug() << QString("Add to watch: %1").arg(path);

	

	// ��Ӽ��·��
	m_pInstance->m_pSystemWatcher->addPath(path);

	// ������·����һ��Ŀ¼�����浱ǰ�����б�
	QFileInfo file(path);
	if (file.isDir())
	{
		const QDir dirw(path);
		m_pInstance->m_currentContentsMap[path] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    }
}

void YYTFileSystemWatcher::removeWatchPath(QString path)
{
    qDebug() << QString("remove to watch: %1").arg(path);

    // remove���·��
    m_pInstance->m_pSystemWatcher->removePath(path);
    m_pInstance->m_currentContentsMap.remove(path);
}

void YYTFileSystemWatcher::directoryUpdated(const QString &path)
{
	// �Ƚ����µ����ݺͱ���������ҳ�����(�仯)
	QStringList currEntryList = m_currentContentsMap[path];
	const QDir dir(path);

	QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

	QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
	QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

	// ������ļ�
	QSet<QString> newFiles = newDirSet - currentDirSet;
	QStringList newFile = newFiles.toList();

	// �ļ��ѱ��Ƴ�
	QSet<QString> deletedFiles = currentDirSet - newDirSet;
	QStringList deleteFile = deletedFiles.toList();

	// ���µ�ǰ����
	m_currentContentsMap[path] = newEntryList;

	if (!newFile.isEmpty() && !deleteFile.isEmpty())
	{
		// �ļ�/Ŀ¼������
		if ((newFile.count() == 1) && (deleteFile.count() == 1))
		{
			qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
		}
	}
	else
	{
		// ������ļ�/Ŀ¼��Dir
		if (!newFile.isEmpty())
		{
            qDebug() << "New Files/Dirs added: " << newFile;

			foreach (QString file, newFile)
			{
				// �������ÿ�����ļ�....
                //emit signal_AddNewFile(QString(path) + QString("/") + QString(file));
                QFileInfo fileInfo(QString(path) + QString("/") + QString(file));
                if(fileInfo.isDir())
                {
                    auto fileMana = new FileManage(QString(path) + QString("/") + QString(file));
                }
			}
		}

		// ��Dir��ɾ���ļ�/Ŀ¼
		if (!deleteFile.isEmpty())
		{
			qDebug() << "Files/Dirs deleted: " << deleteFile;

			foreach(QString file, deleteFile)
			{
				// �������ÿ����ɾ�����ļ�....
			}
		}
	}
}

void YYTFileSystemWatcher::fileUpdated(const QString &path)
{
	QFileInfo file(path);
	QString strPath = file.absolutePath();
	QString strName = file.fileName();

	qDebug() << QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);
}
