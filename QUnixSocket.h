#include <QSocketNotifier>
#include <QObject>

class QUnixSocket: public QObject {
	Q_OBJECT;

public:
	QUnixSocket(const QString &socket, QObject *parent = 0);
	~QUnixSocket();

public slots:
	void activated(int sock);

signals:
	void message(const QByteArray&);

private:
	int sock;
	QString socket_path;
	QSocketNotifier *n;
};

