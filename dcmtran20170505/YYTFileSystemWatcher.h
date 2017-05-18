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
	void directoryUpdated(const QString &path);  // 目录更新时调用，path是监控的路径
	void fileUpdated(const QString &path);   // 文件被修改时调用，path是监控的路径

private:
	static YYTFileSystemWatcher *m_pInstance; // 单例
	QFileSystemWatcher *m_pSystemWatcher;  // QFileSystemWatcher变量
	QMap<QString, QStringList> m_currentContentsMap; // 当前每个监控的内容目录列表

private:
	
};

#endif // YYTFILESYSTEMWATCHER_H
