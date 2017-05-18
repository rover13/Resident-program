#include "OperationXML.h"
#include <QDebug>
#include "Settings.h"

OperationXML::OperationXML(QString strFileName)
    :m_fileName(strFileName)
{
    readXml();
}

OperationXML::~OperationXML()
{

}

int OperationXML::getFileTotal()
{
    return m_fileTatol;
}

void OperationXML::readXml()
{
    QFile file(m_fileName);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "read xml erro" << m_fileName;
        file.close();
        m_fileTatol = 0;
        return;
    }

    QDomDocument doc;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "read xml erro" << m_fileName << "   " << errorStr << errorLine << errorColumn;
        file.close();
        m_fileTatol = 0;
        return;
    }
    file.close();

    if(!filetotal(doc.documentElement()))
    {
        qDebug() << "The total number of reads the XML file";
    }

    modifyXml(doc);
}

bool OperationXML::filetotal(const QDomElement &element)
{
    auto node = element.firstChild();

    while (!node.isNull())
    {
        if (node.toElement().tagName() == "total")
        {
            m_fileTatol = node.toElement().text().toInt();
            return true;
        }

        node = node.nextSibling();
    }

    return false;
}

void OperationXML::modifyXml(const QDomDocument &doc)
{
    auto lists = doc.elementsByTagName("pacs");


    for(int i = 0; i < lists.count(); i++)
    {
        auto node = lists.at(i);

        auto nodeEle = node.firstChildElement("maurl");
        auto strUrl = nodeEle.text() + "&url=" + Settings().pacsUrl();
        nodeEle.firstChild().setNodeValue(strUrl);
    }

    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "open file: " << m_fileName <<  " error!";
    }

    QTextStream out(&file);
    doc.save(out,4);

    file.close();
}
