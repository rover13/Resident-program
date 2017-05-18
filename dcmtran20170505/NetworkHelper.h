#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <QObject>
#include <QByteArray>
#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QtCore>

#define LimitTimeOut 5000 //请求超时时间(毫秒)

enum E_NetworkReply
{
    E_NetOK,
    E_NetTimeOut,//超时
    E_NetReplyError,
};

class NetworkHelper:public QObject
{
    Q_OBJECT
public:
    static NetworkHelper *getInstance();

    explicit NetworkHelper(QObject* parent = 0);
    void excutePost(QString strFileName, QString strUrl, QByteArray strContent);
    ~NetworkHelper();

private slots:
    void DoReplyFinished(QNetworkReply* reply);
    void DoTimer();

signals:
    void signal_TimerOut();
    void signal_Finished(QString filename, bool bSuccess);

private:
    void excuteGet();


private:
    static NetworkHelper*                   m_pInstance;

    QNetworkAccessManager*                  m_pNetworkAccessManager;
    QTimer*                                 m_pTimer;
    QString                                 m_url;
   // QByteArray                              m_content;
    QString                                 m_fileName;
};

#endif // NETWORKHELPER_H
