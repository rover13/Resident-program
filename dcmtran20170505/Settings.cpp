#include "Settings.h"

static auto _softmode = "Soft/mode";
static auto _watcherPath = "Soft/watcherPath";
static auto _timeout = "Soft/timeout";
static auto _pacsDir="Soft/pacsDir";
static auto _casesDir="Soft/casesDir";

static auto _host = "remote/host";
static auto _title = "remote/title";
static auto _port = "remote/port";
static auto _directory = "local/directory";
static auto _blocklist = "local/blocklist";

static auto _serverUrl = "cases/serverUrl";
static auto _xmlUrl = "cases/XmlUrl";
static auto _pacsUrl = "pacs/pacsUrl";

Settings::Settings() {}


Settings::~Settings() { instance().sync(); }


QString
Settings::host() const
{
	return instance().value(_host).toString();
}


void
Settings::setHost(QString h)
{
	instance().setValue(_host, h);
}


QString
Settings::title() const
{
	return instance().value(_title).toString();
}


void
Settings::setTitle(QString t)
{
	instance().setValue(_title, t);
}


int
Settings::port() const
{
	return instance().value(_port).toInt();
}


QString
Settings::portStr() const
{
	return instance().value(_port).toString();
}


void
Settings::setPort(int p)
{
	instance().setValue(_port, p);
}


void
Settings::setPort(QString ps)
{
	instance().setValue(_port, ps);
}


QString
Settings::directory() const
{
	return instance().value(_directory).toString();
}


void
Settings::setDirectory(QString d)
{
	instance().setValue(_directory, d);
}


QStringList
Settings::blocklist() const
{
	return instance().value(_blocklist).toString().split(',');
}

void Settings::setSoftMode(QString d)
{
    instance().setValue(_softmode, d);
}

QString Settings::softMode() const
{
    return instance().value(_softmode).toString();
}

void Settings::setWatcherPath(QString d)
{
    instance().setValue(_watcherPath, d);
}

QString Settings::watcherPath() const
{
    return instance().value(_watcherPath).toString();
}

void Settings::setServerUrl(QString d)
{
    instance().setValue(_serverUrl, d);
}

QString Settings::serverUrl() const
{
    return instance().value(_serverUrl).toString();
}

void Settings::setXmlUrl(QString d)
{
    instance().setValue(_xmlUrl, d);
}

QString Settings::xmlUrl() const
{
    return instance().value(_xmlUrl).toString();
}

void Settings::setPacsUrl(QString d)
{
    instance().setValue(_pacsUrl, d);
}

QString Settings::pacsUrl() const
{
    return instance().value(_pacsUrl).toString();
}

int Settings::timeoutInt() const
{
    return instance().value(_timeout).toInt();
}

QString Settings::timeoutStr() const
{
    return instance().value(_timeout).toString();
}

void Settings::setTimeouts(int p)
{
   instance().setValue(_timeout, p);
}

void Settings::setTimeouts(QString ps)
{
    instance().setValue(_timeout, ps);
}

void Settings::setPacsDir(QString d)
{
    instance().setValue(_pacsDir, d);
}

QString Settings::pacsDir() const
{
    return instance().value(_pacsDir).toString();
}

void Settings::setCasesDir(QString d)
{
    instance().setValue(_casesDir, d);
}

QString Settings::casesDir() const
{
    return instance().value(_casesDir).toString();
}

QSettings&
Settings::instance()
{
	static QSettings s("dcmtran.ini", QSettings::IniFormat);

#ifdef LOVE_GBK
    s.setIniCodec("GB2312");
#else
    s.setIniCodec("UTF8");
#endif

    static bool inited = false;
    if (!inited) {
        inited = true;

        static QPair<char const*, char const*> default_list[] = {
            {_host, "localhost"},
            {_title, "DCMQRSCP"},
            {_port, "11122"},
            {_blocklist, "txt,ini,inf,htm,html,shtml,js,css,jpg,jpeg,png,gif,"
                         "bmp,exe,dll,ocx,bat,cmd,sh,py,chm,zip,gz,7z,bz,bz2,"
                         "tgz,rar"},
            {_softmode, "remote"},
            {_watcherPath, "F:\\test1"},
            {_serverUrl, "http://xb.eastraycloud.net/api/upload/uploader"},
            {_xmlUrl, "http://xb.eastraycloud.net/api/upload/xml"},
            {_pacsUrl, "http://175.25.16.249:8000"},
            {_timeout, "10000000"},
            {_pacsDir, "影像"},
            {_casesDir, "附件"}
        };

        for (auto pair : default_list) {
            if (!s.contains(pair.first))
                s.setValue(pair.first, pair.second);
        }
        s.sync();
    }
	return s;
}
