#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QtCore>


class WorkThread : public QThread
{
	Q_OBJECT

public:
	WorkThread(QString host, QString title, int port, QString directory);

protected:
	virtual void run();

signals:
	void OnStart(int numFiles);
	void OnTransfer(QString file, QString result);
	void OnFinish();

private:
	QString m_host, m_title;
	int m_port;
    QString m_directory;
};


#endif // WORKTHREAD_H
