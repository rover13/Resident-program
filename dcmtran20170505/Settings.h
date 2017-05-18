#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>


class Settings
{
public:
	~Settings();
	Settings();

public:
	QString host() const;
	void setHost(QString h);
	QString title() const;
	void setTitle(QString t);
	int port() const;
	QString portStr() const;
	void setPort(int p);
	void setPort(QString ps);
	QString directory() const;
	void setDirectory(QString d);
	QStringList blocklist() const;

    void setSoftMode(QString d);
    QString softMode() const;

    void setWatcherPath(QString d);
    QString watcherPath() const;

    void setServerUrl(QString d);
    QString serverUrl() const;

    void setXmlUrl(QString d);
    QString xmlUrl() const;

    void setPacsUrl(QString d);
    QString pacsUrl() const;

    int timeoutInt() const;
    QString timeoutStr() const;

    void setTimeouts(int p);
    void setTimeouts(QString ps);

    void setPacsDir(QString d);
    QString pacsDir() const;

    void setCasesDir(QString d);
    QString casesDir() const;

private:
	static QSettings& instance();
};


#endif // SETTINGS_H
