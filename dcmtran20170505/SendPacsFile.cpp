#include "SendPacsFile.h"
#include "WorkThread.h"
#include "Settings.h"

SendPacsFile::SendPacsFile(QObject *parent)
    :QObject(parent), m_worker(nullptr)
{
}

SendPacsFile::~SendPacsFile()
{
    if (m_worker) {
        m_worker->terminate();
    }
}

void SendPacsFile::UpLoad(QString strHost, QString strAETit, int strPort, QString strDirec)
{
    if (m_worker) {
        m_worker->terminate();
    }

    m_worker.reset(new WorkThread(strHost, strAETit, strPort, strDirec));
    connect(m_worker.data(), SIGNAL(OnFinish()), SLOT(DoFinish()));

    m_worker->start();

   // auto s = Settings();
   // s.setHost(strHost);
   // s.setTitle(strAETit);
   // s.setPort(strPort);
   // s.setDirectory(strDirec);

    qDebug() << "start send pacs in " << strDirec;
}

void SendPacsFile::DoFinish()
{
    if (m_worker) {
        m_worker->wait();
        m_worker.reset();
    }

    qDebug() << "finish send pacs file";

    emit OnFinish();

}
