#ifndef SENDCASESFILE_H
#define SENDCASESFILE_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include "NetworkHelper.h"

class SendCasesFile:public QObject
{
    Q_OBJECT
public:
    explicit SendCasesFile(QObject* parent = 0);
    ~SendCasesFile();


    void startSendCasesFile(QString dir);
    void startSendXmlFile(QString fileName);
private:
    bool ReadFile(QString fileName);
    void listFile(QString directory, QStringList& files);
signals:
    void OnFinishAll();
    void OnXmlFinish();
private slots:
    void DoSingleFileFinish(QString fileName, bool bSucces);
    void DoXmlFileFinish(QString fileName, bool bSucces);

private:
    QString     m_strDir;
    QString     m_strFileName;
    QString     m_strFileType;

    QStringList m_fileList;
    QMap<QString, int> m_fileState;
    //QScopedPointer<NetworkHelper> m_pNetwork;
};



#endif // SENDCASESFILE_H
