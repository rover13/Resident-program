#ifndef OPERATIONXML_H
#define OPERATIONXML_H

#include <QObject>
#include <QDomDocument>
#include <QFile>
#include <QDir>

class OperationXML
{
public:
    OperationXML(QString strFileName);
    ~OperationXML();

    int  getFileTotal();
private:
    void readXml();
    bool filetotal(const QDomElement &element);
    void modifyXml(const QDomDocument &doc);

private:
    QString m_fileName;
    int     m_fileTatol;
};

#endif // OPERATIONXML_H
