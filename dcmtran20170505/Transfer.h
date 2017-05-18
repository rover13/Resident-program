#ifndef TRANSFER_H
#define TRANSFER_H

#include <QtCore>


class Transfer
{

public:
	~Transfer();
	Transfer(QString host, QString title, int port);
	bool isValid() const;
	QString store(QString fname);

private:
	struct Imp;
	QScopedPointer<Imp> imp;
};


#endif // TRANSFER_H
