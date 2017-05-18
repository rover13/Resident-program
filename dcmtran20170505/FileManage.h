#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <QObject>
#include <QTimer>
#include "SendCasesFile.h"
#include "SendPacsFile.h"
#include "Settings.h"

class FileManage:public QObject
{
    Q_OBJECT
public:
    FileManage(QString Dir, QObject* parent = nullptr);
    ~FileManage();

    void listFile(QString directory, QStringList& files);
private slots:
    void Timerout();
    void SendPacsFinish();
    void SendCasesFinish();
    void SendXmlFinish();
private:
    void initConnet();
    void timerEvent(QTimerEvent* event);
    void removefilesindir(const QString &path);
    void startRun();
private:
    QString                         m_strDir;
    QScopedPointer<QTimer>          m_Timer;
    QStringList                     m_fileList;
    QScopedPointer<SendPacsFile>    m_sendPacs;
    QScopedPointer<SendCasesFile>   m_sendCases;
    QString                         m_strXmlFileName;

    int m_fileTotal;
    int m_updateTimer;
};

#endif // FILEMANAGE_H
