#include "Pubnub.h"

Pubnub::Pubnub(const QString &_hostname) {
	hostname = _hostname;
	s = new QUnixSocket("/tmp/pubnub.sock", this);
	c = new QTcpSocket(this);
	can_write = false;

	connect(s, SIGNAL(message(const QByteArray&)), this, SLOT(message(const QByteArray&)));
	connect(c, SIGNAL(connected()), this, SLOT(connected()));
	connect(c, SIGNAL(readyRead()), this, SLOT(doRead()));
	connect(c, SIGNAL(bytesWritten(qint64)), this, SLOT(flush()));
	connect(c, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(disconnected()));
	connect(c, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(&t, SIGNAL(timeout()), this, SLOT(check()));

	c->connectToHost(hostname, 80);
	t.setSingleShot(false);
	t.start(15000);
}

void Pubnub::check() {
	// check status of connection to pubnub
	switch(c->state()) {
		case QAbstractSocket::ConnectedState:
			break;
		default:
			// in any other state than ConnectedState: force disconnection/Reconnection
			disconnected();
	}
}

void Pubnub::message(const QByteArray &msg) {
	queue.append(msg);
	flush();
}

void Pubnub::connected() {
	can_write = true;
	buf.clear(); // avoid any half packet
	flush();
}

void Pubnub::disconnected() {
	c->close();
	qDebug("lost connection");
	can_write = false;
	// try to connect
	c->connectToHost(hostname, 80);
	// reset timer to 15 secs
	t.start(15000);
}

void Pubnub::doRead() {
	while(c->bytesAvailable())
		c->read(8192);
}

bool Pubnub::genPacket() {
	if (queue.isEmpty()) return false;
	QByteArray req = queue.takeFirst();
	buf.append("GET "+req+" HTTP/1.1\r\nHost: "+hostname.toUtf8()+"\r\n\r\n");
	return true;
}

void Pubnub::flush() {
	if (!can_write) return;

	while(true) {
		if (buf.isEmpty()) {
			if (!genPacket()) return; // nothing
		}
		// attempt to write
		qint64 l = c->write(buf);
		if (l < 0) {
			disconnected();
			return;
		}
		if (l == 0) break; // can't write anymore for now
		buf = buf.mid(l);
	}
}

