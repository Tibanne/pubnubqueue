#include <QTcpSocket>
#include <QTimer>
#include "QUnixSocket.h"

class Pubnub: public QObject {
	Q_OBJECT;
public:
	Pubnub(const QString &_hostname, const QString &_path);

public slots:
	void connected();
	void disconnected();
	void flush();
	void doRead();
	void message(const QByteArray &);
	void check();

private:
	bool genPacket(); // generate packet

	QUnixSocket *s;
	QTcpSocket *c;
	QString hostname;
	QTimer t;
	bool can_write;

	QList<QByteArray> queue;
	QByteArray buf;
};

