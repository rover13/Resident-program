#include "FileManage.h"
#include "OperationXML.h"
#include <QString>


FileManage::FileManage(QString Dir, QObject *parent)
    :QObject(parent), m_strDir(Dir), m_Timer(new QTimer(this)),
      m_sendPacs(new SendPacsFile), m_sendCases(new SendCasesFile), m_fileTotal(0)
{
    initConnet();
    startRun();
    m_Timer.data()->start(Settings().timeoutInt());

}

FileManage::~FileManage()
{
    int i = 0;
}

void FileManage::listFile(QString directory, QStringList &files)
{
    QDir dir{directory};
    if (!dir.exists()) return;

    dir.setFilter(QDir::Files | QDir::Dirs);
    auto list = dir.entryInfoList();

    for (auto it : list) {
        QString name = it.fileName();
        if (name == "." || name == "..") continue;

        auto path = it.filePath();
        if (it.isDir())
            listFile(path, files);
        else
            files.push_back(path);
    }
}

void FileManage::Timerout()
{
    m_Timer.data()->stop();
    this->deleteLater();
}

void FileManage::SendPacsFinish()
{
    m_sendCases->startSendCasesFile(m_strDir + "/" + Settings().casesDir().toStdString().c_str());
}

void FileManage::SendCasesFinish()
{
    if(m_strXmlFileName.isEmpty())
    {
        return;
    }
    m_sendCases->startSendXmlFile(m_strXmlFileName);
}

void FileManage::SendXmlFinish()
{
    removefilesindir(m_strDir);
    this->deleteLater();
}


void FileManage::initConnet()
{
    connect(m_sendPacs.data(), SIGNAL(OnFinish()), this, SLOT(SendPacsFinish()));
    connect(m_sendCases.data(), SIGNAL(OnFinishAll()), this, SLOT(SendCasesFinish()));
    connect(m_sendCases.data(), SIGNAL(OnXmlFinish()), this, SLOT(SendXmlFinish()));
    connect(m_Timer.data(), SIGNAL(timeout()), this, SLOT(Timerout()));
}

void FileManage::timerEvent(QTimerEvent *event)
{
    killTimer(m_updateTimer);

    startRun();
}

void FileManage::removefilesindir(const QString &path)
{
    QDir dir(path);
    QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs);
    foreach(QFileInfo file_info, info_list)
    {
      if (file_info.isDir())
      {
        removefilesindir(file_info.absoluteFilePath());
      }
      else if (file_info.isFile())
      {
        QFile file(file_info.absoluteFilePath());
        file.remove();
      }
    }

    QDir temp_dir;
    temp_dir.rmdir(path) ;
}

void FileManage::startRun()
{
    m_fileList.clear();
    listFile(m_strDir, m_fileList);

    if(0 == m_fileTotal)
    {
        foreach (auto filename, m_fileList)
        {
            if(filename.contains("xml"))
            {
                OperationXML xml(filename);
                m_fileTotal = xml.getFileTotal();
                m_strXmlFileName = filename;
            }
        }
    }


    if(m_fileList.size() != m_fileTotal)
    {
        m_updateTimer = startTimer(50);
        return;
    }
    else
    {
        if(Settings().softMode().toLower() == "remote")
        {
            m_sendPacs->UpLoad(Settings().host(),
                           Settings().title(),
                           Settings().port(),
                           m_strDir + "/" + Settings().pacsDir().toStdString().c_str());
        }
        else {
            SendPacsFinish();
        }
    }

    if(0 == m_fileTotal)
    {
        m_updateTimer = startTimer(50);
    }
}

