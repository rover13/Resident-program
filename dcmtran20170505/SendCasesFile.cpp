#include "SendCasesFile.h"
#include "NetworkHelper.h"
#include "Settings.h"
#include <QDebug>

const int SENDFILETIME = 0;

SendCasesFile::SendCasesFile( QObject *parent)
    :QObject(parent),m_strFileName(""),m_strFileType("xml")
{
    connect(NetworkHelper::getInstance(), SIGNAL(signal_Finished(QString,bool)), this, SLOT(DoXmlFileFinish(QString,bool)));
    connect(NetworkHelper::getInstance(), SIGNAL(signal_Finished(QString,bool)), this, SLOT(DoSingleFileFinish(QString,bool)));
}

SendCasesFile::~SendCasesFile()
{
    int i = 0;
}

void SendCasesFile::startSendCasesFile(QString dir)
{
    listFile(dir, m_fileList);
    foreach (auto filename, m_fileList)
    {
        m_fileState[filename] = 0;
        if(!ReadFile(filename))
        {
            m_fileState.remove(filename);
        }

    }

    if(m_fileState.isEmpty())
    {
        emit OnFinishAll();
    }
}

void SendCasesFile::startSendXmlFile(QString fileName)
{
    if(!ReadFile(fileName))
    {
        emit OnXmlFinish();
    }
}

bool SendCasesFile::ReadFile(QString fileName)
{
    if(fileName.isEmpty())
    {
        return false;
    }
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "read file error, file name :" << fileName <<"\n";
        return false;
    }

    auto iFileSize = file.size();

    QDataStream in(&file);
    char* buf = new char[iFileSize];

    in.readRawData(buf, iFileSize);
    file.close();


    QByteArray postData;
    postData.append(buf, iFileSize);

    QFileInfo fileInfo(fileName);
    if (m_strFileType.toLower() == fileInfo.suffix().toLower())
    {
       // NetworkHelper* pHelper = new NetworkHelper(fileName, Settings().xmlUrl(), postData);
        //connect(pHelper, SIGNAL(signal_Finished(QString,bool)), this, SLOT(DoXmlFileFinish(QString,bool)));
        NetworkHelper::getInstance()->excutePost(fileName, Settings().xmlUrl(), postData);
    }
    else
    {
        NetworkHelper::getInstance()->excutePost(fileName, Settings().serverUrl(), postData);
        //NetworkHelper* pHelper = new NetworkHelper(fileName, Settings().serverUrl(), postData);
        //connect(pHelper, SIGNAL(signal_Finished(QString,bool)), this, SLOT(DoSingleFileFinish(QString,bool)));
    }

    qDebug() << "send file, file name :" << fileName <<"\n";

    return true;
}

void SendCasesFile::listFile(QString directory, QStringList &files)
{
    QDir dir{directory};
    if (!dir.exists()) return;

    dir.setFilter(QDir::Files | QDir::Dirs);
    auto list = dir.entryInfoList();

    for (auto it : list) {
        auto name = it.fileName();
        if (name == "." || name == "..") continue;

        auto path = it.filePath();
        if (it.isDir())
            listFile(path, files);
        else
            files.push_back(path);
    }
}

void SendCasesFile::DoSingleFileFinish(QString fileName, bool bSucces)
{
    if(bSucces)
    {
        m_fileState.remove(fileName);
    }
    else
    {
        int iSendCount = m_fileState[fileName];
        if(iSendCount >= SENDFILETIME)
        {
            m_fileState.remove(fileName);
        }
        else
        {
            if(!ReadFile(fileName))
            {
                m_fileState.remove(fileName);
            }
            else
            {
                m_fileState[fileName] = iSendCount + 1;
                qDebug() << QString("Send the file(%1) for the %2 time").arg(fileName).arg(iSendCount + 1);
            }
        }
    }


    if(m_fileState.isEmpty())
    {
        emit OnFinishAll();
    }

}

void SendCasesFile::DoXmlFileFinish(QString fileName, bool bSucces)
{
    if(bSucces)
    {
       emit OnXmlFinish();
    }
    else {

    }
}
