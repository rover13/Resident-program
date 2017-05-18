#include "NetworkHelper.h"
#include "Settings.h"

NetworkHelper*  NetworkHelper::m_pInstance = nullptr;

NetworkHelper::NetworkHelper(QObject* parent)
    :QObject(parent), m_pNetworkAccessManager(new QNetworkAccessManager)
    , m_pTimer(new QTimer)
{
    connect( m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(DoReplyFinished(QNetworkReply*)) );
    connect( m_pTimer, SIGNAL(timeout()), this, SLOT(DoTimer()) );
}

NetworkHelper::~NetworkHelper()
{
    if(nullptr != m_pNetworkAccessManager)
    {
        delete m_pNetworkAccessManager;
        m_pNetworkAccessManager = nullptr;
    }

    if(nullptr != m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = nullptr;
    }
}

NetworkHelper *NetworkHelper::getInstance()
{
    if(nullptr == m_pInstance)
    {
        m_pInstance = new NetworkHelper;
    }

    return m_pInstance;
}

void NetworkHelper::DoReplyFinished(QNetworkReply *reply)
{
    m_pTimer->stop();
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        qDebug() << "Receive the server response " << m_fileName << bytes;

        emit signal_Finished(m_fileName, true);
    }
    else
    {

        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug( qPrintable(reply->errorString()) );

        emit signal_Finished(m_fileName, false);
    }

    reply->deleteLater();
   // this->deleteLater();
}

void NetworkHelper::DoTimer()
{
    m_pTimer->stop();

    qDebug() << "send " << m_fileName << "timeout";

    emit signal_Finished(m_fileName, false);

    this->deleteLater();
}

void NetworkHelper::excuteGet()
{
    QNetworkRequest network_request;
    network_request.setUrl( QUrl(m_url) );
    m_pNetworkAccessManager->get(network_request);
    m_pTimer->start(LimitTimeOut);
}

void NetworkHelper::excutePost(QString strFileName, QString strUrl, QByteArray strContent)
{
    m_url = strUrl;
    m_fileName = strFileName;

    QNetworkRequest network_request;

    if(m_fileName.contains(".xml"))
    {
        auto index = m_fileName.lastIndexOf("/");
        auto str = m_fileName.right(m_fileName.size() - index);
        m_url += "?filename=" + str;
        int i = 0;
    }
    else {
        auto str = Settings().casesDir();
        auto index = m_fileName.indexOf(str);
        m_url += "?filename=" + m_fileName.right(m_fileName.size() - index - str.size());
    }


    network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    network_request.setUrl( QUrl(m_url) );

    //QByteArray post_data;
    //post_data.append(strContent);
    m_pNetworkAccessManager->post(network_request, strContent);
    m_pTimer->start(LimitTimeOut);
}
