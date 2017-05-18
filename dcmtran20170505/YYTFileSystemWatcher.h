#ifndef YYTFILESYSTEMWATCHER_H
#define YYTFILESYSTEMWATCHER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QFileSystemWatcher>

class YYTFileSystemWatcher:public QObject
{
	Q_OBJECT

public:
	explicit YYTFileSystemWatcher(QObject *parent = 0);
	~YYTFileSystemWatcher();

	static YYTFileSystemWatcher * getInstance();
static void addWatchPath(QString path);
static void removeWatchPath(QString path);

signals:
	void	signal_AddNewFile(QString filePath);
public slots:
	void directoryUpdated(const QString &path);  // Ŀ¼����ʱ���ã�path�Ǽ�ص�·��
	void fileUpdated(const QString &path);   // �ļ����޸�ʱ���ã�path�Ǽ�ص�·��

private:
	static YYTFileSystemWatcher *m_pInstance; // ����
	QFileSystemWatcher *m_pSystemWatcher;  // QFileSystemWatcher����
	QMap<QString, QStringList> m_currentContentsMap; // ��ǰÿ����ص�����Ŀ¼�б�

private:
	
};

#endif // YYTFILESYSTEMWATCHER_H
