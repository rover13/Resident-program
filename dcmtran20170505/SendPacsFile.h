#ifndef SENDPACSFILE_H
#define SENDPACSFILE_H

#include <QObject>
#include <QtCore>

class WorkThread;

class SendPacsFile:public QObject
{
    Q_OBJECT
public:
    explicit SendPacsFile(QObject* parent = 0);
    ~SendPacsFile();

    void UpLoad(QString strHost, QString strAETit, int strPort, QString strDirec);

signals:
    void OnFinish();
private slots:
    void DoFinish();
private:
    QScopedPointer<WorkThread> m_worker;
};

#endif // SENDPACSFILE_H
