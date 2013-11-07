#include <QTcpSocket>
#include "QUnixSocket.h"

class Pubnub: public QObject {
	Q_OBJECT;
public:
	Pubnub(const QString &_hostname);

public slots:
	void connected();
	void disconnected();
	void flush();
	void doRead();
	void message(const QByteArray &);

private:
	bool genPacket(); // generate packet

	QUnixSocket *s;
	QTcpSocket *c;
	QString hostname;
	bool can_write;

	QList<QByteArray> queue;
	QByteArray buf;
};

