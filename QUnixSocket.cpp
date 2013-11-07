#include <sys/stat.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "QUnixSocket.h"

QUnixSocket::QUnixSocket(const QString &_socket, QObject *parent): QObject(parent) {
	socket_path = _socket;

	unlink(socket_path.toLocal8Bit().data());

	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock < 0) {
		qDebug("Failed to create socket: %s\n", strerror(errno));
		return;
	}

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, socket_path.toLocal8Bit().data());

	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		qDebug("Failed to bind socket: %s\n", strerror(errno));
		close(sock);
		sock = -1;
		return;
	}

	chmod(socket_path.toLocal8Bit().data(), 0777);

	n = new QSocketNotifier(sock, QSocketNotifier::Read, this);
	connect(n, SIGNAL(activated(int)), this, SLOT(activated(int)));
}

QUnixSocket::~QUnixSocket() {
	if (sock > 0) close(sock);
}

void QUnixSocket::activated(int s) {
	char buf[65535];

	if (s != sock) return; // not mine

	int res = recv(sock, &buf, 65535, 0); //MSG_DONTWAIT);
	if (res == -1) {
		qDebug("Packet reception failed: %s", strerror(errno));
		return;
	}

	if (res == 0) return;
	message(QByteArray(buf, res));
}

